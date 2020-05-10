/*******************************************************************************************
*
*   Smurf Invaders. Technical Demo as an explorative project of Raylib.
*	Random game based on Space Invaders and alike.
*
*	Copyright (c) 2020 Oscar Lostes Cazorla, under MIT license.
*
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*   Raylib by Ramon Santamaria (@raysan5), Copyright (c) 2013-2020
*
********************************************************************************************/

// Raylib
#include "raylib.h"
#include "raymath.h"

// Custom files
#include "constants.h"
#include "entity.h"

int main()
{
	// Initialization
	//--------------------------------------------------------------------------------------

	// Player related variables
	Player player; 
	Mesh playerMesh;
	Model playerModel;
	BoundingBox playerBounds;

	// Bullet related variables
	Bullet bulletPool[maxBullets];
	Mesh bulletMesh;
	Model bulletModel;
	BoundingBox bulletBounds;

	// Enemy related variables
	Enemy enemy;
	Mesh enemyMesh;
	Model enemyModel;
	BoundingBox enemyBounds;

	Camera camera;

	SetTraceLogLevel(LOG_ALL); // This allows us to print info onto the terminal

	InitWindow(screenWidth, screenHeight, "Smurf Invaders"); // Creates the screen

	// Camera configuration
	// TODO Make this configurations constants
	camera.position = (Vector3){0.0f, 5.0f, -10.0f};
	camera.target = (Vector3){0.0f, 0.0f, 100.0f};
	camera.up = (Vector3){0.0f, 1.0f, 0.0f};
	camera.fovy = 60.0f;
	camera.type = CAMERA_PERSPECTIVE;

	SetCameraMode(camera, CAMERA_CUSTOM); // Custom mode gives us full controll of the camera. No predefined behaviour

	// Generate all meshes
	playerMesh = GenMeshPoly(3, 2.0f);
	bulletMesh = GenMeshSphere(0.5f,10,10);
	enemyMesh = GenMeshCube(1.0f, 1.0f, 1.0f);


	// Generate all models
	playerModel = LoadModelFromMesh(playerMesh);
	bulletModel = LoadModelFromMesh(bulletMesh);
	enemyModel = LoadModelFromMesh(enemyMesh);

	// Generate all reference BoundingBoxes
	playerBounds = MeshBoundingBox(playerMesh);
	bulletBounds = MeshBoundingBox(bulletMesh);
	enemyBounds = MeshBoundingBox(enemyMesh);

	// Initialization of entities
	initEntity(&player, &playerModel, playerBounds, BLACK, 100.0f, BLUE, BLACK, 
			(Vector3){0.0f, yPos, 0.0f}, (Vector3){0.2f, -yPos, 0.0f}, 10.0f, true);

	initEntityPool(bulletPool, maxBullets, &bulletModel, bulletBounds, BLACK, 100.0f, GREEN, BLACK, 
			(Vector3){0.0f, yPos, 2.5f}, (Vector3){0.2f, -yPos, 0.0f}, 10.0f);

	initEntity(&enemy, &enemyModel, enemyBounds, BLACK, 100.0f, BLACK, PURPLE, 
			(Vector3){0.0f, yPos, 8.0f}, (Vector3){0.2f, -yPos, 0.0f}, 0.0f, true);
	
	recalculateBounds(&enemy, enemyBounds); // TEMPORAL, while enemy is dummy

	SetTargetFPS(60); // Set our game to run at 60 frames-per-second
	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose()) // Detect window close button or ESC key
	{
		// Update
		//----------------------------------------------------------------------------------
		UpdateCamera(&camera); // Update camera (I think this is unnecessary in this case. TODO test this hypothesis)

		// Get player input to move player entity.
		// speed * frameTime = framerate-indepedant movement
		player.position.x += player.speed * IsKeyDown(KEY_A) * GetFrameTime();
		player.position.x -= player.speed * IsKeyDown(KEY_D) * GetFrameTime();
		player.position.x = Clamp(player.position.x, rightXLimit, leftXLimit); // Limits player positions between designed region
		recalculateBounds(&player, playerBounds); // Recalculate player bounding box
		//TraceLog(LOG_DEBUG, "Player X = %.2f", player.position.x);

		// Bullet shooting
		if (IsKeyReleased(KEY_SPACE))
		{
			// Iterates through bullet pool until available bullet is found, or until end.
			int i = 0;
			bool foundBullet = false;
			while (!foundBullet && i < maxBullets)
			{
				// bullet not active =  available bullet
				if (!bulletPool[i].active)
				{
					bulletPool[i].active = true; // Mark bullet as used (render & interactive)

					bulletPool[i].position = (Vector3){player.position.x, yPos, 2.5f}; // Bullet position, fixed by player position

					foundBullet = true; // Stop pool search
				}

				i++;
			}
		}

		// Iterates through bullet pool to apply necessary changes
		for (int i = 0; i < maxBullets; i++)
		{
			// If active, move
			if (bulletPool[i].active){
				bulletPool[i].position.z += bulletPool[i].speed * GetFrameTime(); // Move forward
				recalculateBounds(&bulletPool[i], bulletBounds); // Recalculate bullet bounding box
				if (bulletPool[i].position.z >= frontZLimit) bulletPool[i].active = false; // Deactivate bullet if outside designed limits
			}
			//TraceLog(LOG_DEBUG, "bullet[0] Z = %.2f", bullet[0].position.z);

			// Bullet collision detection. Only if enemy is active (we don't want to collide with invisible enemies...)
			if (CheckCollisionBoxes(enemy.bounds, bulletPool[i].bounds) && enemy.active)
			{
				// If bullet collides with enemy, mutual deactivation
				bulletPool[i].active = false;
				enemy.active = false;
			}
		}

		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();
		{
			ClearBackground(RAYWHITE); // Clear backgrounf

			BeginMode3D(camera);
			{
				DrawGrid(100, 1.0f); // Static scenario
				
				// Draw player + border + shadow
				DrawModel(*player.model, player.position, 1.0f, player.tint);
				DrawModelWires(*player.model, player.position, 1.0f, player.border);
				DrawModel(*player.model, Vector3Add(player.position, player.shadowOffset), 1.0f, player.shadow);
				//DrawBoundingBox(player.bounds, YELLOW); //DEBUG

				// Draw all active bullets
				for (int i = 0; i < maxBullets; i++)
				{
					if (bulletPool[i].active){
						// Draw bullet + border + shadow
						DrawModel(*bulletPool[i].model, bulletPool[i].position, 0.5f, bulletPool[i].tint);
						DrawModelWires(*bulletPool[i].model, bulletPool[i].position, 1.0f, bulletPool[i].border);
						DrawModel(*bulletPool[i].model, Vector3Add(bulletPool[i].position, bulletPool[i].shadowOffset), 0.5f, bulletPool[i].shadow);
						//DrawBoundingBox(bulletPool[i].bounds, YELLOW); //DEBUG
					}
				}

				// Draw active enemy + border + shadow
				if (enemy.active){
					DrawModel(*enemy.model, enemy.position, 1.2f, enemy.tint);
					DrawModelWires(*enemy.model, enemy.position, 1.2f, enemy.border);
					DrawModel(*enemy.model, Vector3Add(enemy.position, enemy.shadowOffset), 1.0f, enemy.shadow);
					//DrawBoundingBox(enemy.bounds, YELLOW); //DEBUG
				}
			}
			EndMode3D();

			// HUD drawing
			// TODO proper HUD
			DrawText("Smurf Invaders: Technical Demo", 10, 40, 20, DARKGREEN);

			DrawFPS(10, 10);
		}
		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	// Unload model data
	UnloadModel(playerModel);
	UnloadModel(bulletModel);
	UnloadModel(enemyModel);
	
	// Unload mesh data
	UnloadMesh(playerMesh);
	UnloadMesh(bulletMesh);
	UnloadMesh(enemyMesh);
	
	CloseWindow(); // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}