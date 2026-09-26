/* date = September 25th 2026 8:57 pm */

#ifndef UI_CORE_H
#define UI_CORE_H


typedef enum Split_Direction Split_Direction;
enum Split_Direction
{
    
    Split_None,
    Split_Horigantal,
    Split_vertical,
    
};

typedef struct Panel_Rectangle Panel_Rectangle;
struct Panel_Rectangle
{
    
    r32 x;
    r32 y;
    r32 width;
    r32 height;
    
    
};



typedef struct Node_Panel Node_Panel;
struct Node_Panel
{
    
    Panel_Rectangle rect;
    bool is_leaf;
    r32 split_ratio;
    Split_Direction split_direction;
    Node_Panel *first;
    Node_Panel *Second;
};


internal void
shape_rectangle(Panel_Rectangle *rect,Color color)
{
    
    DrawRectangle(rect->x,rect->y,rect->width,rect->height,color);
    
}


#endif //UI_CORE_H
