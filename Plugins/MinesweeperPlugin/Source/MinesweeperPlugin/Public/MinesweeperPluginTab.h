// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Docking/SDockTab.h"

// Plugin classes
class FMinesweeperPluginGameView;

// UE classes
class SBox;
class SButton;
class SCanvas;
class SSlider;
template<typename NumericType>
class SSpinBox;
struct FSlateColor;

class MINESWEEPERPLUGIN_API SMinesweeperPluginTab : public SDockTab
{

public:

    // Setup slate args
    SLATE_BEGIN_ARGS(SMinesweeperPluginTab)
        : _GameView(nullptr)
    { }
    SLATE_ARGUMENT(TSharedPtr<FMinesweeperPluginGameView>, GameView)
        SLATE_END_ARGS()

public:

    static const FName MinesweeperPluginTabName;

    SMinesweeperPluginTab();

    void Construct(const FArguments& InArgs);

    // Accessors
    TSharedPtr<SSpinBox<uint8>> GetMinesweeperBoardWidthSlider() const { return MinesweeperBoardWidthSlider; };
    TSharedPtr<SSpinBox<uint8>> GetMinesweeperBoardHeightSlider() const { return MinesweeperBoardHeightSlider; };
    TSharedPtr<SSpinBox<uint16>> GetMinesweeperBombSlider() const { return MinesweeperBombsSlider; };
    TSharedPtr<SButton> GetStartNewGameButton() const { return StartNewGameButton; };
    TSharedPtr<SBox> GetMinesweeperBoardContainer() const { return MinesweeperContainer; };
    TSharedPtr<SCanvas> GetMinesweeperBoard() const { return MinesweeperBoard; };

protected:

    // Plugin
    TSharedPtr<FMinesweeperPluginGameView> GameView;

    // UE
    TSharedPtr<SSpinBox<uint8>> MinesweeperBoardWidthSlider;
    TSharedPtr<SSpinBox<uint8>> MinesweeperBoardHeightSlider;
    TSharedPtr<SSpinBox<uint16>> MinesweeperBombsSlider;
    TSharedPtr<SButton> StartNewGameButton;
    TSharedPtr<SBox> GameWindow;
    TSharedPtr<SBox> MinesweeperContainer;
    TSharedPtr<SCanvas> MinesweeperBoard;
    TSharedPtr<SOverlay> MinesweeperBoardOverlay;

    // UI Accessors
    TOptional<uint8> GetMaximumMinesweeperBoardWidth() const;
    TOptional<uint8> GetMaximumMinesweeperBoardHeight() const;
    TOptional<uint16> GetMaximumBombs() const;
    FSlateColor GetBombsSliderBarColour() const;
    FText GetMinesweeperBombs() const;
    FText GetCurrentGameStatusText() const;
    FText GetCurrentCellsRemaining() const;
};
