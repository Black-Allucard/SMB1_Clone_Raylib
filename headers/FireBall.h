#pragma once
#include "projectile.h"

class FireBall : public Projectile {
private:
	bool exploded;
	float frame;
public:
	FireBall(Vector2 cord, direc dir);
	void update(float dt);
	void draw(Texture2D* t, float dt);
	obj_type get_type();
	void ProcessCollision(collidable* other, float dt);
	Rectangle get_HitBox();
};

class SpinBall : public Projectile {
private:
	float frame;
	float angle;
	Vector2 spin_centre;
	float radius;
	direc turn_dir;
public:
	SpinBall(Vector2 cord, direc spin_dir,Vector2 centre);
	SpinBall();
	void update(float dt);
	void draw(Texture2D* t, float dt);
	obj_type get_type();
	void ProcessCollision(collidable* other, float dt);
	Rectangle get_HitBox();

};

class FireBar : public ent {
private:
	SpinBall My_Balls[6];
public:
	FireBar(Vector2 cord, direc spin_dir, float angle);
	void update(float dt);
	void draw(Texture2D* t, float dt);
	obj_type get_type();
	void ProcessCollision(collidable* other, float dt);
	
};

class DragonBreath : public Projectile {
private:
	float frame = 0;
	bool homing = false;
public:
	DragonBreath(Vector2 cord, direc dir);
	void update(float dt);
	void draw(Texture2D* t, float dt);
	obj_type get_type();
	void ProcessCollision(collidable* other, float dt);
	Rectangle get_HitBox();

};