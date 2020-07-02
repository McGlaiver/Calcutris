// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextRenderComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystem.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "MathTetrisPlayerState.h"
#include "MathTetrisGameModeBase.h"

#include "NumberFalling.generated.h"


/**
 * 
 */

UCLASS()
class MATHTETRIS_API ANumberFalling : public APawn
{
	GENERATED_BODY()


public:
	ANumberFalling();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool isFalling;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BoxComponent")
		UBoxComponent* BoxComponent;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int value;

	UPROPERTY(EditAnywhere)
		int orangeBoxMultiplier = 5;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> newLetter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* frameBox;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		AMathTetrisPlayerState* thePlayer;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* myCube;

	UPROPERTY(EditAnywhere)
		UMaterialInstance* materialBrick;

	UPROPERTY(EditAnywhere)
		UMaterialInstance* materialCopper;

	UPROPERTY(EditAnywhere)
		UMaterialInstance* materialSilver;

	UPROPERTY(EditAnywhere)
		UMaterialInstance* materialGold;

	UPROPERTY(EditAnywhere)
		UMaterialInstance* materialHyper;

	UPROPERTY(EditAnywhere)
		UParticleSystem* explosionEffect;

	UPROPERTY(EditAnywhere)
		UParticleSystem* matchEffect;

	UPROPERTY(EditAnywhere, Category = "Material Chances", meta = ( ClampMin = 0.0f))
		float plainMaterialChance;

	UPROPERTY(EditAnywhere, Category = "Material Chances", meta = (ClampMin = 0.0f))
		float redMaterialChance;

	UPROPERTY(EditAnywhere, Category = "Material Chances", meta = (ClampMin = 0.0f))
		float blueMaterialChance;

	UPROPERTY(EditAnywhere, Category = "Material Chances", meta = (ClampMin = 0.0f))
		float greenMaterialChance;

	UPROPERTY(EditAnywhere, Category = "Material Chances", meta = (ClampMin = 0.0f))
		float explosiveBoxChance;



	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndexType, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void DeleteNumbers(AActor* OtherActor);

	FVector currentLocation;

	UTextRenderComponent* renderComp;

	AMathTetrisGameModeBase* myGameMode;

	FTimerHandle UnusedHandle;

	USphereComponent* explosionRadius;
	
	float highestChance = 1.0f;

	float fallingSpeed;

	int previousNumber = -1;

	int materialComposition = 1;

	int scoreMultiplier = 0;
	
	int neededTotal;
	
	

	virtual void MoveHorizontal(float AxisValue);

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	void Move();

	void DropDown(float AxisValue);
	
	void GenerateTotal();

	bool CheckEqual(int A, int B);

	bool CheckMaterial(int A, int B);

	void MatchMultiplier();
	
	void GenerateValue();

	void GenerateColor();

	void CallResult(int A, int B);

	void StopFalling();

	void NormalizeChances();

	void Explode();




	
	
protected:
	virtual void BeginPlay() override;


	
};
