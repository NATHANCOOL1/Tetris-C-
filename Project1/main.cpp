#include <raylib.h>
#include "game.h"
#include "colors.h"
#include <iostream>

double lastUpdateTime = 0;

bool EventTriggered(double interval)
{
    double currentTime = GetTime();
    if(currentTime - lastUpdateTime >= interval)
    {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

int main()
{
    Color Main_Color = { 0,255,41,1 };
    InitWindow(500, 620, "Tetris C++");
    SetTargetFPS(60);

    Font font = LoadFontEx("Font/Font.ttf", 64, 0,0);
    Image image = LoadImage("Image/image.png");     
    Texture2D texture = LoadTextureFromImage(image);  
    UnloadImage(image);

    Game game = Game();
    
    while (WindowShouldClose() == false)
    {
        UpdateMusicStream(game.music);
        game.HandleInput();
        if(EventTriggered(0.5))
        {
            game.MoveBlockDown();
        }
        BeginDrawing();
        DrawTexture(texture, 500 -620,620-620, WHITE);
        ClearBackground(Main_Color);
        DrawTextEx(font, "Score", {365,15}, 38,2, WHITE);
        DrawTextEx(font, "Next", {370,175}, 38,2, WHITE);
        if(game.GameOver){
        DrawTextEx(font, "GAME OVER", {320,450}, 25,2, WHITE);
        }
        DrawRectangleRounded({320,55,170,60},0.3,6,lightBlue);

        char scoreText[10];
        sprintf_s(scoreText, "%d", game.score);
        Vector2 textSize = MeasureTextEx(font, scoreText,38,2);

        DrawTextEx(font, scoreText, {320 + (170 - textSize.x)/2,65}, 38,2, WHITE);
        DrawRectangleRounded({320,215,170,180},0.3,6,lightBlue);
        game.Draw();
        EndDrawing();
    }
    UnloadTexture(texture);
    CloseWindow();
}