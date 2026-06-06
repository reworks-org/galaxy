#include "raylib.h"
#include "math.h"

#define VIEWPORT_SCALING_IMPLEMENTATION
#include "viewport_scaling.h"

void GameInit();
void GameUpdate();
void GameDraw();
void UpdateResolution();
void DrawScene();

int gameWidth = 320;
int gameHeight = 180;
int screenWidth = 320;
int screenHeight = 180;
RenderTexture viewport;
NezRect_f viewRect;
NezRect_f screenRect;

void (*scaler[6])(Rectangle*, Rectangle*, int*, int*, int*, int*);
int funcIndex = 0;

int main(void){
    GameInit();
    SetTargetFPS(60);
    while (!WindowShouldClose()){
        GameUpdate();
        GameDraw();
    }
    UnloadRenderTexture(viewport);
    CloseWindow();
    return 0;
}

void GameInit() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "2D viewport scaling");
    scaler[0] = ViewportKeepAspectPixel;
    scaler[1] = ViewportKeepHeightPixel;
    scaler[2] = ViewportKeepWidthPixel;
    scaler[3] = ViewportKeepAspect;
    scaler[4] = ViewportKeepHeight;
    scaler[5] = ViewportKeepWidth;
    UpdateResolution();
}

void GameUpdate(){
    // change scaling function
    if (IsKeyPressed(KEY_TAB)){
        funcIndex = (funcIndex + 1) % 6;
        UpdateResolution();
    }
    if (IsWindowResized()){
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();
        UpdateResolution();
    }
}

void UpdateResolution(){
    scaler[funcIndex](&viewRect, &screenRect, &gameWidth, &gameHeight, &screenWidth, &screenHeight);
    
    // Change resolution for RenderTexture
    UnloadRenderTexture(viewport);
    viewport = LoadRenderTexture((int)viewRect.w, (int)viewRect.h);
    
    // RenderTexture needs to be flipped
    viewRect.h *= -1;
}

void GameDraw(){
  const Vector2 orig = (Vector2){ 0.0f, 0.0f };
    const char* modes[] = {
        "Keep aspect (pixel) \nTAB to change",
        "Keep height (pixel) \nTAB to change",
        "Keep width (pixel) \nTAB to change",
        "Keep aspect \nTAB to change",
        "Keep height \nTAB to change",
        "Keep width \nTAB to change",
    };
    BeginTextureMode(viewport);
        DrawScene();
     EndTextureMode();
    BeginDrawing();
    ClearBackground(RAYWHITE);
        DrawTexturePro(viewport.texture, *(Rectangle*)&viewRect, *(Rectangle*)&screenRect, orig, 0.0f, WHITE);
        DrawRectangleLinesEx(*(Rectangle*)&screenRect, 1, BLACK);
        
        DrawText(modes[funcIndex], 7, 8, 16, WHITE);
        DrawText(modes[funcIndex], 9, 8, 16, WHITE);
        DrawText(modes[funcIndex], 8, 7, 16, WHITE);
        DrawText(modes[funcIndex], 8, 9, 16, WHITE);
        DrawText(modes[funcIndex], 8, 8, 16, BLACK);
    EndDrawing();
}

void DrawScene(){
    const Vector2 orig = (Vector2){ 0.0f, 0.0f };
    DrawRectangleLines(0, 0, gameWidth, gameHeight, GOLD);
    DrawLine(0,0,gameHeight, gameHeight, LIGHTGRAY);
    DrawLine(gameWidth,0, gameWidth - gameHeight, gameHeight, LIGHTGRAY);
    for (int i = 0; i < 20; i++){
        int m = 10;
        DrawTexturePro(viewport.texture, *(Rectangle*)&viewRect, *(Rectangle*)&screenRect, orig, 0.0f, WHITE);
    }
}

