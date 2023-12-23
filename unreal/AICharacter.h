// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "AICharacter.generated.h"

USTRUCT(BlueprintType)
struct FConversationLine
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conversation")
	FString Line;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conversation")
	FString Speaker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conversation")
	FString Audio;
};

UCLASS()
class GAUL_API AAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAICharacter();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TArray<FConversationLine> UpcomingLines;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FString CurrentLine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool bIsLoadingResponse;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	TArray<FConversationLine> ConversationHistory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FString Prompt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FString Name;

	UFUNCTION(BlueprintCallable, Category = "AI")
	void AddToConversation(FString Line);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
