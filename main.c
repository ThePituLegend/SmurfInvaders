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

#include "game.h"
#include "mainMenu.h"
#include "scoreMenu.h"
#include "gameOverMenu.h"

int main()
{
	// Initialization
	//--------------------------------------------------------------------------------------

	State gameState = MAIN_M;

	SetTraceLogLevel(LOG_ALL); // This allows us to print info onto the terminal

	InitWindow(screenWidth, screenHeight, title); // Creates the screen
	
	SetTargetFPS(60); // Set our game to run at 60 frames-per-second

	//--------------------------------------------------------------------------------------

	// Main game loop
	while (gameState != EXIT) // Detect window close button or ESC key.
	{
		switch (gameState)
		{
		case MAIN_M:
			gameState = mainMenu();
			break;

		case SCORE_M:
			gameState = scoreMenu();
			break;

		case GAME:
			gameState = gameCore();
			break;

		case GO_M:
			gameState = gameOverMenu();
			break;

		default:
			break;
		}

	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	
	CloseWindow(); // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}