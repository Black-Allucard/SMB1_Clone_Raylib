#pragma once
#include "collider.h"

#define MAX_SPEED (300)

typedef class ent: public collidable{
private:
	Vector2 coordinates;
	Vector2 speed;
	Rectangle rec;
	bool dynamic;
	float gravity;
public:
	void move(float dt);
	virtual void acc(float dt);
	ent();
	virtual int get_dir();
	ent(Vector2 cord, Vector2 s, Vector2 size, bool d, float g);
	Vector2 get_cord();
	void set_cord(float x, float y);
	void set_speedx(float speed_x);
	void set_speedy(float speed_y);
	Vector2 get_speed();
	void set_rect(float x, float y, float width, float height);
	virtual Rectangle get_rect();
	void set_rect(Rectangle r);
	bool is_dynamic();
	void set_dynamic(bool state);
	float calc_int_min(int axis, float dt);
	float calc_int_max(int axis, float dt);

}ent;

