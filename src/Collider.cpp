#pragma once

#include "../headers/collider.h"
float bridge_time = 0.1;
bool fixed_cam = false;
int score = 0;
int frame_count = 0;
int one_ups = 0;
bool flag_down = false;
int block_frame = 0;
bool axe_struck = false;
Vector2 cam_cord = { 0,0 };
Vector2 player_cord = { 0,0 };
bool bowser_died = false;
bool new_frame_rule() {
	if (frame_count % frame_rule == 0 && frame_count > 0) {
		return true;
	}
	else {
		return false;
	}
}
std::unique_ptr<Rectangle[]> collidable::get_HitBoxes() {
	auto boxes = std::make_unique<Rectangle[]>(2);
	return boxes;
}
void collidable::score_points(int streak) {
	spawn.type = text;
	
	switch (streak) {
	case(0):{
		spawn.specify = score_100;
		score += 100;
		break;
	}
	case(1): {
		score += 200;
		spawn.specify = score_200;
		break;
	}
	case(2): {
		spawn.specify = score_400;
		score += 400;
		break;
	}
	case(3): {
		spawn.specify = score_500;
		score += 500;
		break;
	}
	case(4): {
		spawn.specify = score_800;
		score += 800;
		break;
	}
	case(5): {
		spawn.specify = score_1000;
		score += 1000;
		break;
	}
	case(6): {
		spawn.specify = score_2000;
		score += 2000;
		break;
	}
	case(7): {
		spawn.specify = score_4000;
		score += 4000;
		break;
	}
	case(8): {
		spawn.specify = score_5000;
		score += 5000;
		break;
	}
	case(9): {
		spawn.specify = score_8000;
		score += 8000;
		break;
	}
	default: {
		spawn.specify = score_one_up;
		one_ups += 1;
		break;
	}
	}
}
items collidable::what_item() {
	return used;
}

Rectangle collidable::get_rect(){
	Rectangle r = {0,0,1,0};

	return(r);
}

obj_type collidable::get_type() {
	return(none);

}
void collidable::update(float dt) {

}
Vector2 collidable::GetPosition() {
	Vector2 v = { 20,20 };
	return v;
}
bool collidable::Fixed_cam() {
	return false;
}
bool collidable::is_dynamic() {
	return false;
}
void collidable::set_dynamic(bool state) {

}
bool collidable::is_warping() {
	return false;
}
void collidable::ProcessCollision(collidable* other,float dt) {

}
void collidable :: ResolveCollision(collidable* other, float dt) {

}
void collidable::squash() {

}
bool collidable::side_warp() {
	return false;
}
void collidable::lose_health() {
	health -= 1;
}


Vector2 collidable::get_speed() {
	return{ 0,0 };
}

void collidable::set_health(float hp) {
	health = hp;
}

float collidable::get_health() {
	return health;
}
bool collidable::is_friendly() {
	return true;
}

void collidable::draw(Texture2D* t,float dt) {
	DrawRectanglePro(get_rect(), Vector2{ get_rect().width / 2,get_rect().height / 2 }, 0, RED);
}

float collidable::calc_int_min(int axis, float dt) {
	return 0;
}
float collidable::calc_int_max(int axis, float dt) {
	return 0;
}
void collidable::set_rect(Rectangle r) {
	
}
bool collidable::is_dead() {
	return false;
}
void collidable::kill() {
	delete(this);
}
Rectangle collidable::get_HitBox() {
	return{ 0,0,0,0 };
}
void collidable::set_speedx(float speed_x) {

}
void collidable::set_speedy(float speed_y) {

}
Vector2 collidable::GetCamPos() {
	return { 0,0 };
}
setting collidable::GetWarpSetting() {
	return sky;
}