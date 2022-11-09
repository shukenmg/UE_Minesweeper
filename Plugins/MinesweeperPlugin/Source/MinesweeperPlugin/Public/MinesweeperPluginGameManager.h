// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

enum EGameStates
{
    NotPlaying,
    Playing,
    Dead,
    Win,
    Disabled
};

class MINESWEEPERPLUGIN_API FMinesweeperPluginGameManager
{

public:

    // Game functions
    void OnToolbarButtonPressed();

    void Init();
    void StartNewGame(uint8 Width, uint8 Height, uint16 Bombs);
    void DistributeBombs(TArray<bool>& Cells, uint16 Bombs);

    // Player actions
    void UncoverCell(const FCellData& Cell);
    void UncoverCellRecursive(const FCellData& Cell);
    void FlagCell(const FCellData& Cell);

    // Game State
    FText CurrentGameState();

protected:

    void EndGameplay(EGameStates EndGameState);
    void DisableGameplay();

    EGameStates GameState;
};
