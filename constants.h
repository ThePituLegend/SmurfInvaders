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

const static int TEXTSIZE = 256;

typedef enum {EXIT, MAIN_M, SCORE_M, GAME, GO_M} State;

// Game Constants

const static int screenWidth = 800;
const static int screenHeight = 600;

const static int maxBullets = 15;
const static int maxEnemies = 15;

const static float leftXLimit = 6.0f;
const static float rightXLimit = -6.0f;
const static float frontZLimit = 60.0f;
const static float backZLimit = -5.0f;

const static float yPos = 0.3f;
const static float speedVariability = 2.0f;

const static float basePlayerSpeed = 10.0f;
const static float baseBulletPlayerSpeed = 15.0f;
const static float baseBulletEnemySpeed = -15.0f;

const static float enemySpawnPeriod = 2.0f;
const static float enemySpawnZ = 100.0f;
const static int fireProbability = 50;
const static float firePeriod = 1.5f;
const static float baseEnemySpeed = -10.0f;

const static float physicsPeriod = 0.03f;

const static int maxHealth = 3;
const static int baseEnemyScore = 100;

const static int scoreboardSize = 5;

const static struct
{
	Vector3 offset; // Shadow is drawn as the model offsetted
	Color color; // Shadow color
} defaultShadow = {{0.2f, -yPos, 0.0f}, { 0.0f, 0.0f, 0.0f, 100.0f }};

// HUD Constants
const static char title[] = "Smurf Invaders: Technical Demo\0";

#endif