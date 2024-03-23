#include "Raycaster.h"
#include <iostream>

#define P2 PI/2
#define P3 3*PI/2

int mapX = 8, mapY = 8, mapS = 64;
int map[] =
{
	1,1,1,1,1,1,1,1,
	1,0,0,0,0,0,0,1,
	1,0,1,0,0,0,0,1,
	1,0,1,0,0,0,0,1,
	1,0,1,0,0,0,0,1,
	1,0,0,0,0,1,0,1,
	1,0,0,0,0,0,0,1,
	1,1,1,1,1,1,1,1,
};

Raycaster::Raycaster()
{
	float x = GetScreenWidth() / static_cast<float>(2) - 250;
	float y = GetScreenHeight() / static_cast<float>(2);

	playerPosition.x = x;
	playerPosition.y = y;

	playerAngle = 0;
	playerDelta.x = cos(playerAngle) * 5;
	playerDelta.y = sin(playerAngle) * 5;
	
	SetTargetFPS(30);
}

void Raycaster::UpdateFrame()
{
	MovePlayer();
}

void Raycaster::RenderFrame()
{
	BeginDrawing();
	ClearBackground(GRAY);
	
	int xo = 0;
	int yo = 0;

	for (size_t y = 0; y < mapY; y++)
	{
		for (size_t x = 0; x < mapX; x++)
		{
			Color chosenColor = map[y * mapX + x] == 1 ? WHITE : BLACK;
			xo = x * mapS;
			yo = y * mapS;

			DrawRectangle(xo, yo, mapS, mapS, chosenColor);
			DrawRectangleLines(xo, yo, mapS, mapS, GRAY);
		}
	}
	
	DrawRays();
	DrawRectangleV(Vector2Subtract(playerPosition, Vector2{ playerScale.x * .5f, playerScale.y * .5f}), playerScale, RED); //draw player
	DrawLineEx(playerPosition, Vector2{ playerPosition.x + playerDelta.x * 5 , playerPosition.y + playerDelta.y * 5 }, 2, RED);

	DrawFPS(0,0);
	EndDrawing();
}

void Raycaster::MovePlayer() {
	if (IsKeyDown(KEY_W)) {

		if (currentRayDist <= 5)
		{
			playerPosition.x -= playerDelta.x;
			playerPosition.y -= playerDelta.y;
			return;
		}
		playerPosition.x += playerDelta.x;
		playerPosition.y += playerDelta.y;
	}
	if (IsKeyDown(KEY_S)) {
		playerPosition.x -= playerDelta.x;
		playerPosition.y -= playerDelta.y;
	}

	if (IsKeyDown(KEY_A)) {
		playerAngle -= .1;
		if (playerAngle < 0) playerAngle += 2 * PI;
		
		playerDelta.x = cos(playerAngle) * 5;
		playerDelta.y = sin(playerAngle) * 5;
	}
	if (IsKeyDown(KEY_D)) {
		playerAngle += .1;
		if (playerAngle > 2 * PI) playerAngle -= 2 * PI;

		playerDelta.x = cos(playerAngle) * 5;
		playerDelta.y = sin(playerAngle) * 5;
	}

}

float Raycaster::RayDist(float ax, float ay, float bx, float by, float ang)
{
	return (sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay)));
}

void Raycaster::DrawRays()
{
	int r = 0, mx = 0, my = 0, mp = 0, dof = 0, disT = 0;
	float rx = 0, ry = 0, ra = 0, xo = 0, yo = 0;

	ra = playerAngle - DEG2RAD * 30;
	if (ra < 0) { ra += 2 * PI; } if (ra > 2 * PI) { ra -= 2 * PI; }

	for (r = 0; r < 60; r++)
	{
		dof = 0;

		float disH = 1000000, hx = playerPosition.x, hy = playerPosition.y;
		float aTan = -1 / tan(ra);

		if (ra > PI) { 
			ry = (((int)playerPosition.y >> 6) << 6) - 0.0001; 
			rx = (playerPosition.y - ry) * aTan + playerPosition.x;
			yo = -64;
			xo = -yo * aTan;
		}

		if (ra < PI) {
			ry = (((int)playerPosition.y >> 6) << 6) + 64;
			rx = (playerPosition.y - ry) * aTan + playerPosition.x;
			yo = 64;
			xo = -yo * aTan;
		}

		if (ra == 0 || ra == PI) {
			rx = playerPosition.x;
			ry = playerPosition.y;
			dof = 8;
		}
		while (dof < 8)
		{
			mx = static_cast<int>(rx) >> 6;
			my = static_cast<int>(ry) >> 6;
			mp = my * mapX + mx;

			if (mp > 0 && mp < mapX * mapY && map[mp] == 1)
			{
				dof = 8;
				hx = rx;
				hy = ry;
				disH = RayDist(playerPosition.x, playerPosition.y, hx, hy, ra);
			}
			else
			{
				rx += xo;
				ry += yo;
				dof += 1; // Check next horizontal
			}
		}

		//Check vertical lines
		dof = 0;
		
		float disV = 1000000, vx = playerPosition.x, vy = playerPosition.y;
		float nTan = -tan(ra);

		if (ra > P2 && ra < P3) {
			rx = (((int)playerPosition.x >> 6) << 6) - 0.0001;
			ry = (playerPosition.x - rx) * nTan + playerPosition.y;
			xo = -64;
			yo = -xo * nTan;
		}

		if (ra < P2 || ra > P3) {
			rx = (((int)playerPosition.x >> 6) << 6) + 64;
			ry = (playerPosition.x - rx) * nTan + playerPosition.y;
			xo = 64;
			yo = -xo * nTan;
		}

		if (ra == 0 || ra == PI) {
			rx = playerPosition.x;
			ry = playerPosition.y;
			dof = 8;
		}
		while (dof < 8)
		{
			mx = static_cast<int>(rx) >> 6;
			my = static_cast<int>(ry) >> 6;
			mp = my * mapX + mx;

			if (mp > 0 && mp < mapX * mapY && map[mp] == 1)
			{
				dof = 8;
				vx = rx;
				vy = ry;
				disV = RayDist(playerPosition.x, playerPosition.y, vx, vy, ra);
			}
			else
			{
				rx += xo;
				ry += yo;
				dof += 1; 
			}
		}
		Color lineColor;

		if (disV < disH) { rx = vx; ry = vy; disT = disV; lineColor = Color{ 200, 0, 0, 255 }; }
		if (disH < disV) { rx = hx; ry = hy; disT = disH; lineColor = Color{ 150, 0, 0, 255 }; }

		DrawLineEx(playerPosition, Vector2{ rx, ry }, 2, BLUE);

		float ca = playerAngle - ra; if (ca > 2 * PI) { ca -= 2 * PI; } disT = disT * cos(ca);
		float lineH = (mapS * static_cast<float>(320)) / disT;
		if (lineH > 320) lineH = 320;

		float lineO = 160 - static_cast<int>(lineH / 2);
		DrawLineEx(Vector2{ (float)r * 8 + 530, lineO }, Vector2{ (float)r * 8 + 530, lineH + lineO }, 8, lineColor);

		currentRayDist = disT;

		ra += DEG2RAD; 
		if (ra < 0) { ra += 2 * PI; } if (ra > 2 * PI) { ra -= 2 * PI; }
	}
}
