#pragma once
#include "projectile.h"

class SpinCoin : public Projectile {
private:
	float frame;
	bool spawned = false;
public:
	SpinCoin(Vector2 cord);
	void draw(Texture2D* t, float dt);
	void update(float dt);
	obj_type get_type();
	~SpinCoin();
};