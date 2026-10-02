/* date = September 25th 2026 8:57 pm */

#ifndef UI_CORE_H
#define UI_CORE_H

// NOTE(ARUN): @panel_struct 

typedef enum Split_Direction Split_Direction;
enum Split_Direction
{
    Split_None,
    Split_Horigantal,
    Split_vertical,
};




typedef struct MM_Panel MM_Panel;
struct MM_Panel
{
    r32 x;
    r32 y;
    r32 width;
    r32 height;
    Color color;
    
};

typedef struct Node_Panel Node_Panel;
struct Node_Panel
{
    MM_Panel *perant;
    bool is_leaf;
    r32 split_ratio;
    Split_Direction split_direction;
    Node_Panel *first;
    Node_Panel *second;
};

// NOTE(ARUN): @button_struct


typedef struct MM_Button MM_Button;
struct MM_Button
{
    r32 x;
    r32 y;
    r32 width;
    r32 height;
    Color color;
};



internal void
mm_panel(MM_Panel *panel, Texture2D icon, Font font)
{
    Rectangle rect = {
        panel->x,
        panel->y,
        panel->width,
        panel->height
    };
    
    DrawRectangleRoundedLinesEx(
                                rect,
                                1.0f,
                                10,
                                10.0f,
                                panel->color
                                );
    
    // Icon
    DrawTextureEx(
                  icon,
                  Vector2{ panel->x + 30, panel->y + 25 },
                  0.0f,
                  0.5f,
                  WHITE
                  );
    
    // Text
    DrawTextEx(
               font,
               "Arun",
               Vector2{ panel->x + 100, panel->y + 25 },
               50.0f,
               1.0f,
               RED
               );
}

// NOTE(ARUN): @button_function


#endif //UI_CORE_H
