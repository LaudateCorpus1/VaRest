// Copyright 2014-2019 Vladimir Alyamkin. All Rights Reserved.

#include "VaRest.h"

#include "VaRestDefines.h"
#include "VaRestSettings.h"

#include "Developer/Settings/Public/ISettingsModule.h"

#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FVaRestModule"

void FVaRestModule::StartupModule()
{
	ModuleSettings = NewObject<UVaRestSettings>(GetTransientPackage(), "VaRestSettings", RF_Standalone);
	ModuleSettings->AddToRoot();

	// Register settings
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings("Project", "Plugins", "VaRest",
			LOCTEXT("RuntimeSettingsName", "VaRest"),
			LOCTEXT("RuntimeSettingsDescription", "Configure VaRest plugin settings"),
			ModuleSettings);
	}

	UE_LOG(LogVaRest, Log, TEXT("%s: VaRest (%s) module started"), *VA_FUNC_LINE, *GetPluginVersion());
}

void FVaRestModule::ShutdownModule()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "VaRest");
	}

	if (!GExitPurge)
	{
		ModuleSettings->RemoveFromRoot();
	}
	else
	{
		ModuleSettings = nullptr;
	}
}

UVaRestSettings* FVaRestModule::GetSettings() const
{
	check(ModuleSettings);
	return ModuleSettings;
}

FString FVaRestModule::GetPluginVersion() const
{
	IPluginManager& manager = IPluginManager::Get();
	TSharedPtr<IPlugin> const plugin = manager.FindPlugin("VaRest");

	return !plugin.IsValid() ? FString() : plugin->GetDescriptor().VersionName;
}

IMPLEMENT_MODULE(FVaRestModule, VaRest)

DEFINE_LOG_CATEGORY(LogVaRest);

#undef LOCTEXT_NAMESPACE
