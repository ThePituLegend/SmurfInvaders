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

#ifndef ENTITY_H
#define ENTITY_H

// Raylib
#include "raylib.h"
#include "raymath.h"
#include "constants.h"

// Entity Definition
typedef struct Entity
{
	Model* model; // Pointer to the model
	BoundingBox bounds; // Collision box
	BoundingBox* refBounds; //Reference collision box
	Vector3 position; // Position
	Color tint; // Entity color
	Color border; // Border color
	float speed; // Entity speed
	float scale; // Entity Scaling
	int score; // How many points the entity gives when killed
	bool active; // This determines whether the entity is interactible and drawn
} Entity;

// Defines some specific type of entities. Mostly semantic sugar
typedef Entity Player;
typedef Entity Bullet;
typedef Entity Enemy;

// Initializes an entity with the given parameters
void initEntity(Entity* entity, Model* model, BoundingBox* bounds, Color tint,
				Color border, Vector3 position, float speed, float scale, int score, bool active);

// Initializes a pool (array) of entities with the given parameters. Items of the pool are disabled by default
void initEntityPool(Entity* pool, int size, Model* model, BoundingBox* bounds, Color tint,
				Color border, Vector3 position, float speed, float scale, int score);

// Recalculate the collision box for a given entity, given a reference box of that entity
void recalculateBounds(Entity* entity, BoundingBox referenceBox);

// Move Entity while carrying the collision box
void moveEntity(Entity* entity, float x, float y, float z);
void moveEntityVect(Entity* entity, Vector3 newPos);

// Drawing functions
void drawEntity(Entity* entity);
void drawEntityPool(Entity* pool, int size);

// Pooling. Returns 1st free item from pool, -1 if none.
int getEntityFromPool(Entity* pool, int size);

#endif