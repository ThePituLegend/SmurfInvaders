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

void initEntity(Entity* entity, Model* model, BoundingBox bounds, Color shadow, float shadowAlpha, Color tint,
				Color border, Vector3 position, Vector3 shadowOffset, float speed, bool active){
	
	// Inits the entity struct, element by element
	entity->model = model;
	entity->bounds = bounds;
	entity->shadow = shadow;
	entity->shadow.a = shadowAlpha;
	entity->tint = tint;
	entity->border = border;
	entity->position = position;
	entity->shadowOffset = shadowOffset;
	entity->speed = speed;
	entity->active = active;
}

void initEntityPool(Entity* pool, int size, Model* model, BoundingBox bounds, Color shadow, float shadowAlpha, Color tint,
				Color border, Vector3 position, Vector3 shadowOffset, float speed){
	
	// Iterates through the given entity array
	for (int i = 0; i < size; i++)
	{
		// Inits each entity
		initEntity(&(pool[i]), model, bounds, shadow, shadowAlpha, tint, border, 
			position, shadowOffset, speed, false);
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