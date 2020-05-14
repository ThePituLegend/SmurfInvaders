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
	Enemy enemyPool[maxEnemies];
	Mesh enemyMesh;
	Model enemyModel;
	BoundingBox enemyBounds;

	// Timer to control enemy spawn rate
	float timerEnemy = 0.0f;

	// "Physics" timer
	float timerPhysics = 0.0f;

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
	bulletMesh = GenMeshSphere(0.5f, 10, 10);
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
	initEntity(&player, &playerModel, &playerBounds, BLUE, BLACK, (Vector3){0.0f, yPos, 0.0f}, 10.0f, 1.0f, true);

	initEntityPool(bulletPool, maxBullets, &bulletModel, &bulletBounds, GREEN, BLACK, (Vector3){0.0f, yPos, 2.5f}, 10.0f, 0.5f);

	initEntityPool(enemyPool, maxEnemies, &enemyModel, &enemyBounds, BLACK, PURPLE, (Vector3){0.0f, yPos, 8.0f}, 10.0f, 1.2f);
	
	SetTargetFPS(60); // Set our game to run at 60 frames-per-second
	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose()) // Detect window close button or ESC key
	{
		// Update
		//----------------------------------------------------------------------------------

		// Timers control. Increment timer with time elapsed since last frame.
		timerEnemy += GetFrameTime(); 
		timerPhysics += GetFrameTime(); 

		UpdateCamera(&camera); // Update camera (I think this is unnecessary in this case. TODO test this hypothesis)

		// Get player input to move player entity.
		// speed * frameTime = framerate-independant movement
		player.position.x += player.speed * IsKeyDown(KEY_A) * GetFrameTime();
		player.position.x -= player.speed * IsKeyDown(KEY_D) * GetFrameTime();
		moveEntity(&player, Clamp(player.position.x, rightXLimit, leftXLimit), player.position.y, player.position.z);
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

					moveEntity(&bulletPool[i], player.position.x, yPos, 2.5f);
					
					foundBullet = true; // Stop pool search
				}

				i++; // Thx my pana Dubledoh!
			}
		}

		if (timerPhysics >= physicsPeriod)
		{
			// Iterates through bullet pool to apply necessary changes
			for (int i = 0; i < maxBullets; i++)
			{
				int minJ = 0;
				// If active, move
				if (bulletPool[i].active){
					moveEntity(&bulletPool[i], bulletPool[i].position.x, bulletPool[i].position.y, 
								bulletPool[i].position.z + bulletPool[i].speed * timerPhysics);
					if (bulletPool[i].position.z >= frontZLimit) bulletPool[i].active = false; // Deactivate bullet if outside designed limits
				
					float minDist = 999.0f;
					for (int j = 0; j < maxEnemies; j++)
					{
						if (enemyPool[j].active)
						{
							float dist = Vector3Distance(enemyPool[j].position, bulletPool[i].position);
							if (dist < minDist)
							{
								minJ = j;
								minDist = dist;
							}
						}
					}
				}

				/// Bullet collision detection. Only if enemy is active (we don't want to collide with invisible enemies...)
				if (CheckCollisionBoxes(enemyPool[minJ].bounds, bulletPool[i].bounds))
				{
					// If bullet collides with enemy, mutual deactivation
					bulletPool[i].active = false;
					enemyPool[minJ].active = false;
				}
			}

			// Iterates through enemy pool to apply necessary changes
			for (int i = 0; i < maxEnemies; i++)
			{
				// If active, move
				if (enemyPool[i].active){
					moveEntity(&enemyPool[i], enemyPool[i].position.x, enemyPool[i].position.y, 
								enemyPool[i].position.z - enemyPool[i].speed * timerPhysics);
					if (enemyPool[i].position.z <= backZLimit) enemyPool[i].active = false; // Deactivate enemy if outside designed limits
				}

			}

			timerPhysics = 0.0f; // Restart timer
		}
		
		if (timerEnemy >= enemySpawnPeriod) // Spawn enemy if enough time passed already
		{
			// Spawn Enemy
			// Iterates through bullet pool until available bullet is found, or until end.
			int i = 0;
			bool foundEnemy = false;
			while (!foundEnemy && i < maxEnemies)
			{
				// enemy not active =  available enemy
				if (!enemyPool[i].active)
				{
					enemyPool[i].active = true; // Mark enenmy as used (render & interactive)

					// Position enemy in empty space
					moveEntity(&enemyPool[i], GetRandomValue(rightXLimit, leftXLimit), yPos, enemySpawnZ);
					
					foundEnemy = true; // Stop pool search
				}

				i++;
			}

			timerEnemy = 0.0f; // Restart timer
		}
		
		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();
		{
			ClearBackground(RAYWHITE); // Clear background

			BeginMode3D(camera);
			{
				DrawGrid(100, 1.0f); // Static scenario
				
				// Draw player + border + shadow
				drawEntity(&player);

				// Draw all active bullets
				drawEntityPool(bulletPool, maxBullets);

				// Draw all active enemies
				drawEntityPool(enemyPool, maxEnemies);
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