#pragma once
#include "../headers/Spinning_coin.h"

SpinCoin::SpinCoin(Vector2 cord) : Projectile(0, cord, {0, -1000 }, {cord.x,cord.y,64,64}) {
	frame = 0;
	spawn.type = none;
}
void SpinCoin::draw(Texture2D* t, float dt) {
	frame += 15 * dt;
	if (frame >= 4) {
		frame = 0;
	}
	DrawTexturePro(*t, { float(int(frame) * 16),40,16,16 }, get_rect(), { get_rect().width / 2,get_rect().height / 2 }, 0, RAYWHITE);
}
void SpinCoin::update(float dt) {
	move(dt);
	progress_time(dt);
}
obj_type SpinCoin::get_type() {
	if (get_time() > 0.72 && !spawned) {
		spawned = true;
		score_points(1);
	}
	if (get_time() > 0.75) {
		return none;
	}
	else {
		return projectile;
	}

}
SpinCoin::~SpinCoin() {

}