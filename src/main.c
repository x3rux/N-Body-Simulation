#include <raylib.h>

int main(void)
{
    const int screenWidth = 960;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "N-Body Simulator");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        DrawText("N-Body Simulator", 20, 20, 24, RAYWHITE);
        DrawFPS(20, 50);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
