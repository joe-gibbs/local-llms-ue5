// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"
#include "AIGameState.h"
#include "HttpModule.h"
#include "Json.h"
#include "Misc/InteractiveProcess.h"

// Sets default values
AAICharacter::AAICharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AAICharacter::AddToConversation(FString Line)
{
	bIsLoadingResponse = true; 
	FString Setting = GetWorld()->GetGameState<AAIGameState>()->Setting.ToString();
	FString PlayerName = GetWorld()->GetGameState<AAIGameState>()->PlayerName;
	ConversationHistory.Push({
		Line,
		PlayerName,
		""
	});
	FString input = FString::Format(TEXT("This is a conversation between {0} and {1}. {2} The setting is {3} \n\n"), { PlayerName, Name, Prompt, Setting });

	for (FConversationLine FLine : ConversationHistory)
	{
		input += FString::Format(TEXT("{0}: {1} \n"), { FLine.Speaker, FLine.Line });
	}

	input += FString::Format(TEXT("{0}:"), { Name });
	FString params = FString::Format(TEXT("C:\\Users\\joegi\\Documents\\AI\\CharacterServer\\index.js \"{0}\" \"{1}\" \"{2}\""), {input, PlayerName, Name});

	// print to log
	UE_LOG(LogTemp, Warning, TEXT("Params: %s"), *params);
	
	auto proc = new FInteractiveProcess(
		TEXT("C:\\Program Files\\nodejs\\node.exe"),
		params, true
	);
	proc->OnOutput().BindLambda([this](const FString& Output)
	{
		FString JsonString = Output;
		TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonString);
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
		FJsonSerializer::Deserialize(JsonReader, JsonObject);
		const FString Subtitle = JsonObject->GetStringField("subtitle");
		const FString Audio = JsonObject->GetStringField("audio");
		
		ConversationHistory.Push({
			Subtitle,
			Name,
			Audio
		});

		UpcomingLines.Add(FConversationLine{ Subtitle, Name, Audio });
	});
	
	proc->OnCompleted().BindLambda([this](int32 ReturnCode, bool bSuccess)
	{
		bIsLoadingResponse = false;
	});

	proc->Init();
	proc->Launch();
}

// Called every frame
void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
