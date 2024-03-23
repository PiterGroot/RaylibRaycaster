#include <iostream>
#include "Raycaster.h"

int main()
{
    InitWindow(1024, 512, "Raycaster"); //create raylib window

    Raycaster* raycaster = new Raycaster();

    while (!WindowShouldClose())
    {
        raycaster->UpdateFrame();
        raycaster->RenderFrame();
    }

    CloseWindow();
    return 0;
}