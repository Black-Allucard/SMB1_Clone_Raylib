#pragma once
#include "ent.h"
enum enemies {
	Koopa,
	Goomba,
	PiranhaPlant,
	HammerBro,
	BulletBill,
	CheepCheep,
	Buzzybeetle,
	Spiny,
	Lakitu,

};
class Enemy:public ent {
public :
	bool squashed = false;
	bool can_squash;
	bool can_hit = true;
	bool is_flying = false;
	float dead_time; //time for the enemy to dissapear in frame_rules
	int color;
	bool grounded = false;
	Vector2 O_Pos;

	Enemy(Vector2 cord,Vector2 size, Vector2 speed, direc dir,int hp,int c_s,int c,bool flying);
	virtual void squash();
	virtual void lose_health();
	void flip();
	Texture2D get_texture();
	void Process_player_collision(collidable* other,float dt);
};