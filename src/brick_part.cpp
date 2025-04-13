#pragma once
#include "../headers/Brick_part.h"

Brick_part::Brick_part(Vector2 cord, direc dir, int color, Vector2 speed) : Projectile(true, cord, Vector2{ float(fabsf(speed.x) * dir), float(speed.y) }, Rectangle{ cord.x,cord.y,64,64 }) {
	frame = 0;
	c = color;
}
void Brick_part::update(float dt) {
	move(dt);
	progress_time(dt);
}
void Brick_part::draw(Texture2D* t,float dt) {
	
	
	frame += 10 * dt;
	
	if (frame >= 4) {
		frame = 0;
	}

	DrawTexturePro(*t, { float(48 + 16*int(c)),0,16,16 }, get_rect(), { get_rect().width / 2,get_rect().height / 2 }, -45 * int(frame), RAYWHITE);
}
obj_type Brick_part::get_type() {
	if (get_time() >= 6) {
		return none;
	}
	else {
		return projectile;
	}
}