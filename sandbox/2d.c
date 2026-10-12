#include <raylib.h>
#include <stddef.h>
#include <unistd.h>
#include <stdbool.h>

#define HEIGHT 600
#define WIDTH 960

int main(void)
{
    InitWindow(WIDTH, HEIGHT, "nbody");
    SetTargetFPS(60);

    float centerX, centerY;
    float radius;
    radius = 40;
    centerX = radius;
    centerY = radius;
    float ay = 980.0f;
    float vy = 1.0f;
    float vx = 1.0f;
    float bounce_factor = -0.8;
    bool is_paused = false;


    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        if(IsKeyPressed(KEY_SPACE)){
            is_paused = !is_paused;
        }

        if(!is_paused){
            if(centerX + radius >= WIDTH){
                centerX = WIDTH - radius;
                vx *= -1;
            }
            else if(centerX <= radius){
                centerX = radius;
                vx *= -1;
            }
            centerX += vx;

            float dt = GetFrameTime();
            vy += ay*dt;
            if(centerY + radius >= HEIGHT){
                centerY = HEIGHT - radius;
                vy *= bounce_factor;
            }

            centerY += vy*dt;
        }


        DrawCircle((int) centerX, (int) centerY, radius, RED);
        DrawText(TextFormat("Gravity: %.1f | Vel: %.1f", ay, vy), 20, 40, 20, RAYWHITE);
        DrawFPS(0,0);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
