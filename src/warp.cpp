#pragma once
#include "../headers/Warp.h"
#include "../headers/collisions.h"
Vector2 Warp::GetPosition() {
	return warp_to;
}
 
obj_type Warp::get_type() {
	return warp;
}
Vector2 Warp::GetCamPos() {
	return cam_pos;
}
setting Warp::GetWarpSetting() {
	return new_setting;
}
Warp::Warp(Vector2 warp_cord, Vector2 warp_cam_cord, Rectangle rect, setting set, bool fix_cam, bool on_side, Vector2 Warp_speed) : ent({ rect.x,rect.y }, { 0,0 }, { rect.width,rect.height },0,0) {
	warp_to = warp_cord;
	cam_pos = warp_cam_cord;
	new_setting = set;
	set_rect(rect);
	cam_fixed = fix_cam;
	set_speedx(Warp_speed.x);
    set_speedy(Warp_speed.y);
	side = on_side;
	moving_ent = false;
}
bool Warp::Fixed_cam() {
	return cam_fixed;
}
bool Warp::side_warp() {
	return side;
}
void Warp::ProcessCollision(collidable* other, float dt) {
	
	switch (other->get_type()) {
	case(plant): {
		Rectangle r = get_rect();
		Rectangle r2 = other->get_rect();
		if (AABB(r, r2)) {
			if (r.y - r.height / 2 > r2.y - r2.height / 2) {
				has_plant = true;
			}
		}
		break;
	}
	}
}
void Warp::update(float dt) {
	has_plant = false;
}