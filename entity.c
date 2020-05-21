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

#include "entity.h"

void initEntity(Entity* entity, Model* model, BoundingBox* bounds, Color tint,
				Color border, Vector3 position, float speed, float scale, int score, bool active){
	
	// Inits the entity struct, element by element
	entity->model = model;
	entity->bounds = *bounds;
	entity->refBounds = bounds;
	entity->tint = tint;
	entity->border = border;
	entity->speed = speed;
	entity->scale = scale;
	entity->active = active;
	entity->score = score;

	moveEntityVect(entity, position);
}

void initEntityPool(Entity* pool, int size, Model* model, BoundingBox* bounds, Color tint,
				Color border, Vector3 position, float speed, float scale, int score){
	
	// Iterates through the given entity array
	for (int i = 0; i < size; i++)
	{
		// Inits each entity
		initEntity(&pool[i], model, bounds, tint, border, 
			position, speed, scale, score, false);
	}
}

void recalculateBounds(Entity* entity, BoundingBox referenceBox){
	// referenceBox is the pattern BoundingBox of the entity, located at (0,0,0).
	// This means that the actual boundingBox of given entity is referenceBox + entity->position
	entity->bounds.min = Vector3Add(referenceBox.min, entity->position);
	entity->bounds.max = Vector3Add(referenceBox.max, entity->position);

	// DEBUG
	//TraceLog(LOG_DEBUG, "Bounds min: (%.2f, %.2f, %.2f)", entity->bounds.min.x, entity->bounds.min.y, entity->bounds.min.z);
	//TraceLog(LOG_DEBUG, "Bounds max: (%.2f, %.2f, %.2f)", entity->bounds.max.x, entity->bounds.max.y, entity->bounds.max.z));
}

void moveEntityVect(Entity* entity, Vector3 newPos){
	entity->position = newPos; // Set new position
	recalculateBounds(entity, *entity->refBounds); // Recalculate collision box
}

void moveEntity(Entity* entity, float x, float y, float z){
	entity->position = (Vector3){x, y, z}; // Set new position
	recalculateBounds(entity, *entity->refBounds); // Recalculate collision box
}

void drawEntity(Entity* entity){
	DrawModel(*entity->model, entity->position, entity->scale, entity->tint);
	DrawModelWires(*entity->model, entity->position, entity->scale, entity->border);
	DrawModel(*entity->model, Vector3Add(entity->position, defaultShadow.offset), entity->scale, defaultShadow.color);
	//DrawBoundingBox(entity->bounds, YELLOW); //DEBUG
}

void drawEntityPool(Entity* pool, int size){
	for (int i = 0; i < size; i++)
		{
			if (pool[i].active){
				// Draw entity + border + shadow
				drawEntity(&pool[i]);
			}
		}
}

int getEntityFromPool(Entity* pool, int size){
	int i = 0;
	
	while (i < size)
	{
		// Entity not active =  available entity
		if (!pool[i].active)
		{
			pool[i].active = true; // Mark entity as used (render & interactive)

			return i;
		}

		i++; // Thx my pana Dubledoh!
	}

	return -1;
}