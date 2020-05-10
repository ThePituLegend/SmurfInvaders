/*******************************************************************************************
*
*   Smurf Invaders. Technical Demo as an explorative project of Raylib.
*	Random game based on Space Invaders and alike.
*	Feel free to do whatever you want with this, although credit is nice :)
*
*	Made by ThePituLegend, May 2020
*
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*   Raylib by Ramon Santamaria (@raysan5), Copyright (c) 2013-2020
*
********************************************************************************************/

#include "raylib.h"
#include "raymath.h"

#include "constants.h"
#include "entity.h"

int main()
{
	// Initialization
	//--------------------------------------------------------------------------------------
	
	Player player; 
	Mesh playerMesh;
	Model playerModel;
	BoundingBox playerBounds;

	Bullet bulletPool[maxBullets];
	Mesh bulletMesh;
	Model bulletModel;
	BoundingBox bulletBounds;

	Enemy enemy;
	Mesh enemyMesh;
	Model enemyModel;
	BoundingBox enemyBounds;

	Camera camera;

	SetTraceLogLevel(LOG_ALL);

	InitWindow(screenWidth, screenHeight, "Smurf Invaders");

	camera.position = (Vector3){0.0f, 5.0f, -10.0f};
	camera.target = (Vector3){0.0f, 0.0f, 100.0f};
	camera.up = (Vector3){0.0f, 1.0f, 0.0f};
	camera.fovy = 60.0f;
	camera.type = CAMERA_PERSPECTIVE;

	SetCameraMode(camera, CAMERA_CUSTOM);

	playerMesh = GenMeshPoly(3, 2.0f);
	bulletMesh = GenMeshSphere(0.5f,10,10);
	enemyMesh = GenMeshCube(1.0f, 1.0f, 1.0f);

	playerModel = LoadModelFromMesh(playerMesh);
	bulletModel = LoadModelFromMesh(bulletMesh);
	enemyModel = LoadModelFromMesh(enemyMesh);

	playerBounds = MeshBoundingBox(playerMesh);
	bulletBounds = MeshBoundingBox(bulletMesh);
	enemyBounds = MeshBoundingBox(enemyMesh);

	initEntity(&player, &playerModel, playerBounds, BLACK, 100.0f, BLUE, BLACK, 
			(Vector3){0.0f, yPos, 0.0f}, (Vector3){0.2f, -yPos, 0.0f}, 10.0f, true);

	initEntityPool(bulletPool, maxBullets, &bulletModel, bulletBounds, BLACK, 100.0f, GREEN, BLACK, 
			(Vector3){0.0f, yPos, 2.5f}, (Vector3){0.2f, -yPos, 0.0f}, 10.0f);

	initEntity(&enemy, &enemyModel, enemyBounds, BLACK, 100.0f, BLACK, PURPLE, 
			(Vector3){0.0f, yPos, 8.0f}, (Vector3){0.2f, -yPos, 0.0f}, 0.0f, true);
			
	recalculateBounds(&enemy, enemyBounds);

	SetTargetFPS(60); // Set our game to run at 60 frames-per-second
	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose()) // Detect window close button or ESC key
	{
		// Update
		//----------------------------------------------------------------------------------
		UpdateCamera(&camera);

		player.position.x += player.speed * IsKeyDown(KEY_A) * GetFrameTime();
		player.position.x -= player.speed * IsKeyDown(KEY_D) * GetFrameTime();
		player.position.x = Clamp(player.position.x, rightXLimit, leftXLimit);
		recalculateBounds(&player, playerBounds);
		//TraceLog(LOG_DEBUG, "Player X = %.2f", player.position.x);

		if (IsKeyReleased(KEY_SPACE))
		{
			int i = 0;
			bool foundBullet = false;
			while (!foundBullet && i < maxBullets)
			{
				if (!bulletPool[i].active)
				{
					bulletPool[i].active = true;

					bulletPool[i].position = (Vector3){player.position.x, yPos, 2.5f};

					foundBullet = true;
				}

				i++;
			}
		}

		for (int i = 0; i < maxBullets; i++)
		{
			if (bulletPool[i].active){
				bulletPool[i].position.z += bulletPool[i].speed * GetFrameTime();
				recalculateBounds(&bulletPool[i], bulletBounds);
				if (bulletPool[i].position.z >= frontZLimit) bulletPool[i].active = false;
			}
			//TraceLog(LOG_DEBUG, "bullet[0] Z = %.2f", bullet[0].position.z);

			if (CheckCollisionBoxes(enemy.bounds, bulletPool[i].bounds) && enemy.active)
			{
				bulletPool[i].active = false;
				enemy.active = false;
			}
		}

		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();
		{
			ClearBackground(RAYWHITE);

			BeginMode3D(camera);
			{
				DrawGrid(100, 1.0f);
				
				DrawModel(*player.model, player.position, 1.0f, player.tint);
				DrawModelWires(*player.model, player.position, 1.0f, player.border);
				DrawModel(*player.model, Vector3Add(player.position, player.shadowOffset), 1.0f, player.shadow);
				//DrawBoundingBox(player.bounds, YELLOW); //DEBUG

				for (int i = 0; i < maxBullets; i++)
				{
					if (bulletPool[i].active){
						DrawModel(*bulletPool[i].model, bulletPool[i].position, 0.5f, bulletPool[i].tint);
						DrawModelWires(*bulletPool[i].model, bulletPool[i].position, 1.0f, bulletPool[i].border);
						DrawModel(*bulletPool[i].model, Vector3Add(bulletPool[i].position, bulletPool[i].shadowOffset), 0.5f, bulletPool[i].shadow);
						//DrawBoundingBox(bulletPool[i].bounds, YELLOW); //DEBUG
					}
				}

				if (enemy.active){
					DrawModel(*enemy.model, enemy.position, 1.2f, enemy.tint);
					DrawModelWires(*enemy.model, enemy.position, 1.2f, enemy.border);
					DrawModel(*enemy.model, Vector3Add(enemy.position, enemy.shadowOffset), 1.0f, enemy.shadow);
					//DrawBoundingBox(enemy.bounds, YELLOW); //DEBUG
				}
			}
			EndMode3D();

			DrawText("Smurf Invaders: Technical Demo", 10, 40, 20, DARKGREEN);

			DrawFPS(10, 10);
		}
		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	UnloadModel(*player.model);
	for (int i = 0; i < maxBullets; i++)
	{
		UnloadModel(*bulletPool[i].model);
	}
	UnloadModel(*enemy.model);

	UnloadMesh(playerMesh);
	UnloadMesh(bulletMesh);
	UnloadMesh(enemyMesh);
	
	CloseWindow(); // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}