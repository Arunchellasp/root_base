
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

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
#include "base/base_inc.h"
#include "ui_core/ui_core_inc.h"



typedef struct MM_State MM_State;
struct MM_State
{
    Arena *arena;
    Panel_Rectangle rect;
    
    
};


global_variable MM_State *mm = {0};


internal void
mm_init()
{
    
    Arena *arena = arena_alloc(GB(1));
    mm = push_array(arena,MM_State,1);
    mm->arena = arena;
    
    
    mm->rect.x = 400;
    mm->rect.y = 400;
    mm->rect.width= 400;
    mm->rect.height= 400;
    
    
}

int main()
{
    
    const int WIDTH = 1200;
    const int HEIGHT = 800;
    
    
    mm_init();
    
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WIDTH, HEIGHT, "Adaptive Binary Panel Tree");
    
    
    SetTargetFPS(60);
    
    
    
    while (!WindowShouldClose())
    {
        BeginDrawing();
        
        ClearBackground(BLUE);
        
        draw_rectangle(&mm->rect,GREEN);
        
        
        
        EndDrawing();
    }
    
    CloseWindow();
    
    
    return 0;
}


