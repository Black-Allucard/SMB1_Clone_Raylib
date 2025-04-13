#pragma once

#include "enemy.h"
class koopa : public Enemy {
public:
	float squash_time = 4;
	float g_frame;
	bool show;
	float frame_time;
	Rectangle get_HitBox();
	koopa(Vector2 cord, direc dir,int color,bool flying);
	obj_type get_type();
	direc dir;
	direc fly_dir = up; 
	void ProcessCollision(collidable* other, float dt);
	void update(float dt);
	void acc(float dt);
	void draw(Texture2D* t, float dt);
	bool is_dead();

};
