#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class UToolMenu;

class FPlayTradeXEditorModule : public IModuleInterface
{
public:

    virtual void StartupModule() override;

    virtual void ShutdownModule() override;


private:

    // ============================================================
    // Menus
    // ============================================================

    void RegisterMenus();

    void RegisterMainMenu();

    void PopulatePlayTradeXMenu(
        UToolMenu* Menu
    );


    // ============================================================
    // Menu Actions
    // ============================================================

    void OpenAbout();

    void OpenAbiConverter();

    void ImportSample();

    void OpenSettings();
};