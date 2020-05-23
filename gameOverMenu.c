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

#include "gameOverMenu.h"

State gameOverMenu()
{
	// Initialization
	//--------------------------------------------------------------------------------------
	// Initaize HUD displays
	//TextCopy(healthDisp, TextFormat("LIVES: %d", health));
	//TextCopy(scoreDisp, TextFormat("SCORE: %d", score));

	//--------------------------------------------------------------------------------------

	// Main game loop
	while (true) // Detect window close button or ESC key. Dummy exit on player killed
	{
		// Update
		//----------------------------------------------------------------------------------

		if (IsKeyPressed(KEY_ENTER)) return GAME;
		if (IsKeyPressed(KEY_ESCAPE)) return MAIN_M;
		
		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();
		{
			ClearBackground(RAYWHITE); // Clear background

			// Menu drawing
			DrawText("GAME OVER!", screenWidth / 2 - MeasureText("GAME OVER!", 40) / 2, screenHeight / 2 - 70, 40, GREEN);
			DrawText("Press ENTER to play again", screenWidth / 2 - MeasureText("Press ENTER to play again", 20) / 2,
					screenHeight / 2, 20, BLUE);
			DrawText("Press ESCAPE to return main menu", screenWidth / 2 - MeasureText("Press ESCAPE to return main menu", 20) / 2,
					screenHeight / 2 + 40, 20, BLUE);

		}
		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------

	return EXIT;
}