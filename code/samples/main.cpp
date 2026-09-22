
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
#endif

#include "googol_tech_init.h"

#include "raylib_inc.h"


int main(void){
    
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------
        
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        
        ClearBackground(RAYWHITE);
        
        DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
        
        EndDrawing();
        //----------------------------------------------------------------------------------
    }
    
    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    
    
    return 0;
}