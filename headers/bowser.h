#pragma once
#include "ent.h"

class Bowser : public ent {
private:
	float jump_timer = 1.5;
	float fire_timer = 3;
	float open_timer = 1.5;
	float dir_timer = 1.5;
	float dead_timer = 0;
	float frame = 0;
	bool jump = false;
	bool fire = false;
	bool open = false;
	bool flipped = false;
	Rectangle hitboxes[2];
	direc dir = left;
	Vector2 O_pos;

public:
	int get_dir();
	Bowser(Vector2 cord,direc s_dir);
	void update(float dt);
	void ProcessCollision(collidable* other, float dt);
	void draw(Texture2D* t ,float dt);
	void acc(float dt);
	void ResetHitboxes();
	obj_type get_type();
	std::unique_ptr<Rectangle[]> get_HitBoxes();
	

};