// Fill out your copyright notice in the Description page of Project Settings.

#include "NumberFalling.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
#include "Engine/EngineTypes.h"




ANumberFalling::ANumberFalling()
{
	
	renderComp = CreateDefaultSubobject<UTextRenderComponent>(TEXT("RenderedText"));
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));	
	BoxComponent->InitBoxExtent(FVector(12, 12, 12));
	
	RootComponent = BoxComponent;

	renderComp->SetupAttachment(RootComponent);

	isFalling = true;

}

void ANumberFalling::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{

	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANumberFalling::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndexType, bool bFromSweep, const FHitResult& SweepResult)
{

	ANumberFalling* other = Cast<ANumberFalling, AActor>(OtherActor);
	TArray<UActorComponent*> effect;
	FTimerHandle UnusedHandle2;
	FTimerDelegate timedFunction;
	timedFunction.BindUFunction(this, FName("DeleteNumbers"), OtherActor);

	if (materialComposition == 6) {

		if (isFalling) {

			thePlayer->streak++;
			thePlayer->AddScore(orangeBoxMultiplier);
			
			StopFalling();
			
		}

		effect = GetComponentsByTag(UActorComponent::StaticClass(), TEXT("Explosion"));
		effect[0]->Activate();
		Explode();
		
		GetWorld()->GetTimerManager().SetTimer(UnusedHandle2, timedFunction, 0.3f, false);

	}
	else if (OtherComp->ComponentHasTag("Wall") && isFalling) {

		StopFalling();
		thePlayer->streak = 0;

	}
	/*
		// 
			If the overlapped component is stacked up too high -> game over
		//
	*/
	else if (OtherComp->GetRelativeLocation().Z > 180.0f) {

		myGameMode->GameOver();
		this->Destroy();
		thePlayer->streak = 0;

	}
	else if (OtherComp->ComponentHasTag("Number") && other->materialComposition != 6) {
		
		if (CheckEqual(value, other->value)) {
			if (isFalling) {
			
				if (CheckMaterial(materialComposition, other->materialComposition)) MatchMultiplier();
				thePlayer->streak++;
				thePlayer->AddScore(scoreMultiplier);
				
				CallResult(value, other->value);

				StopFalling();

			}
			
			// Assigns the match effect based on number match or whether material matches as well
			if (CheckMaterial(materialComposition, other->materialComposition)) {
				effect = GetComponentsByTag(UActorComponent::StaticClass(), TEXT("ExactMatch"));
			}
			else {
				effect = GetComponentsByTag(UActorComponent::StaticClass(), TEXT("Match"));
			}
			
			effect[0]->Activate();		
			GetWorld()->GetTimerManager().SetTimer(UnusedHandle2, timedFunction, 0.3f, false);
			
		}
		else if (isFalling) {

			StopFalling();
			thePlayer->streak = 0;

		}

	}
		
}


/*
	//
	Active number movement on Tick
	//
*/

void ANumberFalling::Move()
{

	if (isFalling) {
		FVector newLocation;
		newLocation = GetActorLocation();
		newLocation.Z -= fallingSpeed;
		SetActorLocation(newLocation);
	}

}



/*
	//
	Move active number horizontally
	//
*/

void ANumberFalling::MoveHorizontal(float AxisValue)
{

	AddMovementInput(FVector(AxisValue, 0.0f, 0.0f), 1.0f, false);

}

/*
	Drops active number down quickly
*/

void ANumberFalling::DropDown(float AxisValue)
{

	AddMovementInput(FVector(0.0f, 0.0f, AxisValue), 1.0f, false);

}


/*
	//
	Generates new number value
	//
*/
void ANumberFalling::GenerateValue()
{

	// If it is special type of box
	if (materialComposition == 6) {
	value = 0;
	}
	else if (thePlayer->playerLevel < 5)
	{	
		value = FMath::FloorToInt(FMath::RandRange(1, 7));
	}
	else if (thePlayer->playerLevel < 10)
	{
		value = FMath::FloorToInt(FMath::RandRange(1, 15));
	}
	else
	{
		value = FMath::FloorToInt(FMath::RandRange(1, 25));
	}

}


/*
	//
	Generates number box material type based on random chance, or makes a special effect box, also assigns the material
	//
*/
void ANumberFalling::GenerateColor()
{

	UActorComponent* staticMesh = GetComponentByClass(UStaticMeshComponent::StaticClass());
	float chance;
	myCube = Cast<UStaticMeshComponent, UActorComponent >(staticMesh);

	NormalizeChances();
	do { chance = FMath::RandRange(0.0f, highestChance); } while (chance == 0.0f);
		

	if (myCube) {
		if (chance <= plainMaterialChance) {
			//brick
			myCube->SetMaterial(0, materialBrick);
			materialComposition = 2;
		}
		else if (chance <= redMaterialChance ) {
			//bronze
			myCube->SetMaterial(0, materialCopper);
			materialComposition = 3;
		}
		else if (chance <= blueMaterialChance) {
			//silver
			myCube->SetMaterial(0, materialSilver);
			materialComposition = 4;
		}
		else if (chance <= greenMaterialChance) {
			//Gold
			myCube->SetMaterial(0, materialGold);
			materialComposition = 5;
		}
		else {
			myCube->SetMaterial(0, materialHyper);
			materialComposition = 6;
			
			TArray<UActorComponent*> radius = GetComponentsByTag(UActorComponent::StaticClass(), TEXT("ExplosionRadius"));
			explosionRadius = Cast<USphereComponent, UActorComponent>(radius[0]);
			explosionRadius->GetChildComponent(0)->SetHiddenInGame(false);
			
		}
	}

}

/*
	//
	Sends the last successful equation as text to UI to show
	//
*/
void ANumberFalling::CallResult(int A, int B)
{

	FString resultCalled;
	if (A + B == neededTotal) {
		resultCalled = FString(FString::FromInt(A) + " + " + FString::FromInt(B) + " = " + FString::FromInt(neededTotal));
	}
	else if (A - B == neededTotal) {
		resultCalled = FString(FString::FromInt(A) + " - " + FString::FromInt(B) + " = " + FString::FromInt(neededTotal));
	}
	else if (B - A == neededTotal) {
		resultCalled = FString(FString::FromInt(B) + " - " + FString::FromInt(A) + " = " + FString::FromInt(neededTotal));
	}
	else if (A * B == neededTotal) {
		resultCalled = FString(FString::FromInt(A) + " * " + FString::FromInt(B) + " = " + FString::FromInt(neededTotal));
	}
	else if (A / B == neededTotal) {
		resultCalled = FString(FString::FromInt(A) + " / " + FString::FromInt(B) + " = " + FString::FromInt(neededTotal));
	}
	else if (B / A == neededTotal) {
		resultCalled = FString(FString::FromInt(B) + " / " + FString::FromInt(A) + " = " + FString::FromInt(neededTotal));
	}
	thePlayer->lastEquation = resultCalled;

}

/*
	//
		Stop the falling of the number and stops the falling movement timer.
	//
*/
void ANumberFalling::StopFalling()
{

	isFalling = false;
	myGameMode->numberIsActive = false;
	GetWorld()->GetTimerManager().ClearTimer(UnusedHandle);

}
/*
	//
		Transforms Unreal Developer input chances into code-usable values
	//
*/
void ANumberFalling::NormalizeChances()
{

	highestChance = plainMaterialChance + redMaterialChance + blueMaterialChance + greenMaterialChance + explosiveBoxChance;
	redMaterialChance += plainMaterialChance;
	blueMaterialChance += redMaterialChance;
	greenMaterialChance += blueMaterialChance;

}

/*
	//
		Function related to ExplosionBox, destroys all overlapping actors within the explosion radius
	//
*/
void ANumberFalling::Explode()
{
	explosionRadius->SetGenerateOverlapEvents(true);
	TArray<AActor*> numbersWithinExplosion;
	
	UpdateOverlaps();
	GetOverlappingActors(numbersWithinExplosion, ANumberFalling::StaticClass());
	for (int i = 0; i < numbersWithinExplosion.Num(); i++) {
		numbersWithinExplosion[i]->Destroy();
	}

}




/*
	//
	Timed function to delete both numbers after match effect has gone through
	//
*/
void ANumberFalling::DeleteNumbers(AActor* OtherActor)
{

	this->Destroy();

}

/*
	//
	Generates required result for possible equations
	//
*/
void ANumberFalling::GenerateTotal()
{

	if (materialComposition == 6)
	{
		neededTotal = 0;
	}
	else {
		if (thePlayer->playerLevel < 5) {
			neededTotal = FMath::FloorToInt(FMath::RandRange(value + 1, 10));
		}
		else if (thePlayer->playerLevel < 10)
		{
			neededTotal = FMath::FloorToInt(FMath::RandRange(value + 1, 20));
		}
		else
		{
			neededTotal = FMath::FloorToInt(FMath::RandRange(value + 1, 30));
		}
	}
	
	thePlayer->neededTotal = neededTotal;

}


/*
	//
	Checks if two given numbers have any relationship between them, if so changes score multiplier accordingly for different operations
	//
*/
bool ANumberFalling::CheckEqual(int A, int B)
{

	neededTotal = thePlayer->neededTotal;
	if (A + B == neededTotal || A - B == neededTotal || B - A == neededTotal) {
		scoreMultiplier = 1;
		return true;

	}
	else if (A * B == neededTotal || (A / B == neededTotal && A % B == 0) || (B / A == neededTotal && B % A == 0)) {
		scoreMultiplier = 2;
		return true;
	}
	else
	return false;

}

/*
	//
	Checks material of both boxes, if it's matching it adjusts score multiplier based on the material value
	//
*/
bool ANumberFalling::CheckMaterial(int A, int B)
{

	return (A == B);

}

void ANumberFalling::MatchMultiplier()
{
	scoreMultiplier *= materialComposition;
}


void ANumberFalling::BeginPlay()
{

	Super::BeginPlay();

	PossessedBy(GetController());

	myGameMode = Cast<AMathTetrisGameModeBase>(GetWorld()->GetAuthGameMode());
	thePlayer = Cast<AMathTetrisPlayerState, APlayerState>(GetPlayerState());
	previousNumber = thePlayer->previousNumber;
	fallingSpeed = thePlayer->fallingSpeed;

	GenerateColor();

	do { GenerateValue(); } while (value == previousNumber && materialComposition != 6);
	thePlayer->previousNumber = value;
	
	GenerateTotal();
	FText text = FText::AsNumber(value);
	
	
	renderComp->SetVerticalAlignment(EVRTA_TextCenter);
	renderComp->SetHorizontalAlignment(EHTA_Center); \
	renderComp->SetWorldRotation(FRotator(0.0f, 90.0f, 0.0f));
	renderComp->SetTextRenderColor(FColor::Black);
	renderComp->SetRelativeLocation(FVector(0.0f, 10.0f, 0.0f));
	renderComp->SetXScale(0.85f);
	renderComp->SetYScale(0.85f);
	renderComp->SetText(text);

	

	if (isFalling) {
		BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ANumberFalling::OnOverlapBegin);
	}
	

	
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &ANumberFalling::Move, 0.05f, true);


}


