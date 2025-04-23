#pragma once
#include "../headers/plant.h"
#include "../headers/collisions.h"
Pirahna_Plant::Pirahna_Plant(Vector2 cord, int color) : ent(cord, { 0,-100 }, { 64,96 }, true, 0) {
	set_rect({ cord.x,cord.y,64,96 });
	c = color;
	set_health(1);
	still = false;
	set_speedy(-80);
	O_pos = cord;
	blocked = false;
	frame = 0;
	hit.hit = false;
	still = true;
}
obj_type Pirahna_Plant::get_type() {
	if (get_health() > 0) {
		return plant;
	}
	return none;
}
Rectangle Pirahna_Plant::get_HitBox() {
	Rectangle r = get_rect();
	r.width *= 0.75;
	r.height *= 0.4;
	r.y += 10;
	return r;
}
void  Pirahna_Plant::draw(Texture2D* t, float dt) {
	Rectangle r = get_rect();
	frame += 4*dt;
	if (frame >= 2) {
		frame = 0;
	}

	DrawTexturePro(*t, { float(64 + 32*c + int(frame)*16),176,16,24 }, r, { r.width / 2,r.height / 2 }, 0, RAYWHITE);
    r = get_HitBox();
	DrawRectangleLines(int(r.x -r.width/2), int(r.y - r.height / 2),r.width,r.height,WHITE);
	
}
void  Pirahna_Plant::update(float dt) {
	if (hit.hit) {
		hit.hit = false;
		set_health(0);
		
	}
	if (still) {
		if (!blocked || get_cord().y < O_pos.y) {
			still_timer -= dt;
			if (still_timer <= 0) {
				still_timer = 3;
				still = false;
			}
		}
	}
	else {
		float y_cord = get_cord().y;
		float y_speed = get_speed().y;
		if (y_cord >= O_pos.y + 5 && y_speed > 0) {
			still = true;
			set_speedy(y_speed * -1);
			set_cord(get_cord().x, O_pos.y + 4);
		}
		else if (y_cord <= O_pos.y - get_rect().height + 4 && y_speed < 0) {
			still = true;
			set_speedy(y_speed * -1);
			set_cord(get_cord().x, O_pos.y - get_rect().height + 4);
		}
		move(dt);
	}
	
	blocked = false;

}
void Pirahna_Plant::ProcessCollision(collidable* other, float dt) {
	Rectangle r = other->get_rect();
	Rectangle r2 = get_rect();
	Vector2 speed = other->get_speed();
	switch (other->get_type()) {
	case(player): {
		if (DynamicRectVsRect(r, { speed.x * dt,speed.y * dt }, r2, &cp, &cn, &ct, prop_cp, dt)) {
			if (other->is_warping()) {
				set_cord(O_pos.x, O_pos.y);
				still = true;
			}
		
		}
		break;
	}
	case(block): {
		bool col = DynamicRectVsRect(r, { speed.x * dt,speed.y * dt }, r2, &cp, &cn, &ct, prop_cp, dt);
		if (AABB(r,r2) && still && other->In_collision) {
			blocked = true;
		}

		break;
	}
	}
}