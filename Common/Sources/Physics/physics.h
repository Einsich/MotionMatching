#pragma once
#include "GameObject/game_object.h"
#include "Colliders/collider.h"
#include "Colliders/collision.h"
#include "ray.h"
//go + collider || go + collider + body(static)
void add_collider(GameObjectPtr go, ColliderPtr collider);

Collision ray_cast(const Ray &ray);