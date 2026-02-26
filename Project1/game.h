#pragma once
#include "grid.h"
#include "blocks.cpp"

class Game
{
public:
    Game();
    ~Game();
    Block GetRandomBlock();
    std::vector<Block> GetAllBlocks();
    void Draw();
    void HandleInput();
    void MoveBlockLeft();
    void MoveBlockRight();
    void MoveBlockDown();
    Grid grid;
    bool GameOver;
    int score;
    Music music;

private:
    bool IsBlockOutside();
    void RotateBlock();
    void LockBlock();
    void Reset();
    bool BlockFits();
    void UpdateScore(int linesCleared, int moveDownPoints);
    std::vector<Block> blocks;
    Block currentBlock;
    Block nextBlocks;
    Sound clearSound;
    Sound rotateSound;
    Sound gameOver;
};