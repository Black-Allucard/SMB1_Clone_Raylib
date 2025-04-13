#pragma once
#include "../headers/enemy.h"
#include "../headers/collisions.h"
void Enemy::squash() {
	Sound stomp = LoadSound("../assets/audio/stomp.wav");
	SetSoundVolume(stomp, 10);
	
	PlaySound(stomp);
	if (!is_flying) {
		squashed = true;
		set_speedx(0);
	}
	is_flying = false;
	
}
void Enemy::lose_health() {
	set_health(get_health()-1);
}
Enemy::Enemy(Vector2 cord, Vector2 size, Vector2 speed,direc dir, int hp, int c_s, int c, bool flying) : ent(cord,speed, size, true, 1) {
	color = c;
	set_health(hp);
	can_squash = c_s;
	dead_time = 0;
	squashed = false;
	player__col = true;
	O_Pos = cord;
	is_flying = flying;
}
void Enemy::flip() {
	lose_health();
	set_speedx(50);
	set_speedy(-200);
	
}
void Enemy::Process_player_collision(collidable* other,float dt) {
	Vector2 speed = other->get_speed();
	Rectangle r = other->get_HitBox();
	Rectangle r2 = get_HitBox();
	bool col = DynamicRectVsRect(r, { speed.x * dt,speed.y * dt }, r2, &cp, &cn, &ct, prop_cp,dt);
	if (col && other->get_health() > 0 && get_health() > 0) {
		if (player__col && !hit.hit && !hit.hit_up) {
			player__col = false;
			

			float remainingtime = 1.0f - ct;

			if (AABB(r, r2) && fabsf(remainingtime) > 1) {
				Resolve_AABB(&r, &r2, &cp, &cn);
				remainingtime = 1;
				if (cn.y != 0) {
					r.y = cp.y;
				}
				if (cn.x != 0) {
					r.x = cp.x;
				}
			}
			else if (!AABB(r, r2) && fabsf(remainingtime) > 1) {
				remainingtime = 0;
				cn.x = 0;
				cn.y = 0;

			}

			if ((cn.x != 0 && get_speed().x == 0) || (cn.y == -1 )) {
				if (cn.y == -1) {
					hit.hit_up = true;
					other->set_speedy(-400);
				}

				
				if (squashed && cn.x != 0) {
					if(other->streak > 0){
						score_points(2);
					}
					else {
						score_points(3);
					}
				}
				else {
					score_points(other->streak);
				}

				
				other->streak++;
				hit.player_pow = other->pow;
				hit.hit = true;
						
				hit.origin = player;

				if (cn.x > 0 || r.x > r2.x) {
					angle = left;
				}
				else if (cn.x < 0 || r.x < r2.x) {
					angle = right;
				}
				
			}
			else  {
				other->hit.hit = true;
				other->hit.origin = enemy;
			}
		}
	}
	else {
		player__col = true;
	}
}