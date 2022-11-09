// Fill out your copyright notice in the Description page of Project Settings.


#include "MinesweeperPluginBoardCell.h"
#include "MinesweeperPluginGameData.h"
#include "MinesweeperPluginGameView.h"
#include "MinesweeperPluginStyle.h"

#include "Styling/SlateStyle.h"
#include "Widgets/Images/SImage.h"

SMinesweeperPluginBoardCell::SMinesweeperPluginBoardCell()
{
    GameData = nullptr;
}

void SMinesweeperPluginBoardCell::Construct(const FArguments& InArgs)
{
    // Get data passed from board creation
    GameData = InArgs._GameData;
    CellData = InArgs._CellData;

    TSharedPtr<FSlateStyleSet> Style = FMinesweeperPluginStyle::GetPtr();

    if (!Style.IsValid())
    {
        return;
    }

    Button = SNew(SImage).Image(Style->GetBrush("MinesweeperPluginStyle.Cell.Covered"));

    AddSlot(0)[Button.ToSharedRef()];
}

void SMinesweeperPluginBoardCell::Update() const
{
    if (!GameData.IsValid())
    {
        return;
    }

    TSharedPtr<FSlateStyleSet> Style = FMinesweeperPluginStyle::GetPtr();

    if (!Style.IsValid())
    {
        return;
    }

    const FCellData& NewCellData = GameData->GetCellData(CellData.Position);

    switch (NewCellData.State)
    {
    case ECellStates::Covered:
        Button->SetImage(Style->GetBrush("MinesweeperPluginStyle.Cell.Covered"));
        break;

    case ECellStates::Uncovered:
        // If has bomb show bomb image if not show Neighboring Bombs
        if (NewCellData.HasBomb)
        {
            Button->SetImage(Style->GetBrush("MinesweeperPluginStyle.Cell.Bomb"));
            break;
        }

        Button->SetImage(Style->GetBrush(NeighborBombBrushes[NewCellData.NeighboringBombs]));
        break;

    case ECellStates::Flagged:
        Button->SetImage(Style->GetBrush("MinesweeperPluginStyle.Cell.Flagged"));
        break;

    case ECellStates::Exploded:
        Button->SetImage(Style->GetBrush("MinesweeperPluginStyle.Cell.Exploded"));
        break;
    }
}
