#pragma once
#include "Enemy.h"

class goomba : public Enemy {
public:
	goomba(Vector2 cord,direc dir,int color,bool flying);
	void ProcessCollision(collidable* other,float dt);
	obj_type get_type();
	void squash();
	void update(float dt);
	void acc(float dt);
	void draw(Texture2D*t,float dt);
	bool is_dead();
	Rectangle get_HitBox();
	float g_frame;
	void kill();
};