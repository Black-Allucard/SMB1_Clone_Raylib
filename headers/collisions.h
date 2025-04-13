#pragma once
#include "Collider.h"
#include <stdio.h>

extern float Elispson;
extern Vector2 cn;
extern Vector2 cp;
extern float ct;
extern Vector2 prop_cp[2];


struct ray {
	Vector2 size;
	Vector2 point;

};
struct double_vector {
	double x;
	double y;
};

bool AABB(const Rectangle sourceRect, const Rectangle targetRect);
bool RayVsRect2D(const Vector2 ray_origin, const Vector2 ray_dir, const Rectangle targetRect, Vector2* contact_point, Vector2* contact_normal, float* near_contact_time, Vector2 probableContactPoints[]);
bool DynamicRectVsRect(const Rectangle sourceRect, const Vector2 sourceRectVelocity, const Rectangle targetRect, Vector2* contact_point, Vector2* contact_normal, float* near_contact_time, Vector2 probableContactPoints[],float dt);
void Resolve_AABB(const Rectangle* origin, const Rectangle* target, Vector2* cp, Vector2* cn);