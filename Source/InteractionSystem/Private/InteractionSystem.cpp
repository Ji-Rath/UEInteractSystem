// Copyright Epic Games, Inc. All Rights Reserved.

#include "InteractionSystem.h"

#include "InteractionSystem_Settings.h"

#if WITH_EDITOR
#include "ISettingsModule.h"
#endif

#define LOCTEXT_NAMESPACE "FInteractionSystemModule"

void FInteractionSystemModule::StartupModule()
{
#if WITH_EDITOR
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	if(ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings("Project", "Plugins", "InteractionSystem_Settings",
			LOCTEXT("RuntimeSettingsName", "Interaction System"), LOCTEXT("RuntimeSettingsDescription", "Configure my setting"),
			GetMutableDefault<UInteractionSystem_Settings>());
	}
#endif
}

void FInteractionSystemModule::ShutdownModule()
{
#if WITH_EDITOR
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	if(ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "InteractionSystem_Settings");
	}
#endif
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FInteractionSystemModule, InteractionSystem)