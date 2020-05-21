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

void physicsLoop();

int main()
{
	// Initialization
	//--------------------------------------------------------------------------------------

	// Player related variables
	Player player; 
	Mesh playerMesh;
	Model playerModel;
	BoundingBox playerBounds;

	int score = 0;
	int health = maxHealth;

	// Bullet related variables
	Bullet bulletPlayerPool[maxBullets];
	Bullet bulletEnemyPool[maxBullets];
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

	// Enemy fire rate timer
	float timerFire = 0.0f;

	// HUD texts
	char scoreDisp[TEXTSIZE];
	char healthDisp[TEXTSIZE];

	Camera camera;

	SetTraceLogLevel(LOG_ALL); // This allows us to print info onto the terminal

	InitWindow(screenWidth, screenHeight, title); // Creates the screen

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
	initEntity(&player, &playerModel, &playerBounds, BLUE, BLACK, 
				(Vector3){0.0f, yPos, 0.0f}, basePlayerSpeed, 1.0f, 0, true);

	initEntityPool(bulletPlayerPool, maxBullets, &bulletModel, &bulletBounds, GREEN, BLACK,
				 (Vector3){0.0f, yPos, 2.5f}, baseBulletPlayerSpeed, 0.5f, baseEnemyScore);
	initEntityPool(bulletEnemyPool, maxBullets, &bulletModel, &bulletBounds, DARKGREEN, BLACK,
				 (Vector3){0.0f, yPos, 2.5f}, baseBulletEnemySpeed, 0.5f, -100);

	initEntityPool(enemyPool, maxEnemies, &enemyModel, &enemyBounds, BLACK, PURPLE,
				 (Vector3){0.0f, yPos, 8.0f}, baseEnemySpeed, 1.2f, -150);

	// Initaize HUD displays
	TextCopy(healthDisp, TextFormat("LIVES: %d", health));
	TextCopy(scoreDisp, TextFormat("SCORE: %d", score));
	
	SetTargetFPS(60); // Set our game to run at 60 frames-per-second
	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose() && health >  0) // Detect window close button or ESC key. Dummy exit on player killed
	{
		// Update
		//----------------------------------------------------------------------------------

		// Timers control. Increment timer with time elapsed since last frame.
		timerEnemy += GetFrameTime(); 
		timerPhysics += GetFrameTime();
		timerFire += GetFrameTime();

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
			int i = getEntityFromPool(bulletPlayerPool, maxBullets);
		
			moveEntity(&bulletPlayerPool[i], player.position.x, yPos, 2.5f);
		}

		if (timerPhysics >= physicsPeriod)
		{
			physicsLoop();

			timerPhysics = 0.0f; // Restart timer
		}
		
		if (timerEnemy >= enemySpawnPeriod) // Spawn enemy if enough time passed already
		{
			// Spawn Enemy
			int i = getEntityFromPool(enemyPool, maxEnemies);

			moveEntity(&enemyPool[i], GetRandomValue(rightXLimit, leftXLimit), yPos, enemySpawnZ);

			enemyPool[i].speed = baseEnemySpeed + GetRandomValue(-speedVariability, +speedVariability);
					
			timerEnemy = 0.0f; // Restart timer
		}

		if (timerFire >= firePeriod){

			// Spawn enemy bullets
			int i;
			for (i = 0; i < maxEnemies; i++)
			{
				if (enemyPool[i].active){
					bool shooting = GetRandomValue(0, 100) <= fireProbability;

					if (shooting){
						int j = getEntityFromPool(bulletEnemyPool, maxBullets);

						moveEntity(&bulletEnemyPool[j], enemyPool[i].position.x, yPos, enemyPool[i].position.z - 1.0f);

						bulletEnemyPool[j].speed = baseBulletEnemySpeed + GetRandomValue(0, 2 * speedVariability);
					}
				}
			}
			
			timerFire = 0.0f; // Restart timer
		}
		
		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();
		{
			ClearBackground(RAYWHITE); // Clear background

			BeginMode3D(camera);
			{
				DrawGrid(200, 1.0f); // Static scenario
				
				// Draw player + border + shadow
				drawEntity(&player);

				// Draw all active bullets
				drawEntityPool(bulletPlayerPool, maxBullets);
				drawEntityPool(bulletEnemyPool, maxBullets);

				// Draw all active enemies
				drawEntityPool(enemyPool, maxEnemies);
			}
			EndMode3D();

			// HUD drawing
			// TODO proper HUD
			DrawText(title, 10, 40, 20, DARKGREEN);
			DrawText(healthDisp, screenWidth - MeasureText(healthDisp, 20) - 10, 10, 20, DARKGREEN);
			DrawText(scoreDisp, screenWidth - MeasureText(scoreDisp, 20) - 10, 30, 20, DARKGREEN);

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

void physicsLoop(){
	int minI = 0;
	float minDist = 999.0f;
	// Iterates through bullet pool to apply necessary changes
	for (int i = 0; i < maxBullets; i++)
	{
		int minJ = 0;
		// If active, move
		if (bulletPlayerPool[i].active){
			moveEntity(&bulletPlayerPool[i], bulletPlayerPool[i].position.x, bulletPlayerPool[i].position.y, 
						bulletPlayerPool[i].position.z + bulletPlayerPool[i].speed * timerPhysics);
			if (bulletPlayerPool[i].position.z >= frontZLimit) bulletPlayerPool[i].active = false; // Deactivate bullet if outside designed limits
		
			for (int j = 0; j < maxEnemies; j++)
			{
				if (enemyPool[j].active)
				{
					float dist = Vector3LengthSqr(Vector3Subtract(enemyPool[j].position, bulletPlayerPool[i].position));
					if (dist < minDist)
					{
						minJ = j;
						minDist = dist;
					}
				}
			}

			/// Bullet collision detection. Only if enemy is active (we don't want to collide with invisible enemies...)
			if (CheckCollisionBoxes(enemyPool[minJ].bounds, bulletPlayerPool[i].bounds))
			{
				// If bullet collides with enemy, mutual deactivation
				bulletPlayerPool[i].active = false;
				enemyPool[minJ].active = false;

				score += bulletPlayerPool[i].score;
				TextCopy(scoreDisp, TextFormat("SCORE: %d", score));
			}
		}
	}

	// Iterates through enemy pool to apply necessary changes
	for (int i = 0; i < maxEnemies; i++)
	{

		// If active, move
		if (enemyPool[i].active){
			moveEntity(&enemyPool[i], enemyPool[i].position.x, enemyPool[i].position.y, 
						enemyPool[i].position.z + enemyPool[i].speed * timerPhysics);
			if (enemyPool[i].position.z <= backZLimit) enemyPool[i].active = false; // Deactivate enemy if outside designed limits
		
			if (player.active){
				
				float dist = Vector3LengthSqr(Vector3Subtract(enemyPool[i].position, player.position));
				if (dist < minDist)
				{
					minI = i;
					minDist = dist;
				}
			}
		}

		/// Bullet collision detection. Only if enemy is active (we don't want to collide with invisible enemies...)
		if (enemyPool[minI].active && CheckCollisionBoxes(enemyPool[minI].bounds, player.bounds))
		{
			// If bullet collides with enemy, mutual deactivation
			enemyPool[minI].active = false;
			//player.active = false;

			score += enemyPool[minI].score;
			health--;
			TextCopy(scoreDisp, TextFormat("SCORE: %d", score));
			TextCopy(healthDisp, TextFormat("LIVES: %d", health));
		}

	}
	// Iterates through bullet pool to apply necessary changes
	for (int i = 0; i < maxBullets; i++)
	{
		// If active, move
		if (bulletEnemyPool[i].active){
			moveEntity(&bulletEnemyPool[i], bulletEnemyPool[i].position.x, bulletEnemyPool[i].position.y, 
						bulletEnemyPool[i].position.z + bulletEnemyPool[i].speed * timerPhysics);
			if (bulletEnemyPool[i].position.z <= backZLimit) bulletEnemyPool[i].active = false; // Deactivate bullet if outside designed limits
		
			if (player.active){
				
				float dist = Vector3LengthSqr(Vector3Subtract(bulletEnemyPool[i].position, player.position));
				if (dist < minDist)
				{
					minI = i;
					minDist = dist;
				}
			}
		}
	}

	// Bullet collision detection. Only if enemy is active (we don't want to collide with invisible enemies...)
	if (bulletEnemyPool[minI].active && CheckCollisionBoxes(bulletEnemyPool[minI].bounds, player.bounds))
	{
		// If bullet collides with enemy, mutual deactivation
		bulletEnemyPool[minI].active = false;
		//player.active = false;

		score += bulletEnemyPool[minI].score;
		health--;
		TextCopy(scoreDisp, TextFormat("SCORE: %d", score));
		TextCopy(healthDisp, TextFormat("LIVES: %d", health));
	}
}