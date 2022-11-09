// Fill out your copyright notice in the Description page of Project Settings.


#include "MinesweeperPluginTab.h"
#include "MinesweeperPluginStyle.h"
#include "MinesweeperPluginModule.h"
#include "MinesweeperPluginGameView.h"
#include "MinesweeperPluginGameManager.h"

#include "Styling/SlateStyle.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SSlider.h"
#include "Widgets/Input/SSpinBox.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/SCanvas.h"

#define LOCTEXT_NAMESPACE "FMinesweeperPluginModule"

const FName SMinesweeperPluginTab::MinesweeperPluginTabName("Minesweeper");

SMinesweeperPluginTab::SMinesweeperPluginTab()
{
    GameView = nullptr;
    MinesweeperBoardWidthSlider = nullptr;
    MinesweeperBoardHeightSlider = nullptr;
    MinesweeperBombsSlider = nullptr;
    StartNewGameButton = nullptr;
    GameWindow = nullptr;
    MinesweeperContainer = nullptr;
    MinesweeperBoard = nullptr;
}


void SMinesweeperPluginTab::Construct(const FArguments& InArgs)
{
    GameView = InArgs._GameView;

    if (!GameView.IsValid())
    {
        return;
    }

    TSharedPtr<class FSlateStyleSet> Style = FMinesweeperPluginStyle::GetPtr();

    if (!Style.IsValid())
    {
        return;
    }

    SDockTab::Construct(
        SDockTab::FArguments().TabRole(NomadTab).OnTabClosed(GameView.Get(), &FMinesweeperPluginGameView::OnTabClosed)
        [
            // Game settings
            SAssignNew(GameWindow, SBox)
            [
                SNew(SVerticalBox) + SVerticalBox::Slot().Padding(10, 10, 10, 10).MaxHeight(25).HAlign(HAlign_Fill).VAlign(VAlign_Fill)
                [
                    SNew(SBox).HAlign(HAlign_Center)
                    [
                        SNew(SHorizontalBox) + SHorizontalBox::Slot().AutoWidth().Padding(FMargin(0, 5, 10, 0))
                        [
                            SNew(STextBlock).Font(Style->GetFontStyle("MinesweeperPluginStyle.UIText")).Text(LOCTEXT("Board Width", " Board Width"))
                        ]
    + SHorizontalBox::Slot().MaxWidth(100)
        [
            SAssignNew(MinesweeperBoardWidthSlider, SSpinBox<uint8>)
            .MinValue(1)
        .MaxValue(this, &SMinesweeperPluginTab::GetMaximumMinesweeperBoardWidth)
        .MinSliderValue(1)
        .MaxSliderValue(this, &SMinesweeperPluginTab::GetMaximumMinesweeperBoardWidth)
        .Value(GameView->GetMinesweeperBoardWidth())
        .MinDesiredWidth(100)
        .OnValueChanged(GameView.Get(), &FMinesweeperPluginGameView::OnMinesweeperBoardWidthSliderValueChanged)
        ]

    + SHorizontalBox::Slot().AutoWidth().Padding(FMargin(25, 5, 10, 0))
        [
            SNew(STextBlock).Font(Style->GetFontStyle("MinesweeperPluginStyle.UIText")).Text(LOCTEXT("Board Height", "Board Height"))
        ]

    + SHorizontalBox::Slot().MaxWidth(100)
        [
            SAssignNew(MinesweeperBoardHeightSlider, SSpinBox<uint8>)
            .MinValue(1)
        .MaxValue(this, &SMinesweeperPluginTab::GetMaximumMinesweeperBoardHeight)
        .MinSliderValue(1)
        .MaxSliderValue(this, &SMinesweeperPluginTab::GetMaximumMinesweeperBoardHeight)
        .Value(GameView->GetMinesweeperBoardHeight())
        .MinDesiredWidth(100)
        .OnValueChanged(GameView.Get(), &FMinesweeperPluginGameView::OnMinesweeperBoardHeightSliderValueChanged)
        ]

    + SHorizontalBox::Slot().AutoWidth().Padding(FMargin(25, 5, 10, 0))
        [
            SNew(STextBlock).Font(Style->GetFontStyle("MinesweeperPluginStyle.UIText")).Text(LOCTEXT("Bombs", "Bombs"))
        ]

    + SHorizontalBox::Slot().MaxWidth(100)
        [
            SAssignNew(MinesweeperBombsSlider, SSpinBox<uint16>)
            .MinValue(1)
        .MaxValue(this, &SMinesweeperPluginTab::GetMaximumBombs)
        .MinSliderValue(1)
        .MaxSliderValue(this, &SMinesweeperPluginTab::GetMaximumBombs)
        .Value(GameView->GetMinesweeperBombs())
        .MinDesiredWidth(100)
        .OnValueChanged(GameView.Get(), &FMinesweeperPluginGameView::OnMinesweeperBombsSliderValueChanged)
        ]
                    ]
                ]

    // Buttons and status
    + SVerticalBox::Slot()
        .Padding(10, 10, 10, 10)
        .MaxHeight(25)
        .HAlign(HAlign_Fill)
        [
            SNew(SBox).HAlign(HAlign_Center)
            [
                SNew(SHorizontalBox) + SHorizontalBox::Slot().AutoWidth().Padding(FMargin(0, 5, 10, 0))
                + SHorizontalBox::Slot().AutoWidth()
        [
            SAssignNew(StartNewGameButton, SButton).HAlign(HAlign_Center).VAlign(VAlign_Center).Text(LOCTEXT("Button", "New Game"))
        ]
    + SHorizontalBox::Slot().AutoWidth().HAlign(HAlign_Right).Padding(FMargin(25, 5, 0, 0))
        [
            SNew(STextBlock).Font(Style->GetFontStyle("MinesweeperPluginStyle.UIText")).Text(LOCTEXT("Game Status:", "Game Status:"))
        ]

    + SHorizontalBox::Slot().AutoWidth().Padding(FMargin(10, 5, 0, 0))
        [
            SNew(STextBlock).Font(Style->GetFontStyle("MinesweeperPluginStyle.UIText")).Text(this, &SMinesweeperPluginTab::GetCurrentGameStatusText)
        ]

    + SHorizontalBox::Slot().AutoWidth().HAlign(HAlign_Right).Padding(FMargin(25, 5, 0, 0))
        [
            SNew(STextBlock).Font(Style->GetFontStyle("MinesweeperPluginStyle.UIText")).Text(LOCTEXT("Cells Remaining:", "Cells Remaining:"))
        ]

    + SHorizontalBox::Slot().AutoWidth().Padding(FMargin(10, 5, 0, 0))
        [
            SNew(STextBlock).Font(Style->GetFontStyle("MinesweeperPluginStyle.UIText")).Text(this, &SMinesweeperPluginTab::GetCurrentCellsRemaining)
        ]
            ]
        ]

    // Minesweeper board
    + SVerticalBox::Slot()
        .Padding(10, 10, 10, 10)
        .HAlign(HAlign_Center).AutoHeight()
        [
            SAssignNew(MinesweeperContainer, SBox).HAlign(HAlign_Left).VAlign(VAlign_Fill)
            [
                SAssignNew(MinesweeperBoard, SCanvas)
            ]
        ]
            ]
        ]
    );

}

TOptional<uint8> SMinesweeperPluginTab::GetMaximumMinesweeperBoardWidth() const
{
    if (!GameWindow.IsValid())
    {
        return 255;
    }

    float Width = GameWindow->GetCachedGeometry().GetLocalSize().X - 100.0f;

    // Invalid check
    if (Width < FMinesweeperPluginGameView::CellSize)
    {
        return 255;
    }
    Width /= FMinesweeperPluginGameView::CellSize;

    if (Width < 1.0f)
    {
        Width = 1.0f;
    }
    else if (Width > 255.0f)
    {
        Width = 255;
    }

    return static_cast<uint8>(Width);
}

TOptional<uint8> SMinesweeperPluginTab::GetMaximumMinesweeperBoardHeight() const
{
    if (!GameWindow.IsValid())
    {
        return 255;
    }

    float Height = GameWindow->GetCachedGeometry().GetLocalSize().Y - 100.0f;

    // Invalid check
    if (Height < FMinesweeperPluginGameView::CellSize)
    {
        return 255;
    }

    Height /= FMinesweeperPluginGameView::CellSize;

    if (Height < 1.0f)
    {
        Height = 1.0f;
    }
    else if (Height > 255.0f)
    {
        Height = 255.0f;
    }

    return static_cast<uint8>(Height);
}

TOptional<uint16> SMinesweeperPluginTab::GetMaximumBombs() const
{
    return (uint16)(MinesweeperBoardWidthSlider->GetValue() * MinesweeperBoardHeightSlider->GetValue()) / 2;
}

FSlateColor SMinesweeperPluginTab::GetBombsSliderBarColour() const
{
    return FSlateColor();
}

FText SMinesweeperPluginTab::GetMinesweeperBombs() const
{
    uint32 Bombs = MinesweeperBombsSlider->GetValue();

    return FText::FromString(FString::FromInt(Bombs));
}

FText SMinesweeperPluginTab::GetCurrentGameStatusText() const
{
    static const FText ErrorReturn = LOCTEXT("Error", "Error");

    FMinesweeperPluginModule* PluginModule = FMinesweeperPluginModule::GetPtr();

    // Valid checks
    if (!PluginModule || !PluginModule->GetGameManager().IsValid())
    {
        return ErrorReturn;
    }

    return PluginModule->GetGameManager()->CurrentGameState();
}

FText SMinesweeperPluginTab::GetCurrentCellsRemaining() const
{
    static const FText ErrorReturn = LOCTEXT("Error", "Error");

    FMinesweeperPluginModule* PluginModule = FMinesweeperPluginModule::GetPtr();

    // Valid checks
    if (!PluginModule || !PluginModule->GetGameData().IsValid() || !PluginModule->GetGameManager().IsValid())
    {
        return ErrorReturn;
    }

    // If not playing, show 0
    if (PluginModule->GetGameManager()->CurrentGameState().ToString() != LOCTEXT("Playing", "Playing").ToString())
    {
        return FText::AsNumber(0);
    }

    // Subtract revealed from board size
    return FText::AsNumber(PluginModule->GetGameData()->GetMinesweeperBoardSize() - PluginModule->GetGameData()->GetNumberOfCellsRevealed());
}

#undef LOCTEXT_NAMESPACE