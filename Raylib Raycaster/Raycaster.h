#pragma once
#include "raylib.h"
#include "raymath.h"

class Raycaster
{
public: 
	Raycaster();

	void UpdateFrame();
	void RenderFrame();

private:
	void MovePlayer();
	void DrawRays();
	float RayDist(float ax, float ay, float bx, float by, float ang);

	Vector2 playerPosition;
	Vector2 playerDelta;
	
	Vector2 playerScale = Vector2{ 10,10 };
	float playerAngle;
	float currentRayDist;
};

