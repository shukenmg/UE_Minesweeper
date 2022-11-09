// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MinesweeperPluginGameData.h"

#include "Widgets/SOverlay.h"

class SBox;
class SImage;
class STextBlock;
class SColorBlock;

class MINESWEEPERPLUGIN_API SMinesweeperPluginBoardCell : public SOverlay
{

public:

    // Setup slate args
    SLATE_BEGIN_ARGS(SMinesweeperPluginBoardCell)
        : _GameData(nullptr)
        , _CellData(FCellData::DefaultValue())
    { }

    SLATE_ARGUMENT(TSharedPtr<FMinesweeperPluginGameData>, GameData)
        SLATE_ARGUMENT(FCellData, CellData)

        SLATE_END_ARGS()

public:

    SMinesweeperPluginBoardCell();

    void Construct(const FArguments& InArgs);

    const struct FCellData& GetCellData() const { return CellData; };

    void Update() const;

protected:

    TSharedPtr<FMinesweeperPluginGameData> GameData;

    FCellData CellData;

    TSharedPtr<SImage> Button;

    const TArray<FName> NeighborBombBrushes = {
        "MinesweeperPluginStyle.Cell.Neighbor.0",
        "MinesweeperPluginStyle.Cell.Neighbor.1",
        "MinesweeperPluginStyle.Cell.Neighbor.2",
        "MinesweeperPluginStyle.Cell.Neighbor.3",
        "MinesweeperPluginStyle.Cell.Neighbor.4",
        "MinesweeperPluginStyle.Cell.Neighbor.5",
        "MinesweeperPluginStyle.Cell.Neighbor.6",
        "MinesweeperPluginStyle.Cell.Neighbor.7",
        "MinesweeperPluginStyle.Cell.Neighbor.8",
    };
};
