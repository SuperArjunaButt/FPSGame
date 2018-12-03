// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlackHole.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UPrimitiveComponent;

UCLASS()
class FPSGAME_API ABlackHole : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlackHole();

private:


	TArray<UPrimitiveComponent*> OverlappingComps;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = BLAGOLE)
	UStaticMeshComponent* BlagoleMesh;

	UPROPERTY(EditDefaultsOnly, Category = BLAGOLE)
	float magnitude = 2.0f;

	UPROPERTY(EditDefaultsOnly, Category = BLAGOLE)
	USphereComponent* AttractionSphere;

	UPROPERTY(EditDefaultsOnly, Category = BLAGOLE)
	USphereComponent* DestructionSphere;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()     
	void DestroyObj(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


};
