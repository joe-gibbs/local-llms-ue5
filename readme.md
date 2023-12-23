# Local LLM-powered NPCs with llama.cpp, Mistral7b and StyleTTS

This repo contains the code for [this demo](https://jgibbs.dev/blogs/local-llm-npcs-in-unreal-engine.html) of local LLM-powered NPCs in Unreal Engine 5.3. 
It is based on [StyleTTS](https://github.com/yl4579/StyleTTS), [llama.cpp](https://github.com/ggerganov/llama.cpp) and [Mistral7b](https://huggingface.co/mistralai/Mistral-7B-v0.1).

## Setup

1. Clone this repo
2. Download a .gguf of Mistral
3. Install and build [llama.cpp](https://github.com/ggerganov/llama.cpp) and run the server
4. Install mrfakename's [StyleTTS demo](https://huggingface.co/spaces/styletts2/styletts2%3E) and run the Docker image
5. Create an Unreal project. Unfortunately I couldn't include the Content folder because most of it is from the Marketplace and demo content from Epic, but you can create a new project based on the source code in the unreal folder. You will have to create a new Blueprint class based on the AICharacter class and set up your own interaction with it. I used the [RuntimeAudioImporter plugin](https://github.com/gtreshchev/RuntimeAudioImporter) to load the audio files at runtime.
6. Replace the harcoded addresses in the node/index.js file with your own addresses for the neutral voice sample file, the Gradio link to the StyleTTS demo, and the llama.cpp server address, as well as where you want the audio outputs to be saved to. Do the same in AICharacter.cpp.
7. When running node file you may experience issues with EventSource, in which case the best solution is to just patch the Gradio code to convert the URLs it's trying to parse into strings.