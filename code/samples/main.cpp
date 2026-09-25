
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

#if 0


int main()
{
    const int WIDTH = 1200;
    const int HEIGHT = 800;
    
    
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WIDTH, HEIGHT, "Adaptive Binary Panel Tree");
    
    
    SetTargetFPS(60);
    
    
    
    
    // --------------------------------------------------------
    // Main loop
    // --------------------------------------------------------
    
    while (!WindowShouldClose())
    {
        BeginDrawing();
        
        ClearBackground({20, 20, 24, 255});
        
        
        DrawRectangle(0,0,200,200,GREEN);
        DrawRectangle(0,200,200,200,GREEN);
        
        EndDrawing();
    }
    
    CloseWindow();
    
    
    return 0;
}




#define MIN_PANEL_SIZE 80.0f
#define SPLITTER_SIZE 6.0f
#define PLUS_SIZE 24.0f

typedef enum
{
    SPLIT_NONE,
    SPLIT_VERTICAL,
    SPLIT_HORIZONTAL
} SplitDirection;


typedef struct PanelNode
{
    Rectangle rect;
    
    bool isLeaf;
    
    int id;
    
    // Used when this node is split.
    float splitRatio;
    
    SplitDirection direction;
    
    struct PanelNode *first;
    struct PanelNode *second;
    
} PanelNode;


static int nextPanelId = 1;


// ============================================================
// Create leaf
// ============================================================

PanelNode *CreatePanel(Rectangle rect)
{
    PanelNode *node = malloc(sizeof(PanelNode));
    
    if (!node)
        return NULL;
    
    node->rect = rect;
    
    node->isLeaf = true;
    
    node->id = nextPanelId++;
    
    node->splitRatio = 0.5f;
    
    node->direction = SPLIT_NONE;
    
    node->first = NULL;
    node->second = NULL;
    
    return node;
}


// ============================================================
// Destroy tree
// ============================================================

void DestroyPanelTree(PanelNode *node)
{
    if (!node)
        return;
    
    DestroyPanelTree(node->first);
    DestroyPanelTree(node->second);
    
    free(node);
}


// ============================================================
// Calculate child rectangles
// ============================================================

void LayoutPanelTree(PanelNode *node)
{
    if (!node || node->isLeaf)
        return;
    
    float ratio = node->splitRatio;
    
    if (ratio < 0.05f)
        ratio = 0.05f;
    
    if (ratio > 0.95f)
        ratio = 0.95f;
    
    node->splitRatio = ratio;
    
    
    if (node->direction == SPLIT_VERTICAL)
    {
        float availableWidth =
            node->rect.width - SPLITTER_SIZE;
        
        float firstWidth =
            availableWidth * ratio;
        
        float secondWidth =
            availableWidth - firstWidth;
        
        
        node->first->rect = (Rectangle)
        {
            node->rect.x,
            node->rect.y,
            firstWidth,
            node->rect.height
        };
        
        
        node->second->rect = (Rectangle)
        {
            node->rect.x + firstWidth + SPLITTER_SIZE,
            node->rect.y,
            secondWidth,
            node->rect.height
        };
    }
    else
    {
        float availableHeight =
            node->rect.height - SPLITTER_SIZE;
        
        float firstHeight =
            availableHeight * ratio;
        
        float secondHeight =
            availableHeight - firstHeight;
        
        
        node->first->rect = (Rectangle)
        {
            node->rect.x,
            node->rect.y,
            node->rect.width,
            firstHeight
        };
        
        
        node->second->rect = (Rectangle)
        {
            node->rect.x,
            node->rect.y + firstHeight + SPLITTER_SIZE,
            node->rect.width,
            secondHeight
        };
    }
    
    
    LayoutPanelTree(node->first);
    LayoutPanelTree(node->second);
}


// ============================================================
// Can this panel be split?
// ============================================================

bool CanSplit(PanelNode *node, SplitDirection direction)
{
    if (!node || !node->isLeaf)
        return false;
    
    if (direction == SPLIT_VERTICAL)
    {
        return node->rect.width >= MIN_PANEL_SIZE * 2
            + SPLITTER_SIZE;
    }
    
    return node->rect.height >= MIN_PANEL_SIZE * 2
        + SPLITTER_SIZE;
}


// ============================================================
// Split panel
// ============================================================

bool SplitPanel(
                PanelNode *node,
                SplitDirection direction
                )
{
    if (!CanSplit(node, direction))
        return false;
    
    
    Rectangle original = node->rect;
    
    
    node->first = CreatePanel(original);
    node->second = CreatePanel(original);
    
    if (!node->first || !node->second)
        return false;
    
    
    node->isLeaf = false;
    
    node->direction = direction;
    
    node->splitRatio = 0.5f;
    
    
    LayoutPanelTree(node);
    
    return true;
}


// ============================================================
// Get splitter rectangle
// ============================================================

Rectangle GetSplitterRect(PanelNode *node)
{
    if (node->direction == SPLIT_VERTICAL)
    {
        float x =
            node->first->rect.x +
            node->first->rect.width;
        
        return (Rectangle)
        {
            x,
            node->rect.y,
            SPLITTER_SIZE,
            node->rect.height
        };
    }
    
    
    float y =
        node->first->rect.y +
        node->first->rect.height;
    
    return (Rectangle)
    {
        node->rect.x,
        y,
        node->rect.width,
        SPLITTER_SIZE
    };
}


// ============================================================
// Update splitter
// ============================================================

void UpdateSplitter(PanelNode *node)
{
    if (!node || node->isLeaf)
        return;
    
    
    Rectangle splitter =
        GetSplitterRect(node);
    
    
    Vector2 mouse =
        GetMousePosition();
    
    
    if (CheckCollisionPointRec(mouse, splitter))
    {
        if (node->direction == SPLIT_VERTICAL)
            SetMouseCursor(MOUSE_CURSOR_RESIZE_EW);
        else
            SetMouseCursor(MOUSE_CURSOR_RESIZE_NS);
    }
    
    
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) &&
        CheckCollisionPointRec(mouse, splitter))
    {
        if (node->direction == SPLIT_VERTICAL)
        {
            float relative =
                mouse.x - node->rect.x;
            
            float available =
                node->rect.width -
                SPLITTER_SIZE;
            
            node->splitRatio =
                relative / available;
        }
        else
        {
            float relative =
                mouse.y - node->rect.y;
            
            float available =
                node->rect.height -
                SPLITTER_SIZE;
            
            node->splitRatio =
                relative / available;
        }
        
        
        // Enforce minimum size.
        
        float minRatio;
        
        if (node->direction == SPLIT_VERTICAL)
        {
            minRatio =
                MIN_PANEL_SIZE /
            (node->rect.width - SPLITTER_SIZE);
        }
        else
        {
            minRatio =
                MIN_PANEL_SIZE /
            (node->rect.height - SPLITTER_SIZE);
        }
        
        
        if (node->splitRatio < minRatio)
            node->splitRatio = minRatio;
        
        if (node->splitRatio > 1.0f - minRatio)
            node->splitRatio = 1.0f - minRatio;
        
        
        LayoutPanelTree(node);
    }
    
    
    UpdateSplitter(node->first);
    UpdateSplitter(node->second);
}


// ============================================================
// Draw plus button
// ============================================================

bool DrawPlusButton(Rectangle panel)
{
    Rectangle button =
    {
        panel.x + panel.width - PLUS_SIZE - 6,
        panel.y + 6,
        PLUS_SIZE,
        PLUS_SIZE
    };
    
    
    Vector2 mouse =
        GetMousePosition();
    
    
    bool hover =
        CheckCollisionPointRec(mouse, button);
    
    
    DrawRectangleRec(
                     button,
                     hover ? DARKGRAY : GRAY
                     );
    
    
    DrawLine(
             button.x + 6,
             button.y + PLUS_SIZE / 2,
             button.x + PLUS_SIZE - 6,
             button.y + PLUS_SIZE / 2,
             WHITE
             );
    
    
    DrawLine(
             button.x + PLUS_SIZE / 2,
             button.y + 6,
             button.x + PLUS_SIZE / 2,
             button.y + PLUS_SIZE - 6,
             WHITE
             );
    
    
    return hover &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}


// ============================================================
// Update leaf panels
// ============================================================

void UpdatePanels(PanelNode *node)
{
    if (!node)
        return;
    
    
    if (!node->isLeaf)
    {
        UpdatePanels(node->first);
        UpdatePanels(node->second);
        return;
    }
    
    
    if (DrawPlusButton(node->rect))
    {
        /*
         * Automatically choose the direction
         * based on the largest dimension.
         */
        
        SplitDirection direction;
        
        if (node->rect.width >= node->rect.height)
            direction = SPLIT_VERTICAL;
        else
            direction = SPLIT_HORIZONTAL;
        
        
        SplitPanel(node, direction);
    }
}


// ============================================================
// Draw tree
// ============================================================

void DrawPanelTree(PanelNode *node)
{
    if (!node)
        return;
    
    
    if (!node->isLeaf)
    {
        DrawPanelTree(node->first);
        DrawPanelTree(node->second);
        
        // Draw splitter
        
        Rectangle splitter =
            GetSplitterRect(node);
        
        DrawRectangleRec(
                         splitter,
                         (Color){50, 50, 55, 255}
                         );
        
        return;
    }
    
    
    // Panel background
    
    DrawRectangleRec(
                     node->rect,
                     (Color){35, 35, 40, 255}
                     );
    
    
    // Panel border
    
    DrawRectangleLinesEx(
                         node->rect,
                         1,
                         DARKGRAY
                         );
    
    
    // Panel ID
    
    char text[32];
    
    snprintf(
             text,
             sizeof(text),
             "Panel %d",
             node->id
             );
    
    
    DrawText(
             text,
             node->rect.x + 10,
             node->rect.y + 10,
             18,
             LIGHTGRAY
             );
    
    
    // Plus button
    
    DrawPlusButton(node->rect);
}


// ============================================================
// Main
// ============================================================

int main(void)
{
    const int WIDTH = 1200;
    const int HEIGHT = 800;
    
    
    InitWindow(
               WIDTH,
               HEIGHT,
               "Raylib - Adaptive Binary Panel Tree"
               );
    
    
    SetTargetFPS(60);
    
    
    PanelNode *root =
        CreatePanel(
                    (Rectangle)
                    {
                        10,
                        10,
                        WIDTH - 20,
                        HEIGHT - 20
                    }
                    );
    
    
    while (!WindowShouldClose())
    {
        // ====================================================
        // Window resizing
        // ====================================================
        
        int width = GetScreenWidth();
        int height = GetScreenHeight();
        
        
        root->rect =
        (Rectangle)
        {
            10,
            10,
            width - 20,
            height - 20
        };
        
        
        // Recalculate complete tree.
        
        LayoutPanelTree(root);
        
        
        // ====================================================
        // Update
        // ====================================================
        
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        
        UpdateSplitter(root);
        
        UpdatePanels(root);
        
        
        // ====================================================
        // Draw
        // ====================================================
        
        BeginDrawing();
        
        ClearBackground(
                        (Color){20, 20, 24, 255}
                        );
        
        
        DrawPanelTree(root);
        
        
        EndDrawing();
    }
    
    
    DestroyPanelTree(root);
    
    CloseWindow();
    
    return 0;
}
#endif