#pragma once
#include "../headers/flag.h"
#include "../headers/collisions.h"
obj_type Flag::get_type() {
	return block;
}
void Flag::update(float dt) {
	move(dt);
}
void Flag::draw(Texture2D* t, float dt) {
	DrawTexturePro(*t, { 64 + float(16 * c),96,16,16 }, get_rect(), { get_rect().width / 2,get_rect().height / 2 }, 0, RAYWHITE);
}

obj_type Flag_pole::get_type(){
	return pole;
}
void Flag_pole::update(float dt){
	if (fabsf(f.get_rect().y - (get_rect().y + get_rect().height / 2 - f.get_rect().height / 2)) <= 5) {
		f.set_cord(f.get_rect().x, get_rect().y + get_rect().height / 2 - f.get_rect().height / 2);
		f.set_speedx(0);
		f.set_speedy(0);
		flag_down = true;
	}
	else {
		f.set_speedx(get_speed().x);
		f.set_speedy(get_speed().y);
		f.move(dt);
	}
}
void Flag_pole::draw(Texture2D* t, float dt){
	f.draw(t, dt);
}
Flag_pole::Flag_pole(Vector2 cord, int flag_color) : ent(cord,{0,0},{2*4,152*4},0,0) {
	flagSpeed = { 0,0 };
	f.c = flag_color;

	set_health(1);
	f.set_cord(cord.x - 32 , cord.y - 152 * 2 + 72);
	f.set_rect(get_cord().x, get_cord().y, 64, 64);
	f.set_speedx(0);
	f.set_speedy(0);
}
void Flag_pole::ProcessCollision(collidable* other, float dt){
	Rectangle r = get_rect();
	Rectangle r2 = other->get_HitBox();

	switch (other->get_type()) {
	case(player): {
		if (AABB(r,r2)) {
			if (other->get_speed().x == 0 && other->get_speed().y == 0) {
				flagSpeed = { 0,0 };
				other->set_speedx(0);
				other->set_speedy(0);
			}
			else {
				other->set_speedx(0);
				other->set_speedy(200);
				flagSpeed = { 0,200 };
			}
			
		}
		break;
	}
	}
}