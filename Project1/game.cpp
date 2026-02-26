#include "game.h"
#include <random>

Game::Game()
{
    grid = Grid();
    blocks = GetAllBlocks();
    currentBlock = GetRandomBlock();
    nextBlocks = GetRandomBlock();
    GameOver = false;
    score = 0;
    InitAudioDevice();
    music = LoadMusicStream("Sounds/music.mp3");
    PlayMusicStream(music);
    rotateSound = LoadSound("Sounds/rotate.mp3");
    clearSound = LoadSound("Sounds/clear.mp3");
    gameOver = LoadSound("Sounds/game-over.mp3");
}

Game::~Game()
{
    UnloadSound(rotateSound);
    UnloadSound(clearSound);
    UnloadSound(gameOver);
    UnloadMusicStream(music);
    CloseAudioDevice();
}


Block Game::GetRandomBlock()
{
    if(blocks.empty())
    {
        blocks = GetAllBlocks();
    }
    int randomIndex = rand() % blocks.size();
    Block block = blocks[randomIndex];
    blocks.erase(blocks.begin() + randomIndex);
    return block;
}

std::vector<Block> Game::GetAllBlocks()
{
    return {IBlock(), JBlock(), LBlock(), OBlock(), SBlock(), TBlock(), ZBlock(),};
}

void Game::Draw()
{
    grid.Draw();
    currentBlock.Draw(11, 11);
    switch(nextBlocks.id)
    {
        case 3:
            nextBlocks.Draw(255,290);
            break;
        case 4:
            nextBlocks.Draw(255,280);
            break;
        default:
            nextBlocks.Draw(270,270);
            break;
    }
}

void Game::HandleInput()
{
    int keyPressed = GetKeyPressed();
    if(GameOver && keyPressed != 0)
    {
        GameOver = false;
        PlayMusicStream(music);
        Reset();
    }
    switch (keyPressed)
    {
        case KEY_Q:
        MoveBlockLeft();
        break;
        case KEY_Z:
        RotateBlock();
        break;
        case KEY_A:
        MoveBlockLeft();
        break;
        case KEY_D:
        MoveBlockRight();
        break;
        case KEY_S:
        MoveBlockDown();
        UpdateScore(0,1);
        break;
        case KEY_W:
        RotateBlock();
        break;
        case KEY_SPACE:
        RotateBlock();
        break;
        case KEY_LEFT:
        MoveBlockLeft();
        break;
        case KEY_RIGHT:
        MoveBlockRight();
        break;
        case KEY_DOWN:
        MoveBlockDown();
        UpdateScore(0,1);
        break;
        case KEY_UP:
        RotateBlock();
        break;

    }
}

void Game::MoveBlockLeft()
{
    if(!GameOver)
    {
        currentBlock.Move(0,-1);
        if(IsBlockOutside() || BlockFits() == false)
        {
        currentBlock.Move(0,1);
        }
    }
}

void Game::MoveBlockRight()
{
    if(!GameOver){
        currentBlock.Move(0,1);
        if(IsBlockOutside() || BlockFits() == false)
        {
            currentBlock.Move(0,-1);
        }
    }
}

void Game::MoveBlockDown()
{
    if(!GameOver){
        currentBlock.Move(1,0);
        if(IsBlockOutside() || BlockFits() == false)
        {
            currentBlock.Move(-1,0);
            LockBlock();
        }
    }
}

bool Game:: IsBlockOutside()
{
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for(Position item: tiles)
    {
        if(grid.IsCellOutside(item.row, item.column))
        {
            return true;
        }
    }
    return false;
}

void Game::RotateBlock()
{
    if(!GameOver)
    {
        currentBlock.Rotate();
        if(IsBlockOutside() || BlockFits() == false)
        {
            currentBlock.UndoRotation();
        }
        else
        {
            PlaySound(rotateSound);
        }
    }
}

void Game::LockBlock()
{
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for(Position item: tiles)
    {
        grid.grid[item.row][item.column] = currentBlock.id;
    }
    currentBlock = nextBlocks;
    if(BlockFits() == false)
    {
        GameOver = true;
        StopMusicStream(music);
        PlaySound(gameOver);
    }
    nextBlocks = GetRandomBlock();
    int rowsCleared = grid.ClearFullRows();
    if(rowsCleared > 0)
    {
        PlaySound(clearSound);
        UpdateScore(rowsCleared, 0);
    }
    
}

bool Game::BlockFits()
{
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for(Position item: tiles)
    {
        if(grid.IsCellEmpty(item.row,item.column) == false)
        {
            return false;
        }
    }
    return true;
}

void Game::Reset()
{
    grid.Initialize();
    blocks = GetAllBlocks();
    currentBlock = GetRandomBlock();
    nextBlocks = GetRandomBlock();
    score = 0;
}

void Game::UpdateScore(int linesCleared, int moveDownPoints)
{
    switch(linesCleared)
    {
        case 1:
            score += 100;
            break;
        case 2:
            score += 300;
            break;
        case 3:
            score += 500;
            break;
        default:
            break;
    }
    score += moveDownPoints;
}