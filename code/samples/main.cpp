
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
    MM_Panel rect_p;
    MM_Panel rect_r;
    MM_Panel rect_l;
    Node_Panel panel_1;
    Node_Panel panel_2;
    Node_Panel panel_3;
    Vector2 mouse;
    
    r32 rander_width;
    r32 rander_height;
};


global_variable MM_State *mm = {0};


internal void
mm_init()
{
    
    Arena *arena = arena_alloc(GB(1));
    mm = push_array(arena,MM_State,1);
    mm->arena = arena;
    
    
    mm->rect_p.x = 2.5;
    mm->rect_p.y = 2.5;
    mm->rect_p.width= 400;
    mm->rect_p.height= 400;
    mm->rect_p.color= BLACK;
    
    mm->rect_r.x = mm->rect_p.x;
    mm->rect_r.y = mm->rect_p.y;
    mm->rect_r.width = mm->rect_p.width/2;
    mm->rect_r.height= mm->rect_p.height;
    mm->rect_r.color= ORANGE;
    
    mm->rect_l.x = mm->rect_r.width;
    mm->rect_l.y = mm->rect_p.y;
    mm->rect_l.width= mm->rect_p.width/2;
    mm->rect_l.height= mm->rect_p.height;
    mm->rect_l.color= YELLOW;
    
    mm->panel_2.perant = &mm->rect_r;
    mm->panel_2.is_leaf = true;
    mm->panel_2.split_ratio = 50.0;
    mm->panel_2.split_direction = Split_vertical;
    mm->panel_2.first = NULL;
    mm->panel_2.second = NULL;
    
    mm->panel_3.perant = &mm->rect_l;
    mm->panel_3.is_leaf = true;
    mm->panel_3.split_ratio = 50.0;
    mm->panel_3.split_direction = Split_vertical;
    mm->panel_3.first = NULL;
    mm->panel_3.second = NULL;
    
    mm->panel_1.perant = &mm->rect_p;
    mm->panel_1.is_leaf = false;
    mm->panel_1.split_ratio = 50.0;
    mm->panel_1.split_direction = Split_Horigantal;
    mm->panel_1.first = &mm->panel_2;
    mm->panel_1.second = &mm->panel_3;
    
}

int main()
{
    
    const int WIDTH = 1200;
    const int HEIGHT = 800;
    
    
    mm_init();
    
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_UNDECORATED);
    InitWindow(WIDTH, HEIGHT, "Adaptive Binary Panel Tree");
    
    
    SetTargetFPS(60);
    
    
    
    while (!WindowShouldClose())
    {
        BeginDrawing();
        
        
        ClearBackground(BLUE);
        
        
        
        
        mm->rander_width = GetRenderWidth();
        mm->rander_height = GetRenderHeight();
        
        
        mm->rect_p.width = GetScreenWidth() - 5;
        mm->rect_p.height = GetScreenHeight() - 5;
        
        mm->mouse = GetMousePosition();
        
        
        mm->rect_r.x = mm->rect_p.x;
        mm->rect_r.y = mm->rect_p.y;
        mm->rect_r.width = (mm->rect_p.width + 2.5 ) /2;
        mm->rect_r.height= mm->rect_p.height;
        
        mm->rect_l.x = mm->rect_r.width;
        mm->rect_l.y = mm->rect_p.y;
        mm->rect_l.width= mm->rect_p.width/2;
        mm->rect_l.height= mm->rect_p.height;
        
        
        
        mm_defualt_window(&mm->panel_1);
        mm_defualt_window(&mm->panel_2);
        mm_defualt_window(&mm->panel_3);
        
        DrawRectangle(0, 0, GetScreenWidth(), 40, DARKGRAY);
        
        DrawText("My App", 15, 10, 20, WHITE);
        
        DrawText("-", 1100, 10, 20, WHITE);
        DrawText("□", 1130, 10, 20, WHITE);
        DrawText("X", 1160, 10, 20, WHITE);
        
        
        DrawText(TextFormat("X:%.0fY:%.0f",mm->mouse.x,mm->mouse.y),50,50,18,BLACK);
        
        EndDrawing();
    }
    
    CloseWindow();
    
    
    return 0;
}


