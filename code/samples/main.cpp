
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#if defined(_WIN32)
#define NOGDI
#define NOUSER
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#if defined(_WIN32)
#undef PlaySound
#endif`

#include "googol_tech_init.h"

#include "raylib_inc.h"




typedef enum Split_Direction Split_Direction;
enum Split_Direction
{
    Split_None,
    Split_vertical,
    Split_Horizontal
};

typedef struct Node_Panel Node_Panel;
struct Node_Panel
{
    
    Rectangles 
        
        
};



int main(void){
    
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    //ToggleBorderlessWindowed();
    ToggleBorderl
        
        SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        
        
        BeginDrawing();
        
        ClearBackground(BLACK);
        
        
        DrawRectangle(0,0,screenWidth/2,screenHeight,YELLOW);
        DrawRectangle(screenWidth/2,0,screenWidth/2,screenHeight,GREEN);
        
        
        
        //DrawLine(100,100,100,400,BLACK);
        //DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
        
        EndDrawing();
        
    }
    
    
    CloseWindow();        // Close window and OpenGL context
    
    return 0;
}