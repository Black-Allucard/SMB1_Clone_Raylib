#pragma once
#include "../headers/projectile.h"
Projectile::Projectile(bool is_friendly, Vector2 cord, Vector2 speed, Rectangle r) : ent(cord,speed,{r.width,r.height},1,1) {
	friendly = is_friendly;
	timer = 0;
}
float Projectile::get_time() {
	return float(timer);
}
bool Projectile::is_friendly() {
	return friendly;
}
void Projectile::progress_time(float time) {
	timer += time;
}
void Projectile::acc(float dt) {
	if (get_speed().y < MAX_SPEED * 6) {
		set_speedy(get_speed().y + 2400 * dt);
	}
	else {
		set_speedy(MAX_SPEED * 6);
	}
	if (!started_cycle) {
		set_speedx(0);
		set_speedy(0);
	}
}