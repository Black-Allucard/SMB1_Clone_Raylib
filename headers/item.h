#pragma once
#include "ent.h"



class Item : public ent {
private:
	items I_T;
	bool is_dead;
	float item_frame;
public:
	Vector2 O_pos;
	Item(items it, Vector2 cord,int color);
	items get_Itype();
	obj_type get_type();
	void ProcessCollision(collidable* other, float dt);
	void draw(Texture2D* t, float dt);
	Vector2 GetPosition();
	Vector2 GetSize();
	bool rising;
	void update(float dt);


	void acc(float dt);
	
};
