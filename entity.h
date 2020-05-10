#pragma once

#include "raylib.h"
#include "raymath.h"

typedef struct Entity
{
	Model* model;
	BoundingBox bounds;
	Vector3 position;
	Vector3 shadowOffset;
	Color shadow;
	Color tint;
	Color border;
	float speed;
	bool active;
} Entity;

typedef Entity Player;
typedef Entity Bullet;
typedef Entity Enemy;

void initEntity(Entity* entity, Model* model, BoundingBox bounds, Color shadow, float shadowAlpha, Color tint,
				Color border, Vector3 position, Vector3 shadowOffset, float speed, bool active);

void initEntityPool(Entity* pool, int size, Model* model, BoundingBox bounds, Color shadow, float shadowAlpha, Color tint,
				Color border, Vector3 position, Vector3 shadowOffset, float speed);

void recalculateBounds(Entity* entity, BoundingBox referenceBox);