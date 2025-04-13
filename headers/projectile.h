#pragma once
#include "ent.h"

class Projectile : public ent{
private:
	float timer;
	
public:
	bool friendly;
	Projectile(bool is_friendly, Vector2 cord, Vector2 speed, Rectangle r);
	bool is_friendly();
	float get_time();
	void progress_time(float time);
	void acc(float dt);
};

