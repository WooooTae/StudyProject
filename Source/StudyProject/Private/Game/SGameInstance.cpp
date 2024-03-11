// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SUnrealObjectClass.h"
#include "Examples\SFlyable.h"
#include "Examples\SPigeon.h"
#include "JsonObjectConverter.h"
#include "UObject\SavePackage.h"

USGameInstance::USGameInstance()
{
	UE_LOG(LogTemp, Log, TEXT("USGameInstance::USGameInstance() has been called."));

	Name = TEXT("USGameInstance class Default Object");
}

void USGameInstance::Init()
{
	Super::Init();

	FBirdData SrcRawData(TEXT("Piegeon17"), 17);
	UE_LOG(LogTemp, Log, TEXT("[SrcRawData] Name: %s, ID: %d"), *SrcRawData.Name, SrcRawData.ID);

	const FString SaveDir = FPaths::Combine(FPlatformMisc::ProjectDir(), TEXT("Saved"));
	UE_LOG(LogTemp, Log, TEXT("SaveDir: %s"), *SaveDir);

	const FString RawDataFileName(TEXT("RawData.bin"));
	FString AbsolutePathForRawData = FPaths::Combine(*SaveDir, *RawDataFileName);
	UE_LOG(LogTemp, Log, TEXT("Relative path for saved file: %s"), *AbsolutePathForRawData);
	FPaths::MakeStandardFilename(AbsolutePathForRawData);
	UE_LOG(LogTemp, Log, TEXT("Absolute path for saved file: %s"), *AbsolutePathForRawData);

	FArchive* RawFileWriterAr = IFileManager::Get().CreateFileWriter(*AbsolutePathForRawData);
	if (nullptr != RawFileWriterAr)
	{
		*RawFileWriterAr << SrcRawData;
		RawFileWriterAr->Close();
		delete RawFileWriterAr;
		RawFileWriterAr = nullptr;
	}

	FBirdData DstRawData;
	FArchive* RawFileReadAr = IFileManager::Get().CreateFileReader(*AbsolutePathForRawData);
	if (nullptr != RawFileReadAr)
	{
		*RawFileReadAr << DstRawData;
		RawFileReadAr->Close();
		delete RawFileReadAr;
		RawFileReadAr = nullptr;

		UE_LOG(LogTemp, Log, TEXT("[DstRawData] Name: %s, ID: %d"), *DstRawData.Name, DstRawData.ID);
	}

	SerializedPigeon = NewObject<USPigeon>();
	SerializedPigeon->SetName(TEXT("Pigeon76"));
	SerializedPigeon->SetID(76);
	UE_LOG(LogTemp, Log, TEXT("[SerializedPigeon] Name: %s, ID: %d"), *SerializedPigeon->GetName(), SerializedPigeon->GetID());

	const FString ObjectDataFileName(TEXT("ObjectData.bin"));
	FString AbsolutePathForObjectData = FPaths::Combine(*SaveDir, *ObjectDataFileName);
	FPaths::MakeStandardFilename(AbsolutePathForObjectData);

	TArray<uint8> BufferArray;
	FMemoryWriter MemoryWriterAr(BufferArray);
	SerializedPigeon->Serialize(MemoryWriterAr);

	TUniquePtr<FArchive> ObjectDataFileWriterAr = TUniquePtr<FArchive>(IFileManager::Get().CreateFileWriter(*AbsolutePathForObjectData));
	if (nullptr != ObjectDataFileWriterAr)
	{
		*ObjectDataFileWriterAr << BufferArray;
		ObjectDataFileWriterAr->Close();

		ObjectDataFileWriterAr = nullptr; //delete ObjectDataFileWriterAr; 와 같은 효과.
	}

	TArray<uint8> BufferArrayFromObjectDataFile;
	TUniquePtr<FArchive> ObjectDataFileReaderAr = TUniquePtr<FArchive>(IFileManager::Get().CreateFileReader(*AbsolutePathForObjectData));
	if (nullptr != ObjectDataFileReaderAr)
	{
		*ObjectDataFileReaderAr << BufferArrayFromObjectDataFile;
		ObjectDataFileReaderAr->Close();

		ObjectDataFileReaderAr = nullptr;
	}

	FMemoryReader MemoryReaderAr(BufferArrayFromObjectDataFile);
	USPigeon* pigeon77 = NewObject<USPigeon>();
	pigeon77->Serialize(MemoryReaderAr);
	UE_LOG(LogTemp, Log, TEXT("[Pigeon77] Name: &s, ID: %d"), *pigeon77->GetName(), pigeon77->GetID());

	const FString JsonDataFileName(TEXT("StudyJsonFile.txt"));
	FString AbsolutePathForJsonData = FPaths::Combine(*SaveDir, *JsonDataFileName);
	FPaths::MakeStandardFilename(AbsolutePathForJsonData);

	TSharedRef<FJsonObject> SrcJsonObject = MakeShared<FJsonObject>();
	FJsonObjectConverter::UStructToJsonObject(SerializedPigeon->GetClass(), SerializedPigeon, SrcJsonObject);

	FString JsonOutString;
	TSharedRef<TJsonWriter<TCHAR>> JsonWriteAr = TJsonWriterFactory<TCHAR>::Create(&JsonOutString);
	if (true == FJsonSerializer::Serialize(SrcJsonObject, JsonWriteAr))
	{
		FFileHelper::SaveStringToFile(JsonOutString, *AbsolutePathForJsonData);
	}

	FString JsonInString;
	FFileHelper::LoadFileToString(JsonInString, *AbsolutePathForJsonData);
	TSharedRef<TJsonReader<TCHAR>> JsonReaderAr = TJsonReaderFactory<TCHAR>::Create(JsonInString);

	TSharedPtr<FJsonObject> DstJsonObject;
	if (true == FJsonSerializer::Deserialize(JsonReaderAr, DstJsonObject)) 
	{
		USPigeon* Pigeon78 = NewObject<USPigeon>();
		if (true == FJsonObjectConverter::JsonObjectToUStruct(DstJsonObject.ToSharedRef(), Pigeon78->GetClass(), Pigeon78))
		{
			UE_LOG(LogTemp, Log, TEXT("[Pigeon78] Name: %s, ID: %d"), *Pigeon78->GetName(), Pigeon78->GetID());
		}
	}
}

void USGameInstance::Shutdown()
{
	Super::Shutdown();

	UE_LOG(LogTemp, Log, TEXT("USGanmeInstance::Shutdown() has been called."));
}
