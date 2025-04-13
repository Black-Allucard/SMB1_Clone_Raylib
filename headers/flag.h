#pragma once
#include "ent.h"
class Flag : public ent {
public:
	bool has_pole;
	obj_type get_type();
	void update(float dt);
	void draw(Texture2D* t, float dt);
};
class Flag_pole : public ent {
private:
	Flag f;
	Vector2 flagSpeed = { 0,0 };
	
public:
	obj_type get_type();
	void update(float dt);
	void draw(Texture2D* t, float dt);
	Flag_pole(Vector2 cord,int flag_color);
	void ProcessCollision(collidable* other, float dt);
};