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

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "raylib.h"

// Game Constants

const static int screenWidth = 800;
const static int screenHeight = 600;

const static int maxBullets = 10;
const static int maxEnemies = 10;

const static float leftXLimit = 6.0f;
const static float rightXLimit = -6.0f;
const static float frontZLimit = 45.0f;

const static float yPos = 0.3f;

const static float enemySpawnPeriod = 4.0f;
const static float enemySpawnZ = 25.0f;

const static struct
{
	Vector3 offset; // Shadow is drawn as the model offsetted
	Color color; // Shadow color
} defaultShadow = {{0.2f, -yPos, 0.0f}, { 0.0f, 0.0f, 0.0f, 100.0f }};

#endif