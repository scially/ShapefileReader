// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FShapefileReaderModule : public IModuleInterface
{
public:
	FShapefileReaderModule();
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
private:
	FString gdal_dir_;
	FString proj_dir_;
};
