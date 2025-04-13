#pragma once
#include "ent.h"
class Pirahna_Plant : public ent {
private:
	bool still = false;
	float still_timer = 0;
	float frame = 0;
	bool blocked = false;
	Vector2 O_pos;
public:
	Pirahna_Plant(Vector2 cord, int color);
	obj_type get_type();
	Rectangle  get_HitBox();
	void draw(Texture2D* t, float dt);
	void update(float dt);
	void ProcessCollision(collidable* other, float dt);

};
