//SPlayer.cpp.


#include "Characters/SPlayerPawn.h"
#include "Components\CapsuleComponent.h"
#include "GameFramework\FloatingPawnMovement.h"
#include "GameFramework\SpringArmComponent.h"
#include "Camera\CameraComponent.h"

ASPlayerPawn::ASPlayerPawn()
{
	PrimaryActorTick.bCanEverTick = false;

	float CharacterHalfHeight = 95.f;
	float CharacterRadius = 40.f;

#pragma region InitializeCapsuleComponent
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	SetRootComponent(CapsuleComponent);
	CapsuleComponent->SetCapsuleHalfHeight(CharacterHalfHeight);
	CapsuleComponent->SetCapsuleRadius(CharacterRadius);
#pragma endregion

#pragma region InitializeSkeletaMesh
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);
	FVector PivotPositon(0.f, 0.f, -CharacterHalfHeight);
	FRotator PivotRotation(0.f, -90.f, 0.f);
	SkeletalMeshComponent->SetRelativeLocationAndRotation(PivotPositon, PivotRotation);

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassinfo(TEXT("/Script/Engine.AnimBlueprint'/Game/StudyProject/Animations/AnimationBlueprint/ABP_PlayerPawn.ABP_PlayerPawn_C'"));
	if (true == AnimInstanceClassinfo.Succeeded())
	{
		SkeletalMeshComponent->SetAnimClass(AnimInstanceClassinfo.Class);
	}
#pragma endregion

#pragma region InitializeCamera
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 400.f;
	SpringArmComponent->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
#pragma endregion

	FloatingPawnMovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatinPawnMovementComponent"));
}

void ASPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &ThisClass::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &ThisClass::LeftRight);
}

void ASPlayerPawn::BeginPlay()
{
	Super::BeginPlay();
}

void ASPlayerPawn::UpDown(float InAxisValue)
{
	AddMovementInput(GetActorForwardVector(), InAxisValue);
}

void ASPlayerPawn::LeftRight(float InAxisValue)
{
	AddMovementInput(GetActorRightVector(), InAxisValue);
}
