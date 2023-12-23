import * as Gradio from "@gradio/client";
import * as FS from "fs";
import EventSource from "eventsource";

global.EventSource = EventSource;

const voices = {
  netural: FS.readFileSync(
    "C:/Users/joegi/Documents/AI/CharacterServer/voices/neutral.m4a"
  ),
};

const main = async () => {
  const ttsApp = await Gradio.client("http://localhost:7860");
  let response = "";
  let responses = [];

  const [_, __, prompt, speaker1, speaker2] = process.argv;

  const body = JSON.stringify({
    stream: true,
    n_predict: 400,
    temperature: 0.7,
    stop: ["</s>", `${speaker1}:`, `${speaker2}:`, `${speaker1.charAt(0)}:`, `${speaker2.charAt(0)}:`],
    repeat_last_n: 256,
    repeat_penalty: 1.18,
    top_k: 40,
    top_p: 0.5,
    min_p: 0.05,
    tfs_z: 1,
    typical_p: 1,
    presence_penalty: 0,
    frequency_penalty: 0,
    mirostat: 0,
    mirostat_tau: 5,
    mirostat_eta: 0.1,
    grammar: "",
    n_probs: 0,
    image_data: [],
    cache_prompt: true,
    slot_id: 0,
    prompt,
  })

  let isCallingSynth = false;

  const synth = async (text) => {
    while (isCallingSynth) {
      await new Promise((r) => setTimeout(r, 100));
    }
    isCallingSynth = true;

    const result = await ttsApp.predict("clsynthesize", [
      text,
      voices.netural,
      5,
    ]);

    const obj = await fetch(result.data[0].url);

    const randomUUID = `C:/Users/joegi/Documents/AI/CharacterServer/out/${Math.random()
      .toString(36)
      .substring(2, 15)}.wav`;
    FS.writeFileSync(
      randomUUID,

      Buffer.from(await obj.arrayBuffer())
    );

    const res = {
      subtitle: text,
      audio: randomUUID,
    };
    
    process.stdout.write(JSON.stringify(res));

    isCallingSynth = false;
    return res;
  };
  
  await fetch("http://127.0.0.1:8080/completion", {
    headers: {
      "Content-Type": "application/json",
    },
    method: "POST",
    body,
  })
    // Use the response as a readable stream
    .then((response) => response.body)
    .then((body) => {
      const reader = body?.getReader();
      const decoder = new TextDecoder();
      const pump = () => {
        return reader?.read().then(({ done, value }) => {
          if (done) {
            return;
          }
          try {
            let decoded = decoder.decode(value);
            decoded = decoded.replace("data: ", "");
            const parsed = JSON.parse(decoded);
            const body = parsed.content;

            response += body;
            if (response.split(".") != response) {
              responses.push(synth(response.trim()));
              response = "";
            }

            return pump();
          } catch (error) {
            return pump();
          }
        });
      };

      return new ReadableStream({
        start(controller) {
          pump().then(() => controller.close());
        },
      });
    })
    .then((stream) => stream.getReader().read());

  await Promise.all(responses);

  process.exit(0);
};

main();
