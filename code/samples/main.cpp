#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOUSER

#define RAYGUI_IMPLEMENTATION

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
    Color color;
    Rectangle bounds;
    Rectangle bounds_1;
    
    
    Texture2D icons;
    Font font;
    
    bool test;
    int test_1;
    
    r32 rander_width;
    r32 rander_height;
};


global_variable MM_State *mm = 0;


internal void
mm_init(void)
{
    Arena *arena = arena_alloc(GB(1));
    
    mm = push_array(arena, MM_State, 1);
    
    mm->arena = arena;
    
    mm->rect_p.x = 16;
    mm->rect_p.y = 22;
    mm->rect_p.width = 160;
    mm->rect_p.height = 50;
    mm->rect_p.color = BACK_COLOR;;
    
    mm->rect_r.x = 20;
    mm->rect_r.y = 20;
    mm->rect_r.width = 400;
    mm->rect_r.height = 200;
    mm->rect_r.color = RAYWHITE;
    
    
    
    mm->icons = LoadTexture("assets/icons/test.png");
    mm->font =  LoadFont("assets/fonts/Inter_28pt-ExtraBoldItalic.ttf");
    
    
    
}


int
main(void)
{
    const int WIDTH  = 1200;
    const int HEIGHT = 800;
    
    
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    
    InitWindow(WIDTH,HEIGHT,"Adaptive Binary Panel Tree");
    
    SetTargetFPS(60);
    
    mm_init();
    
    
    
    while (!WindowShouldClose())
    {
        mm->mouse = GetMousePosition();
        
        BeginDrawing();
        
        ClearBackground(BACK_COLOR);
        
        DrawRectangle(0,0,200,100,MY_COLOR);
        
        mm_panel(&mm->rect_p,mm->icons,mm->font);
        
        EndDrawing();
    }
    
    CloseWindow();
    
    return 0;
}