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

/*******************************************************************************************
*
*   General definition of "Entity" and auxiliar functions.
*	Everything that moves or is interactible on the game, is defined as an entity.
*
********************************************************************************************/

#pragma once

// Raylib
#include "raylib.h"
#include "raymath.h"

// Entity Definition
typedef struct Entity
{
	Model* model; // Pointer to the model
	BoundingBox bounds; // Colision box
	Vector3 position;
	Vector3 shadowOffset; // Shadow is drawn as the model offsetted
	Color shadow; // Shadow color
	Color tint; // Entity color
	Color border; // Border color
	float speed;
	bool active; // This determines whether the entity is interactible and drawn
} Entity;


// Defines some specific type of entities. Mostly semantic sugar
typedef Entity Player;
typedef Entity Bullet;
typedef Entity Enemy;

// Initializes an entity with the given parameters
void initEntity(Entity* entity, Model* model, BoundingBox bounds, Color shadow, float shadowAlpha, Color tint,
				Color border, Vector3 position, Vector3 shadowOffset, float speed, bool active);

// Initializes a pool (array) of entities with the given parameters. Items of the pool are disabled by default
void initEntityPool(Entity* pool, int size, Model* model, BoundingBox bounds, Color shadow, float shadowAlpha, Color tint,
				Color border, Vector3 position, Vector3 shadowOffset, float speed);

// Recalculate the colision box for a given entity, given a reference box of that entity
void recalculateBounds(Entity* entity, BoundingBox referenceBox);