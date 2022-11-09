// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#define DEFAULT_BOARD_WIDTH 10
#define DEFAULT_BOARD_HEIGHT 10
#define DEFAULT_BOMB_COUNT 10

class FMinesweeperPluginGameData;
class FMinesweeperPluginGameManager;
class FMinesweeperPluginGameView;

class FMinesweeperPluginModule : public IModuleInterface, public TSharedFromThis<FMinesweeperPluginModule>
{
public:

    static FMinesweeperPluginModule& Get();
    static FMinesweeperPluginModule* GetPtr();

    // Handle toolbar button
    static void OnToolbarButtonPressed();

    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

    // Accessors
    TSharedPtr<FMinesweeperPluginGameData> GetGameData() const { return GameData; };
    TSharedPtr<FMinesweeperPluginGameManager> GetGameManager() const { return GameManager; };
    TSharedPtr<FMinesweeperPluginGameView> GetGameView() const { return GameView; };

protected:

    TSharedPtr<FMinesweeperPluginGameData> GameData;
    TSharedPtr<FMinesweeperPluginGameManager> GameManager;
    TSharedPtr<FMinesweeperPluginGameView> GameView;
};
