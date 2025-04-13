#pragma once
#include "ent.h"
#include "collisions.h"

typedef enum Block_type {
	static_block,
	empty_block,
	brick,
	item_block,
	platform,
	axe,
	bridge,
	death,
	stop
}block_type;

class Block : public ent {
private:
	block_type B_T;
	Vector2 O_pos;
	items block_item = used;
	float bounce_time = 0;
	float active_time = 0;
	bool top = false;;
	bool bounce;
	bool started_timer = false;
public:
	block_type getB_type();
	obj_type get_type();
	void ProcessCollision(collidable* other,float dt);
	void draw(Texture2D* t, float dt);
	Vector2 GetPosition();
	Vector2 GetSize();
	Block(block_type b,Rectangle rect,items item,bool is_hidden,bool is_on_top,int color,bool is_edge);
	items what_item();
	void update(float dt);
	void kill();
	bool is_dead();
	void acc(float dt);
	void ResolveCollision(collidable* other, float dt);
	Rectangle getHitBox();
	
};