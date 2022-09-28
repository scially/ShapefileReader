// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShapefileReader.h"
THIRD_PARTY_INCLUDES_START
#include "gdal_frmts.h"
#include "ogrsf_frmts.h"
THIRD_PARTY_INCLUDES_END
#define LOCTEXT_NAMESPACE "FShapefileReaderModule"

FShapefileReaderModule::FShapefileReaderModule()
{
	gdal_dir_ = FPaths::Combine(FPaths::ProjectDir(), TEXT("Plugins/ShapefileReader/Resources"), TEXT("GDAL_DATA"));
	proj_dir_ = FPaths::Combine(FPaths::ProjectDir(), TEXT("Plugins/ShapefileReader/Resources"), TEXT("PROJ_LIB"));
}

void FShapefileReaderModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	GDALAllRegister();
	CPLSetConfigOption("GDAL_DATA", TCHAR_TO_UTF8(*gdal_dir_));
	const char *const proj_lib_path[] = {TCHAR_TO_UTF8(*proj_dir_), nullptr};
	OSRSetPROJSearchPaths(proj_lib_path);
}

void FShapefileReaderModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FShapefileReaderModule, ShapefileReader)