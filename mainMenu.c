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

#include "mainMenu.h"

State mainMenu()
{
	// Initialization
	//--------------------------------------------------------------------------------------
	// Initaize HUD displays
	//TextCopy(healthDisp, TextFormat("LIVES: %d", health));
	//TextCopy(scoreDisp, TextFormat("SCORE: %d", score));

	SetExitKey(KEY_ESCAPE);	

	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose()) // Detect window close button or ESC key. Dummy exit on player killed
	{
		// Update
		//----------------------------------------------------------------------------------

		if (IsKeyPressed(KEY_ENTER)) return GAME;
		if (IsKeyPressed(KEY_S)) return SCORE_M;
		
		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();
		{
			ClearBackground(RAYWHITE); // Clear background

			// Menu drawing
			DrawText("SMURF INVADERS", screenWidth / 2 - MeasureText("SMURF INVADERS", 40) / 2, screenHeight / 2 - 10, 40, DARKGREEN);

			DrawText("Press ENTER to start", screenWidth / 2 - MeasureText("Press ENTER to start", 20) / 2, screenHeight / 2 + 50, 20, BLUE);
			DrawText("Press S to see the scoreboard", screenWidth / 2 - MeasureText("Press S to see the scoreboard", 20) / 2, screenHeight / 2 + 90, 20, BLUE);
			DrawText("Press ESC to quit", screenWidth / 2 - MeasureText("Press ESC to quit", 20) / 2, screenHeight / 2 + 130, 20, BLUE);

		}
		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------

	return EXIT;
}