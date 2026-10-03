/* date = September 25th 2026 8:57 pm */

#ifndef UI_CORE_H
#define UI_CORE_H

// NOTE(ARUN): @panel_struct 



#define BACK_COLOR   CLITERAL(Color){ 216, 214, 217, 255 }   // My own White (raylib logo)
#define MY_COLOR   CLITERAL(Color){ 97, 1, 238, 255 }   // My own White (raylib logo)


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
mm_lable(MM_Panel *panel,Font font)
{
    
    Rectangle rect = {
        panel->x,
        panel->y,
        panel->width,
        panel->height
    };
    
    
    DrawRectangleRounded(
                         rect,
                         30.0f,
                         0,
                         RED
                         );
    
#if 0
    DrawRectangleRoundedLinesEx(
                                rect,
                                30.0f,
                                0,
                                3.0f,
                                RED
                                );
#endif
    
    
    
    
    // Text
    DrawTextEx(
               font,
               "ADD",
               Vector2{69,28},
               40.0f,
               1.0f,
               RED
               );
    
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
                                30.0f,
                                0,
                                3.0f,
                                RED
                                );
    
    
    // Icon
    DrawTextureEx(
                  icon,
                  Vector2{31,31},
                  0.0f,
                  0.4f,
                  RED
                  );
    
    // Text
    DrawTextEx(
               font,
               "ADD",
               Vector2{69,28},
               40.0f,
               1.0f,
               RED
               );
}

internal void
mm_button(MM_Panel *panel, Texture2D icon, Font font, const char *text)
{
    // ------------------------------------------------------------
    // Layout constants
    // ------------------------------------------------------------
    
    const r32 icon_height_ratio = 0.50f;
    const r32 text_height_ratio = 0.55f;
    const r32 gap_ratio         = 0.04f;
    const r32 horizontal_pad    = 0.10f;
    
    // ------------------------------------------------------------
    // Button rectangle
    // ------------------------------------------------------------
    
    Rectangle rect = {
        panel->x,
        panel->y,
        panel->width,
        panel->height
    };
    
    // ------------------------------------------------------------
    // Border
    // ------------------------------------------------------------
    
    DrawRectangleRoundedLinesEx(
                                rect,
                                0.30f * panel->height,
                                0,
                                3.0f,
                                panel->color
                                );
    
    // ------------------------------------------------------------
    // Calculate text size
    // ------------------------------------------------------------
    
    r32 text_size = panel->height * text_height_ratio;
    
    Vector2 text_measure = MeasureTextEx(
                                         font,
                                         text,
                                         text_size,
                                         1.0f
                                         );
    
    // ------------------------------------------------------------
    // Calculate icon size
    //
    // Preserve original PNG aspect ratio.
    // ------------------------------------------------------------
    
    r32 icon_height = panel->height * icon_height_ratio;
    
    r32 icon_aspect = 1.0f;
    
    if (icon.height > 0)
    {
        icon_aspect =
        (r32)icon.width /
        (r32)icon.height;
    }
    
    r32 icon_width = icon_height * icon_aspect;
    
    // ------------------------------------------------------------
    // Gap between icon and text
    // ------------------------------------------------------------
    
    r32 gap = panel->width * gap_ratio;
    
    // ------------------------------------------------------------
    // Total content width
    // ------------------------------------------------------------
    
    r32 content_width =
        icon_width +
        gap +
        text_measure.x;
    
    // ------------------------------------------------------------
    // Available width
    // ------------------------------------------------------------
    
    r32 available_width =
        panel->width *
    (1.0f - horizontal_pad * 2.0f);
    
    // ------------------------------------------------------------
    // Scale everything down if necessary
    // ------------------------------------------------------------
    
    if (content_width > available_width)
    {
        r32 scale =
            available_width /
            content_width;
        
        icon_width  *= scale;
        icon_height *= scale;
        
        text_size *= scale;
        
        gap *= scale;
        
        text_measure = MeasureTextEx(
                                     font,
                                     text,
                                     text_size,
                                     1.0f
                                     );
        
        content_width =
            icon_width +
            gap +
            text_measure.x;
    }
    
    // ------------------------------------------------------------
    // Center complete group horizontally
    // ------------------------------------------------------------
    
    r32 start_x =
        panel->x +
    (panel->width - content_width) * 0.5f;
    
    // ------------------------------------------------------------
    // Center icon vertically
    // ------------------------------------------------------------
    
    r32 icon_y =
        panel->y +
    (panel->height - icon_height) * 0.5f;
    
    // ------------------------------------------------------------
    // Center text vertically
    // ------------------------------------------------------------
    
    r32 text_y =
        panel->y +
    (panel->height - text_measure.y) * 0.5f;
    
    // ------------------------------------------------------------
    // Draw icon
    //
    // IMPORTANT:
    //
    // The source PNG should be WHITE with transparency.
    // panel->color is then used as the tint.
    // ------------------------------------------------------------
    
    if (icon.id != 0 && icon.width > 0 && icon.height > 0)
    {
        Rectangle source = {
            0.0f,
            0.0f,
            (r32)icon.width,
            (r32)icon.height
        };
        
        Rectangle destination = {
            start_x,
            icon_y,
            icon_width,
            icon_height
        };
        
        DrawTexturePro(
                       icon,
                       source,
                       destination,
                       Vector2{0.0f, 0.0f},
                       0.0f,
                       panel->color
                       );
    }
    
    // ------------------------------------------------------------
    // Draw text
    // ------------------------------------------------------------
    
    DrawTextEx(
               font,
               text,
               Vector2{
                   start_x + icon_width + gap,
                   text_y
               },
               text_size,
               1.0f,
               panel->color
               );
}


// NOTE(ARUN): @button_function





#endif //UI_CORE_H
