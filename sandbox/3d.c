#include <raylib.h>
#include <stddef.h>
#include <unistd.h>
#include <stdbool.h>

#define HEIGHT 600
#define WIDTH 960
#define BOX_SIZE 10.0f

int main(void)
{
    InitWindow(WIDTH, HEIGHT, "nbody");
    SetTargetFPS(60);

    float radius_3d;
    radius_3d = 0.5f;
    float ay = -9.8f;
    Vector3 vel = {6.0f, 0.1f, 6.0f};
    float bounce_factor = -0.8;

    Vector3 center = {-10, 10, 0};

    Camera3D camera = { 0 };
    camera.position = (Vector3) {0.0, 10.0, 25.0};
    camera.target = (Vector3) {0.0f, 0.0f, 0.0f};
    camera.up = (Vector3) {0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    while (!WindowShouldClose()) {
        UpdateCamera(&camera, CAMERA_ORBITAL);
        BeginDrawing();
        ClearBackground(BLACK);


        BeginMode3D(camera);

        DrawGrid(20, 1.0f);

        if(center.x + radius_3d >= BOX_SIZE){
            center.x = BOX_SIZE - radius_3d;
            vel.x *= -1;
        } else if(center.x - radius_3d <= -BOX_SIZE){
            center.x = radius_3d - BOX_SIZE;
            vel.x *= -1;
        }

        if(center.z + radius_3d >= BOX_SIZE){
            center.z = BOX_SIZE - radius_3d;
            vel.z *= -1;
        } else if(center.z - radius_3d <= -BOX_SIZE){
            center.z = radius_3d - BOX_SIZE;
            vel.z *= -1;
        }

        float dt = GetFrameTime();
        vel.y += ay*dt;
        if(center.y - radius_3d <= 0){
            center.y = radius_3d;
            vel.y *= bounce_factor;
        }


        center.y += vel.y * dt;
        center.x += vel.x * dt;
        center.z += vel.z * dt;

        DrawSphere(center, radius_3d, RED);
        EndMode3D();

        DrawText(TextFormat("Gravity: %.1f | Vel: %.1f", ay, vel.y), 20, 40, 20, RAYWHITE);
        DrawFPS(0,0);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
