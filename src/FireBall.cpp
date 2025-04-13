#pragma once
#include "../headers/FireBall.h"
#include "../headers/collisions.h"
#include <cmath>
DragonBreath::DragonBreath(Vector2 cord, direc dir) : Projectile(0,cord, Vector2{ float(200 * dir),0 }, Rectangle{ cord.x,cord.y,96,32 }) {
	set_speedx(float(450 * dir));
	frame = 0;
	set_health(1);
	
}  
void DragonBreath::update(float dt){
	set_speedy(0);

	move(dt);
}
void DragonBreath::draw(Texture2D* t, float dt) {
	Rectangle r = get_rect();
	//Rectangle r2 = get_HitBox();
	frame += 7 * dt;
	if (frame >= 2) {
		frame = 0;
	}
	DrawTexturePro(*t, { float(24 * int(frame)),16,24*(-1)*get_speed().x/fabsf(get_speed().x),8}, r, {r.width / 2,r.height / 2}, 0, WHITE);
	//DrawRectanglePro(r2, { r2.width / 2,r2.height / 2 }, 0, GREEN);
}
obj_type DragonBreath::get_type() {
	return projectile;
}
void DragonBreath::ProcessCollision(collidable* other, float dt) {

}
Rectangle DragonBreath::get_HitBox() {
	Rectangle r = get_rect();
	r.x += r.width / 4;
	r.y += r.height / 4;
	r.width *= 0.125;
	r.height *= 0.5;
	return r;
}
SpinBall::SpinBall() : Projectile(0,{0,0},{0,0},{0,0,64,64}) {
	friendly = false;
}
SpinBall::SpinBall(Vector2 cord, direc spin_dir, Vector2 centre) : Projectile(0,cord,{0,0},{cord.x,cord.y,64,64}) {
	float y_dist = cord.y - centre.y;
	float x_dist = cord.x - centre.x;
	if (y_dist == 0 && x_dist == 0) {
		angle = 0;
	}
	else if (y_dist == 0) {
		if(x_dist > 0){
			angle = 0;
		}
		if (x_dist < 0) {
			angle = 180;
		}
	}
	else if (x_dist == 0) {
		if (y_dist > 0) {
			angle = -90;
		}
		if (y_dist < 0) {
			angle = 90;
		}
	}
	else {
		angle = atan(y_dist / x_dist) * 180 / PI;
	}
	angle = angle - remainderf(angle,12);
	spin_centre = centre;
	turn_dir = spin_dir;
	frame = 0;
	radius = sqrt((cord.x - centre.x)* (cord.x - centre.x) + (cord.y - centre.y) * (cord.y - centre.y));
	friendly = false;
	set_health(1);
}
void SpinBall::update(float dt) {
	if (frame_count % 10 == 0 && frame_count > 0) {
		float a = angle;
		set_cord(spin_centre.x + radius * cos(a*PI/180), spin_centre.y + radius * sin(a * PI / 180));
		a += 12 * turn_dir;
		angle = a;
	}
}
void SpinBall::draw(Texture2D* t, float dt) {
	frame += 12*dt;
	if (frame >= 4) {
		frame = 0;
	}
	DrawTexturePro(*t, { 0,24,16,16 }, get_rect(), Vector2{ get_rect().width / 2,get_rect().height / 2 }, -90 * int(frame), RAYWHITE);
}
obj_type SpinBall::get_type() {
	return projectile;
}
void SpinBall::ProcessCollision(collidable* other, float dt) {

}
Rectangle SpinBall::get_HitBox() {
	Rectangle r = get_rect();
	r.width /= 2;
	r.height /= 2;
	return r;
}

FireBar::FireBar(Vector2 cord, direc spin_dir, float angle) : ent(cord,Vector2{0,0}, Vector2{384,384},false,0 ) {
	for (int i = 0; i < 6; i++) {
		Vector2 new_Cord;
		new_Cord.x = cord.x + 32 * i * cos(angle * PI / 180);
		new_Cord.y = cord.y + 32 * i * sin(angle * PI / 180);
		My_Balls[i] = SpinBall(new_Cord,spin_dir, cord);
	}
}

void FireBar::update(float dt) {
	for (int i = 0; i < 6; i++) {
		My_Balls[i].update(dt);
	}

}

void FireBar::draw(Texture2D* t, float dt) {
	for (int i = 0; i < 6; i++) {
		My_Balls[i].draw(t,dt);
	}
}

obj_type FireBar::get_type() {
	return projectile;
}

void FireBar::ProcessCollision(collidable* other, float dt) {
	switch (other->get_type()) {
	case(player): {
		for (int i = 0; i < 6; i++) {
			other->ProcessCollision(&My_Balls[i],dt);
		}
		break;
	}
	}
}




FireBall::FireBall(Vector2 cord, direc dir) : Projectile(false,cord,{float(800*dir),0},{cord.x,cord.y,64,64}) {
	frame = 0;
	friendly = true;
	exploded = false;
}

void FireBall::draw(Texture2D* t, float dt) {
	if (exploded) {
		frame += 18 * dt;
	}
	else {
		frame +=  12 * dt;
	}



	if (frame >= 4 || (exploded && frame >= 3)) {
		if (exploded) {
			frame = 2;
		}
		else {
			frame = 0;
		}
	}

	if (exploded) {
		DrawTexturePro(*t, { float(16*int(frame)),0,16,16 }, get_rect(), Vector2{ get_rect().width / 2,get_rect().height / 2 },0, RAYWHITE);
	}
	else{
		DrawTexturePro(*t,{0,24,16,16},get_rect(),Vector2{get_rect().width/2,get_rect().height/2},-90*int(frame),RAYWHITE);
	}
}

void FireBall::update(float dt) {
	if (!exploded) {
		move(dt);
	}
	else if (exploded) {
		progress_time(dt);
	}
}

obj_type FireBall::get_type(){
	if (exploded && get_time() > 0.2) {
		return none;
	}
	else {
		return projectile;
	}
}
Rectangle FireBall::get_HitBox() {
	Rectangle r = get_rect();
	r.width /= 2;
	r.height /= 2;
	return r;

}
void FireBall::ProcessCollision(collidable* other, float dt) {
	Vector2 speed = get_speed();
	Rectangle r2 = other->get_rect();
	Rectangle r = get_HitBox();
	
	switch (other->get_type()) {
		case(block): {
			bool col = DynamicRectVsRect(r, { speed.x * dt, speed.y * dt }, r2, &cp, &cn, &ct, prop_cp, dt) && !exploded && other->get_health() > 0 && 1.0f - ct <= 1 && other->get_health() > 0;
			if (!other->hidden) {
				if (col) {
					if (cn.x != 0) {
						exploded = true;
					}
					else if (cn.y != 0) {
						if (cn.y == 1) {
							exploded = true;
							frame = 0;
						}
						else if (cn.y == -1) {
							r.y = cp.y;
							speed.y = -300;
							set_cord(r.x, r.y);
							set_speedy(speed.y);
						}
					}
				}
				else if (r.x >= r2.x - r2.width / 2 && r.x <= r2.x + r2.width / 2 && r.y <= r2.y + r2.height / 2 && r.y >= r2.y - r2.height / 2 && !exploded) {
					exploded = true;
					frame = 0;
				}
			}

			break;
		}

		case(enemy): {
			r2 = other->get_HitBox();
			r = get_HitBox();
			bool col = DynamicRectVsRect(r, { speed.x * dt, speed.y * dt }, r2, &cp, &cn, &ct, prop_cp,dt) && other->get_health() > 0;
			if (col && !exploded) {
				exploded = true;
				frame = 0;
				other->hit.hit = true;
				other->hit.origin = projectile;

			}
			break;
		}

		case(plant): {
			r2 = other->get_HitBox();
			r = get_HitBox();
			bool col = DynamicRectVsRect(r, { speed.x * dt, speed.y * dt }, r2, &cp, &cn, &ct, prop_cp, dt) && other->get_health() > 0;
			if (col && !exploded) {
				exploded = true;
				frame = 0;
				other->hit.hit = true;
				other->hit.origin = projectile;
				other->score_points(1);
			}
			break;
		}
		case(boss): {
			for (int i = 0; i < 2; i++) {
				r2 = other->get_HitBoxes()[i];
				r = get_HitBox();
				bool col = DynamicRectVsRect(r, { speed.x * dt, speed.y * dt }, r2, &cp, &cn, &ct, prop_cp, dt) && other->get_health() > 0;
				if (col && !exploded) {
					exploded = true;
					frame = 0;
					other->hit.hit = true;
					other->hit.origin = projectile;

				}
			}
			break;
		}

		}
	
}