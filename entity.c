#include "entity.h"

void initEntity(Entity* entity, Model* model, BoundingBox bounds, Color shadow, float shadowAlpha, Color tint,
				Color border, Vector3 position, Vector3 shadowOffset, float speed, bool active){
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
	for (int i = 0; i < size; i++)
	{
		initEntity(&(pool[i]), model, bounds, shadow, shadowAlpha, tint, border, 
			position, shadowOffset, speed, false);
	}
}

void recalculateBounds(Entity* entity, BoundingBox referenceBox){
	entity->bounds.max = Vector3Add(referenceBox.max, entity->position);
	entity->bounds.min = Vector3Add(referenceBox.min, entity->position);
	//TraceLog(LOG_DEBUG, "Bounds min: (%.2f, %.2f, %.2f)");
}