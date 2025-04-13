#pragma once
#include "../headers/ent.h"

void ent::move(float dt) {
	coordinates.x += speed.x * dt + carry_speed.x * dt;
	coordinates.y += speed.y * dt + carry_speed.y * dt;
	set_rect(coordinates.x, coordinates.y, get_rect().width, get_rect().height);
	
 }
void ent::acc(float dt) {

}

int ent::get_dir() {
	return 0;
}

bool ent::is_dynamic() {
	return(dynamic);
}

void ent::set_dynamic(bool state) {
	dynamic = state; 
}

ent::ent(Vector2 cord, Vector2 s, Vector2 size, bool d, float g) {
	
	coordinates.x = cord.x;
	coordinates.y = cord.y;
	speed = s;
	rec = {cord.x,cord.y,size.x,size.y};
	dynamic = d;
	if (dynamic) {
		gravity = g;
	}
	else {
		gravity = 0;
	}
	
	moving_ent = true;
}
ent::ent() {
	coordinates.x = 1;
	coordinates.y = 1;
	speed = Vector2{0,0};
	rec = Rectangle{1,1,1,1};
	dynamic = false;
	if (dynamic) {
		gravity = 1;
	}
	else {
		gravity = 0;
	}
}

Vector2 ent::get_cord() {
	return { coordinates.x, coordinates.y };
}
void ent::set_cord(float x, float y){
	coordinates = Vector2{x,y};
	set_rect(coordinates.x, coordinates.y, get_rect().width, get_rect().height);

}
Vector2 ent::get_speed() {
	return speed;
}
void ent::set_speedy(float speed_y) {
	speed.y = speed_y;
}
void ent::set_speedx(float speed_x) {
	speed.x = speed_x;
}
void ent::set_rect(float x, float y, float width, float height) {
	rec = Rectangle{ x,y,width,height };
	
}
Rectangle ent::get_rect() {
	return rec;
}
void ent::set_rect(Rectangle r) {
	rec = r;
}

float ent::calc_int_min(int axis,float dt){
	float min = 0;
	switch (axis) {
	case(0): {
		min = get_cord().x - get_rect().width / 2;
		if (get_speed().x < 0) {
			min += get_speed().x * dt;
		}
		break;

	}
	case(1): {
		min = get_cord().y - get_rect().height / 2;
		if (get_speed().y < 0) {
			min += get_speed().y * dt;
		}
		break;

	}
	}
	return min;
}
float ent::calc_int_max(int axis,float dt){
	float max = 0;
	switch (axis) {
	case(0): {
		max = get_cord().x + get_rect().width / 2;
		if (speed.x >= 0) {
			max += speed.x*dt;
		}
		break;
	}
	case(1): {
		max = get_cord().y + get_rect().height / 2;
		if (speed.y >= 0) {
			max += speed.y * dt;
		}
		break;
		
	}
	}
	return max;
}