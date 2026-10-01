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
    
    Rectangle bounds;
    Rectangle bounds_1;
    
    bool test;
    int test_1;
    
    r32 rander_width;
    r32 rander_height;
};


global_variable MM_State *mm = {0};


/*
    ------------------------------------------------------------
    Panel Tree
    ------------------------------------------------------------
*/

internal void
mm_panel_update(Node_Panel *node)
{
    if (!node)
        return;
    
    MM_Panel *parent = node->perant;
    
    if (!parent)
        return;
    
    /*
        Leaf node:
        Nothing to split.
    */
    if (node->is_leaf)
        return;
    
    
    r32 ratio = node->split_ratio / 100.0f;
    
    if (node->split_direction == Split_Horigantal)
    {
        /*
            Horizontal split:

            +----------------------+
            |        FIRST         |
            +----------------------+
            |       SECOND         |
            +----------------------+
        */
        
        r32 first_height = parent->height * ratio;
        r32 second_height = parent->height - first_height;
        
        if (node->first)
        {
            node->first->perant = parent;
            
            /*
                Your first child
                occupies the top part.
            */
            
            if (node->first->perant)
            {
                mm->rect_r.x = parent->x;
                mm->rect_r.y = parent->y;
                
                mm->rect_r.width  = parent->width;
                mm->rect_r.height = first_height;
            }
        }
        
        if (node->second)
        {
            node->second->perant = parent;
            
            /*
                Second child starts AFTER
                the first child's height.
            */
            
            if (node->second->perant)
            {
                mm->rect_l.x = parent->x;
                mm->rect_l.y = parent->y + first_height;
                
                mm->rect_l.width  = parent->width;
                mm->rect_l.height = second_height;
            }
        }
    }
    else
    {
        /*
            Vertical split:

            +-----------+-----------+
            |           |           |
            |   FIRST   |  SECOND   |
            |           |           |
            +-----------+-----------+
        */
        
        r32 first_width = parent->width * ratio;
        r32 second_width = parent->width - first_width;
        
        if (node->first)
        {
            node->first->perant = parent;
            
            mm->rect_r.x = parent->x;
            mm->rect_r.y = parent->y;
            
            mm->rect_r.width  = first_width;
            mm->rect_r.height = parent->height;
        }
        
        if (node->second)
        {
            node->second->perant = parent;
            
            mm->rect_l.x = parent->x + first_width;
            mm->rect_l.y = parent->y;
            
            mm->rect_l.width  = second_width;
            mm->rect_l.height = parent->height;
        }
    }
}


/*
    ------------------------------------------------------------
    Initialization
    ------------------------------------------------------------
*/

internal void
mm_init()
{
    Arena *arena = arena_alloc(GB(1));
    
    mm = push_array(arena, MM_State, 1);
    
    mm->arena = arena;
    
    mm->test = false;
    mm->test_1 = 0;
    
    
    /*
        Root panel
    */
    
    mm->rect_p.x = 50;
    mm->rect_p.y = 50;
    
    mm->rect_p.width  = 900;
    mm->rect_p.height = 600;
    
    mm->rect_p.color = BLACK;
    
    
    /*
        Child panels
    */
    
    mm->rect_r = mm->rect_p;
    mm->rect_l = mm->rect_p;
    
    mm->rect_r.color = ORANGE;
    mm->rect_l.color = YELLOW;
    
    
    /*
        --------------------------------------------------------
        Node 2
        --------------------------------------------------------
        Left side of root.
    */
    
    mm->panel_2.perant = &mm->rect_r;
    
    mm->panel_2.is_leaf = true;
    
    mm->panel_2.split_ratio = 50.0f;
    
    mm->panel_2.split_direction = Split_vertical;
    
    mm->panel_2.first  = NULL;
    mm->panel_2.second = NULL;
    
    
    /*
        --------------------------------------------------------
        Node 3
        --------------------------------------------------------
        Right side of root.
    */
    
    mm->panel_3.perant = &mm->rect_l;
    
    mm->panel_3.is_leaf = true;
    
    mm->panel_3.split_ratio = 50.0f;
    
    mm->panel_3.split_direction = Split_vertical;
    
    mm->panel_3.first  = NULL;
    mm->panel_3.second = NULL;
    
    
    /*
        --------------------------------------------------------
        Root Node
        --------------------------------------------------------
    */
    
    mm->panel_1.perant = &mm->rect_p;
    
    mm->panel_1.is_leaf = false;
    
    /*
        50 / 50
    */
    
    mm->panel_1.split_ratio = 50.0f;
    
    /*
        Root is split vertically.
    */
    
    mm->panel_1.split_direction = Split_vertical;
    
    mm->panel_1.first  = &mm->panel_2;
    mm->panel_1.second = &mm->panel_3;
}


/*
    ------------------------------------------------------------
    Window Layout
    ------------------------------------------------------------
*/

internal void
mm_update_layout()
{
    /*
        Keep the root panel inside the window.
    */
    
    r32 margin = 2.0f;
    
    mm->rect_p.x = margin;
    mm->rect_p.y = margin;
    
    mm->rect_p.width =
    (r32)GetRenderWidth() - margin * 2.0f;
    
    mm->rect_p.height =
    (r32)GetRenderHeight() - margin * 2.0f;
    
    
    /*
        Root cannot become negative.
    */
    
    if (mm->rect_p.width < 100)
        mm->rect_p.width = 100;
    
    if (mm->rect_p.height < 100)
        mm->rect_p.height = 100;
    
    
    /*
        Calculate children.
    */
    
    r32 ratio =
        mm->panel_1.split_ratio / 100.0f;
    
    
    /*
        LEFT
    */
    
    mm->rect_r.x = mm->rect_p.x;
    mm->rect_r.y = mm->rect_p.y;
    
    mm->rect_r.width =
        mm->rect_p.width * ratio;
    
    mm->rect_r.height =
        mm->rect_p.height;
    
    
    /*
        RIGHT
    */
    
    mm->rect_l.x =
        mm->rect_r.x + mm->rect_r.width;
    
    mm->rect_l.y =
        mm->rect_p.y;
    
    mm->rect_l.width =
        mm->rect_p.width - mm->rect_r.width;
    
    mm->rect_l.height =
        mm->rect_p.height;
}


/*
    ------------------------------------------------------------
    UI
    ------------------------------------------------------------
*/

internal void
mm_draw_left_panel()
{
    Rectangle rect =
    {
        mm->rect_r.x + 5,
        mm->rect_r.y + 5,
        mm->rect_r.width - 10,
        mm->rect_r.height - 10
    };
    
    
    /*
        Panel background
    */
    
    GuiPanel(rect, "Project");
    
    
    /*
        Toolbar
    */
    
    Rectangle button =
    {
        rect.x + 10,
        rect.y + 10,
        100,
        30
    };
    
    GuiButton(button, "New");
    
    
    button.x += 110;
    
    GuiButton(button, "Open");
    
    
    /*
        Label
    */
    
    Rectangle label =
    {
        rect.x + 10,
        rect.y + 55,
        rect.width - 20,
        25
    };
    
    GuiLabel(label, "Project Explorer");
    
    
    /*
        Example controls
    */
    
    Rectangle combo =
    {
        rect.x + 10,
        rect.y + 90,
        rect.width - 20,
        30
    };
    
    GuiComboBox(
                combo,
                "Debug;Release;Profile",
                &mm->test_1
                );
}


internal void
mm_draw_right_panel()
{
    Rectangle rect =
    {
        mm->rect_l.x + 5,
        mm->rect_l.y + 5,
        mm->rect_l.width - 10,
        mm->rect_l.height - 10
    };
    
    
    /*
        Main editor panel
    */
    
    GuiPanel(rect, "Editor");
    
    
    /*
        Editor title
    */
    
    Rectangle title =
    {
        rect.x + 10,
        rect.y + 10,
        rect.width - 20,
        30
    };
    
    GuiLabel(title, "main.cpp");
    
    
    /*
        Editor area
    */
    
    Rectangle editor =
    {
        rect.x + 10,
        rect.y + 50,
        rect.width - 20,
        rect.height - 100
    };
    
    GuiPanel(editor, NULL);
    
    
    /*
        Bottom buttons
    */
    
    Rectangle button =
    {
        rect.x + 10,
        rect.y + rect.height - 40,
        100,
        30
    };
    
    GuiButton(button, "Run");
    
    
    button.x += 110;
    
    GuiButton(button, "Build");
}


/*
    ------------------------------------------------------------
    Main UI
    ------------------------------------------------------------
*/

internal void
mm_draw_ui()
{
    /*
        Root frame
    */
    
    Rectangle root =
    {
        mm->rect_p.x,
        mm->rect_p.y,
        mm->rect_p.width,
        mm->rect_p.height
    };
    
    
    GuiPanel(root, NULL);
    
    
    /*
        Draw children.
    */
    
    mm_draw_left_panel();
    
    mm_draw_right_panel();
    
    
    /*
        Divider
    */
    
    r32 divider_x =
        mm->rect_r.x + mm->rect_r.width;
    
    
    DrawRectangle(
                  (int)divider_x - 2,
                  (int)mm->rect_p.y,
                  4,
                  (int)mm->rect_p.height,
                  DARKGRAY
                  );
}


/*
    ------------------------------------------------------------
    Main
    ------------------------------------------------------------
*/

int
main()
{
    const int WIDTH  = 1200;
    const int HEIGHT = 800;
    
    
    mm_init();
    
    
    SetConfigFlags(
                   FLAG_WINDOW_RESIZABLE
                   );
    
    
    InitWindow(
               WIDTH,
               HEIGHT,
               "Adaptive Binary Panel Tree"
               );
    
    
    SetTargetFPS(60);
    
    
    while (!WindowShouldClose())
    {
        /*
            ----------------------------------------------------
            UPDATE
            ----------------------------------------------------
        */
        
        mm->mouse = GetMousePosition();
        
        mm->rander_width =
        (r32)GetRenderWidth();
        
        mm->rander_height =
        (r32)GetRenderHeight();
        
        
        /*
            Update panel geometry BEFORE drawing.
        */
        
        mm_update_layout();
        
        
        /*
            ----------------------------------------------------
            DRAW
            ----------------------------------------------------
        */
        
        BeginDrawing();
        
        
        ClearBackground(
                        GetColor(0x202020FF)
                        );
        
        
        mm_draw_ui();
        
        
        EndDrawing();
    }
    
    
    CloseWindow();
    
    
    return 0;
}