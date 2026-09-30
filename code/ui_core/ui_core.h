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

// NOTE(ARUN): @panel_function
internal void
mm_panel(MM_Panel *panel)
{
    DrawRectangle(panel->x,panel->y,panel->width,panel->height,panel->color);
}



// NOTE(ARUN): @button_function

internal void
mm_button(MM_Button *button)
{
    
    DrawRectangle(button->x,button->y,button->width,button->height,button->color);
    
}


internal void
mm_defualt_window(Node_Panel *node_panel)
{
    
    mm_panel(node_panel->perant);
    
    
}

#endif //UI_CORE_H
