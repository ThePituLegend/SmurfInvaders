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

#include "scoreMenu.h"

State scoreMenu()
{
	// Initialization
	//--------------------------------------------------------------------------------------
	// Initaize HUD displays
	//TextCopy(healthDisp, TextFormat("LIVES: %d", health));
	//TextCopy(scoreDisp, TextFormat("SCORE: %d", score));

	SetExitKey(KEY_PAUSE);	

	//--------------------------------------------------------------------------------------

	// Main game loop
	while (true) // Detect window close button or ESC key. Dummy exit on player killed
	{
		// Update
		//----------------------------------------------------------------------------------

		if (IsKeyPressed(KEY_ESCAPE)) return MAIN_M;
		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();
		{
			ClearBackground(RAYWHITE); // Clear background

			// Menu drawing
			DrawText("SCORE", screenWidth / 2 - MeasureText("SCORE", 40) / 2, screenHeight / 2 - 10, 40, DARKGREEN);

			DrawText("Press ESCAPE to return main menu", screenWidth / 2 - MeasureText("Press ESCAPE to return main menu", 20) / 2, screenHeight / 2 + 50, 20, BLUE);
		}
		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------
}