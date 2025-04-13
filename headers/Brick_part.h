#pragma once
#include "projectile.h"

class Brick_part: public Projectile{
private:
	float frame;
public:
	obj_type get_type();
	Brick_part(Vector2 cord,direc dir, int color,Vector2 speed);
	void update(float dt);
	void draw(Texture2D* t,float dt);

};