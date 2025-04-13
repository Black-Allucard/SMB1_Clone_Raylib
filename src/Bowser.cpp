#include "../headers/bowser.h"
#include <random>
#include "../headers/collisions.h"
int Bowser::get_dir() {
	return dir;
}
void Bowser::ResetHitboxes() {
	Rectangle r1 = get_rect();
	Rectangle r2 = get_rect();
	r1.width *= 0.5;
	r1.x += r1.width / 2;
	r1.y += 16;
	r1.height *= 0.625;
	r2.y -= 20;
	r2.height *= 0.5625;
	r2.x -= r2.width / 4;
	r2.width /= 2;
	hitboxes[0] = r1;
	hitboxes[1] = r2;

}
void Bowser::acc(float dt) {
	if (get_speed().y < MAX_SPEED * 2) {
		set_speedy(get_speed().y + 1200 * dt);
	}
	else {
		set_speedy(MAX_SPEED * 2);
	}
}
Bowser::Bowser(Vector2 cord, direc s_dir) : ent(cord, Vector2{ float(50 * s_dir),0 }, Vector2{ 128,128 }, 1,0) {
	ResetHitboxes();
	fire = false;
	open = false;
	jump = false;
	set_health(5);
	hit.hit = false;
	hit.hit_up = false;
	hit.origin = none;
	O_pos = cord;
	
}
void Bowser::update(float dt) {
	srand(GetTime());
	
	
	if (hit.hit) {
		set_health(get_health() - 1);
		hit.hit = false;
		if (get_health() <= 0) {
			set_speedx(0);
		}
	}
	if (get_health() <= 0 || axe_struck) {
		if ((get_health() <= 0 || (axe_struck && dead_timer >= 3)) && !flipped) {
			set_speedy(-500);
			if (get_health() <= 0) {
				score_points(8);
			}
			if (!flipped) {
				Sound fall = LoadSound("../assets/audio/bowser_fall.wav");
				SetSoundVolume(fall, 10);
				PlaySound(fall);
			}
			flipped = true;

		}
		else if(!flipped){
			set_speedy(0);
			set_speedx(0);
		}
		dead_timer += dt;
	}

	else {
		fire_timer -= dt;
		dir_timer -= dt;
		if (dir_timer <= 0) {
			if (rand() % 2 == 0) {
				set_speedx(get_speed().x*-1);
			}
			dir_timer = 1.5;
		}
		if (get_cord().x - O_pos.x <= -128) {
			set_speedx(50);
		}
		if (get_cord().x - O_pos.x >= 128) {
			set_speedx(-50);
		}

		if (!jump) {
			jump_timer -= dt;
			if (jump_timer < 0) {
				jump_timer = 1.5;

				if (rand() % 3 == 0) {

					set_speedy(-630);
					jump = true;
				}
			}
		}
		if (fire_timer < 0) {
			fire_timer = 3;
			if (rand() % 2 == 0) {
				open = true;
			}

		}
		if (open) {
			open_timer -= dt;
			if (open_timer < 1 && !fire) {
				fire = true;
				spawn.type = projectile;
				spawn.specify = dragon_breath;
				Sound flame = LoadSound("../assets/audio/bowser_flame.wav");
				SetSoundVolume(flame, 10);
				PlaySound(flame);
			}
			if (open_timer <= 0) {
				open_timer = 1.5;
				fire = false;
				open = false;
			}

		}
		if (player_cord.x >= get_cord().x) {
			dir = right;
		}
		if (player_cord.x < get_cord().x) {
			dir = left;
		}
		ResetHitboxes();
	}
	
	move(dt);
	
	jump = true;
}
void Bowser::ProcessCollision(collidable* other, float dt) {
	Rectangle r2 = get_rect();
	Rectangle r = other->get_rect();
	Vector2 speed = other->get_speed();
	if (get_health() > 0 && !axe_struck) {
		switch (other->get_type()) {
		case(block): {
			speed = get_speed();
			bool col = DynamicRectVsRect(r2, { speed.x * dt,speed.y * dt }, r, &cp, &cn, &ct, prop_cp, dt);
			if (col) {
				float remainingtime = 1 - ct;
				if (AABB(r, r2) && fabsf(remainingtime) > 1) {
					Resolve_AABB(&r, &r2, &cp, &cn);
					remainingtime = 1;
					if (cn.y != 0) {
						r2.y = cp.y;
					}
					if (cn.x != 0) {
						r2.x = cp.x;
					}
					set_rect(r2.x, r2.y, r2.width, r2.height);
				}
				if (!AABB(r, r2) && fabsf(remainingtime) > 1) {
					remainingtime = 0;

				}
				In_collision = true;
				if (cn.y != 0) {
					if (cn.y < 0) {
						jump = false;
					}
					speed.y += fabsf(speed.y) * remainingtime * cn.y;
					set_speedy(speed.y);

				}
				if (cn.x != 0) {
					speed.x += fabsf(speed.x) * remainingtime * cn.x;
					set_speedy(speed.y);


				}
			}
			break;
		}
		case(player): {
			for (int i = 0; i < 2; i++) {
				r = other->get_HitBox();
				r2 = hitboxes[i];
				bool col = DynamicRectVsRect(r, { speed.x * dt,speed.y * dt }, r2, &cp, &cn, &ct, prop_cp, dt);
				if (col && !other->hit.hit) {
					other->hit.hit = true;
					other->hit.origin = enemy;
					break;
				}
			}
			break;
		}

		}
	}
}
void Bowser::draw(Texture2D* t, float dt) {
	float x = 0;
	frame += dt*12;
	if (frame >= 2) {
		frame = 0;
	}
	if (open) {
		x = 64;
	}
	int height = 32;
	if (flipped) {
		height *= -1;
	}
	Rectangle r = get_rect();
	Rectangle r0 = hitboxes[0];
	Rectangle r1 = hitboxes[1];
	if(get_health() <= 0){
		DrawTexturePro(*t, { 96,0,16,-16 }, {r.x,r.y,64,64}, {32,32}, 0, WHITE);
	}
	else {
		DrawTexturePro(*t, { x + 32 * int(frame),96,float(32 * (-dir)),float(height)}, get_rect(), { get_rect().width / 2,get_rect().height / 2 }, 0, WHITE);
		
	}
	
	

}
obj_type Bowser::get_type(){
	if ((dead_timer >= 3 && get_health() <= 0) || (dead_timer >= 7 && axe_struck)) {
		bowser_died = true;
		fixed_cam = false;
		return none;
	}
	return boss;
}
std::unique_ptr<Rectangle[]> Bowser::get_HitBoxes() {
	auto boxes = std::make_unique<Rectangle[]>(2);
	boxes[0] = hitboxes[0];
	boxes[1] = hitboxes[1];
	return boxes;
}