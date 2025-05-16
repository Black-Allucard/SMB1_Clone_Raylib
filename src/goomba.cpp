#pragma once

#include "../headers/Goomba.h"
#include "../headers/collisions.h"
#include "../headers/Mario.h"

int p_streak = 0;

goomba::goomba(Vector2 cord,direc dir,int color, bool flying) : Enemy(cord,{64,64}, { 80,0 }, dir,1,true,color,flying) {
	g_frame = 0;
	set_speedx(80 * dir);
	hit.hit = false;
	hit.hit_up = false;
	set_health(1);
	dead_time = 3;
}
Rectangle goomba::get_HitBox() {
	Rectangle r = get_rect();

	r.height *= 1/2;
	r.y += r.height;
	
	return r;

}
void goomba::kill() {
	this->~goomba();
}
void goomba::acc(float dt) {
	if (get_speed().y < MAX_SPEED * 5) {
		set_speedy(get_speed().y + 1800 * dt);
	}
	else {
		set_speedy(MAX_SPEED * 5);
	}
	if (!started_cycle) {
		set_speedx(0);
		set_speedy(0);
	}
}
void goomba::squash(){
	squashed = true;
	Sound hit = LoadSound("../assets/audio/stomp.wav");
	SetSoundVolume(hit, 10);

	PlaySound(hit);
	
	lose_health();   
}
void goomba::update(float dt) {
	if (new_frame_rule()) {
		if (get_health() <= 0) {
			hit.hit = false;
			hit.hit_up = false;
			dead_time -= 1;
		}
	
	}
	
	if (hit.hit && get_health() > 0 && started_cycle) {

		if (hit.hit_up && hit.origin == player) {
			
			squash();
			dead_time = 2;
		}
		else if (!squashed) {
			flip();
			if (hit.origin == projectile) {
				score_points(0);
			}
			if ((hit.origin == player && hit.player_pow == star) || hit.origin == block) {
				score_points(1);
			}
			dead_time = 20;
			Sound hit = LoadSound("../assets/audio/hit.wav");
			SetSoundVolume(hit, 10);
		
			PlaySound(hit);
			

		}
		if (started_cycle) {
			hit.hit = false;
			hit.hit_up = false;
		}
		return;
	}
	
	//printf("speedx : %f\n", get_speed().x);
	if (!squashed) {

		move(dt);

	}
	
	In_collision = false;
	//printf("y : %f", get_cord().y);
	p_streak = 0;
}
bool goomba::is_dead() {
	if (dead_time <= 0) {
		return true;
	}
	else {
		return false;
	}
}
obj_type goomba::get_type() {
	if (is_dead()) {
		return none;
	}
	return enemy;
}

void goomba::ProcessCollision(collidable* other,float dt) {
	obj_type t = other->get_type();
	Rectangle r = get_rect();
	Rectangle r2 = other->get_rect();
	Vector2 speed = get_speed();
	if (other->moving_ent) {
		speed.x -= other->get_speed().x;
		speed.y -= other->get_speed().y;
	}

	
	if (get_health() > 0 && other->get_health() > 0) {
		switch (other->get_type()) {
		case(block): {
			bool col = DynamicRectVsRect(r, { speed.x * dt, speed.y * dt }, r2, &cp, &cn, &ct, prop_cp, dt);
			if (col) {
				float remainingtime = 1.0f - ct;

				if (AABB(r, r2) && fabsf(remainingtime) > 1) {
					Resolve_AABB(&r,&r2, &cp, &cn);
					remainingtime = 1;
					if (cn.y != 0) {
						r.y = cp.y;
					}
					if (cn.x != 0) {
						r.x = cp.x;
					}
				}
				if (!AABB(r, r2) && fabsf(remainingtime) > 1) {
					remainingtime = 0;

				}
				In_collision = true;
				if (!(cn.y != 1 && other->hidden)) {
					speed.y += fabsf(speed.y) * remainingtime * cn.y;
					set_speedy(speed.y);
					if (other->hit.hit) {
						hit.hit = true;
						hit.origin = block;
					}
				}
				if (cn.x != 0) {
					
					set_speedx(fabsf(get_speed().x) * cn.x);
					

				}

			

			
			}
			break;
		}
		case(enemy): {
			
			bool col = DynamicRectVsRect(r, { speed.x * dt, speed.y * dt }, r2, &cp, &cn, &ct, prop_cp, dt);
			if (col ) {
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
				if (!AABB(r, r2) && fabsf(remainingtime) > 1) {
					remainingtime = 0;

				}

				if (cn.x != 0 && remainingtime > 0 && remainingtime <= 1) {
					

					set_speedx(fabsf(get_speed().x) * cn.x);
					if (other->started_cycle && !other->squashed) {
						other->set_speedx(fabsf(other->get_speed().x) * -1 * cn.x);
					}
					
				}
				if (cn.y != 0 ) {

					set_speedy(get_speed().y + fabsf(speed.y)* remainingtime * cn.y);
				}
				set_rect(r.x, r.y, r.width, r.height);
				if (cn.y != 0) {
					DrawLine(r.x, r.y, r.x, r.y + 80 * cn.y,YELLOW);
				}

				
			}
			break;

		}
		case(player): {
			p_streak = other->streak;
			Process_player_collision(other, dt);
			break;
			
		}
		}
	}

	

	
	
}

void goomba::draw(Texture2D* t, float dt) {
	float height = 16;
	

	g_frame += 8 * dt;

    if (g_frame >= 2) {
	    g_frame = 0;
	}
	
		
	
	if (!squashed) {
		if (get_health() <= 0) {
			DrawTexturePro(*t, { float(0 + 48 * color),0,16,16 }, get_rect(), { get_rect().width / 2,get_rect().height / 2 }, -180, RAYWHITE);
		}
		else {
			DrawTexturePro(*t, { float(16 * int(g_frame) + 48 * color),0,16,16 }, get_rect(), { get_rect().width / 2,get_rect().height / 2 },0 , RAYWHITE);
		}
	}
	else {
		DrawTexturePro(*t, { float(32 + 48 * color),0,16,16 }, get_rect(), { get_rect().width / 2,get_rect().height / 2 }, 0, RAYWHITE);
	}

}