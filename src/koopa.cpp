#include "../headers/koopa.h"
#include "../headers/collisions.h"

koopa::koopa(Vector2 cord, direc direc, int color, bool flying) : Enemy(cord, { 64,96 }, {float(60 * (!is_flying || color != 2)),0 }, dir, 1, true, color,flying) {
	g_frame = 0;
	dir = direc;
	set_speedx(80 * dir * !(is_flying && color == 2));
	dead_time = 20;
	hit.hit = false;
	hit.hit_up = false;
	set_health(1);
	hit.origin = none;
}
obj_type koopa::get_type() {
	if (is_dead()) {
		return none;
	}
	return enemy;
}
void koopa::acc(float dt) {
	if (!(is_flying && color == 2)) {
		if (get_speed().y < MAX_SPEED * 5) {
			set_speedy(get_speed().y + 1800 * dt);
		}
		else {
			set_speedy(MAX_SPEED * 5);
		}
	}
	else {
		set_speedy(100 * fly_dir);
	}
	if (!started_cycle) {
		set_speedx(0);
		set_speedy(0);
	}
}
bool koopa::is_dead() {
	if (dead_time <= 0) {
		return true;
	}
	return false;
}
void koopa::ProcessCollision(collidable* other, float dt) {
	obj_type t = other->get_type();
	Rectangle r;
	if (squashed) {
		r = { get_rect().x,float(get_rect().y + get_rect().height * 0.25),get_rect().width,float(get_rect().height * 0.5) };
	}
	else {
		r = { get_rect().x,float(get_rect().y + get_rect().height * 0.25 * 0.5),get_rect().width,float(get_rect().height * 0.75) };
	}
	Rectangle r2 = other->get_rect();
	Vector2 speed = get_speed();

	bool col = DynamicRectVsRect(r, { speed.x * dt,speed.y * dt }, r2, &cp, &cn, &ct, prop_cp, dt);

	if (get_health() > 0 && other->get_health() > 0) {



		switch (other->get_type()) {
		case(block): {
			if (col) {

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
				In_collision = true;

				if (other->hit.hit && cn.y == -1) {
					hit.hit = true;
					hit.origin = block;
					break;
				}
				else if (cn.y != 0) {
					if (!squashed && other->edge_block && color == 2 && cn.y < 0) {
						if ((fabsf(r.x + r.width / 2 - r2.x) >= r2.width / 2 && speed.x > 0) || (fabsf(r.x - r.width / 2 - r2.x) >= r2.width / 2 && speed.x < 0) ) {
							if (dir == right) {
								dir = left;
							}
							else {
								dir = right;
							}

							set_speedx(speed.x * -1);
						}
					}
					if (cn.y == -1) {
						if (is_flying && c != 2) {
							set_speedy(-300);
						}
						grounded = true;
					}
					set_speedy(get_speed().y + fabsf(get_speed().y) * remainingtime * cn.y);
				}
				else if (cn.x != 0) {

					
					if (dir == right) {
						dir = left;
					}
					else {
						dir = right;
					}
					set_speedx(speed.x * -1);
				}
				if (squashed) {
					set_cord(r.x, r.y - get_rect().height * 0.25);
				}
				else{
					set_cord(r.x, r.y - get_rect().height * 0.25 * 0.5);
				}
			}
			break;
		}
		case(enemy): {
			if (col && other->get_health() > 0) {
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
				In_collision = true;
		

				if (squashed && get_speed().x != 0) {
					printf("hit\n");
					other->hit.hit = true;
					other->hit.origin = enemy;
					streak++;
					if (!other->started_cycle) {
						other->started_cycle = true;
					}
					score_points(streak);
					hit.hit = false;
					hit.hit_up = false;
					break;

				}

				else if (hit.origin != projectile && other->hit.origin != projectile) {


					if (cn.x != 0 && remainingtime > 0 && remainingtime <= 1) {
						speed.x *= -1;
						r.x = cp.x;
						if (dir == left) {
							dir = right;
						}
						else {
							dir = left;
						}
						set_cord(r.x, r.y);
						set_speedx(speed.x);

					}
					else if (cn.y != 0) {
						if (cn.y == -1) {
							if (is_flying && c != 2) {
								set_speedy(-300);
							}
							grounded = true;
						}
						set_speedy(get_speed().y + fabsf(get_speed().y) * remainingtime * cn.y);
					}

				}
			}
			break;
		}
			
		
		

		
		case(player): {
			Process_player_collision(other, dt);
			
		}
			break;
		
		}


	}
	return;
}
Rectangle koopa::get_HitBox() {
	Rectangle r = get_rect();

	r.height *= 0.5;
	r.width *= 0.8;
	r.y += r.height * 0.25;
	
	return r;
}
void koopa::draw(Texture2D* t, float dt) {
	Rectangle r = get_rect();
	
	if (squash_time < 4) {
		if (frame_time > 1) {
			frame_time = 0;
			show = !show;
		}
		else {
			frame_time += 12*dt;
		}
	}
	else {
		show = false;
		frame_time = 0;
	}
	if (get_health() <= 0) {
		DrawTexturePro(*t, { float(64 + 96 *color),16,-16,24 }, r, { r.width / 2,r.height / 2 }, -180, RAYWHITE);
	}
	else if (squashed) {
		

		if(show){
			DrawTexturePro(*t, { float(80 + 96 * color),16,float(16 * dir),24 }, r, { r.width / 2,r.height / 2 }, 0, RAYWHITE);
		}
		else {
			DrawTexturePro(*t, { float(64 + 96 * color),16,float(16 * dir),24 }, r, { r.width / 2,r.height / 2 }, 0, RAYWHITE);
		}

	}
	else {
		g_frame += 6 * dt;
		if (g_frame >= 2) {
			g_frame = 0;
		}


		if (dir == right) {
			DrawTexturePro(*t, { float(16* floorf(g_frame)+ 96 * color + 32 * is_flying),16,-16,24 }, r, { r.width / 2,r.height / 2 }, 0, RAYWHITE);
		}
		else {
			DrawTexturePro(*t, { float(16 * floorf(g_frame) + 96 * color + 32 * is_flying),16,16,24 }, r, { r.width / 2,r.height / 2 }, 0, RAYWHITE);
		}

	}
	r = get_HitBox();
	DrawRectangleLines(r.x - r.width/2, r.y - r.height/ 2, r.width , r.height ,RED);
	 
}
void koopa::update(float dt) {
	move(dt);
	if (new_frame_rule()) {
		if (get_health() <= 0) {
			hit.hit = false;
			hit.hit_up = false;
			dead_time -= 1;
		}
		if (squashed && get_speed().x == 0) {
			if (grounded) {
				squash_time -= 1;
			}
		}
		else {
			squash_time = 16;
		}

	}

	if (hit.hit && get_health() > 0) {
		if (started_cycle) {
			if (hit.player_pow == star && hit.origin == player) {
				Sound hit = LoadSound("../assets/audio/hit.wav");
				SetSoundVolume(hit, 10);
				
				PlaySound(hit);
				
				score_points(1);
				flip();
			}

			else if (squashed && hit.origin == player) {


				squash_time = 16;
				Sound hit_h = LoadSound("../assets/audio/hit.wav");
				SetSoundVolume(hit_h, 10);
				
				PlaySound(hit_h);
				
				if (get_speed().x != 0) {
					set_speedx(0);
				}
				else {


					if (angle == right) {

						set_speedx(700);
					}
					else if (angle == left) {
						set_speedx(-700);
					}
				}

			}



			else if (hit.hit_up) {

				squash_time = 16;
				if (!squashed) {
					squash();
				}
				else {
					Sound hit = LoadSound("../assets/audio/hit.wav");
					SetSoundVolume(hit, 10);
					
					PlaySound(hit);
					
				}
			}
			else {
				is_flying = false;
				Sound hit = LoadSound("../assets/audio/hit.wav");
				SetSoundVolume(hit, 10);
				
				PlaySound(hit);
				
				score_points(1);
				flip();
			}
		}

		hit.hit = false;
		hit.hit_up = false;
	}
	if (is_flying && color == 2) {
		if (get_HitBox().y + get_HitBox().height / 2 >= O_Pos.y + 64 * 6) {
			if (fly_dir == down) {
				fly_dir = up;
			}
		}
		else if (get_HitBox().y - get_HitBox().height / 2 <= O_Pos.y - 64 ) {
			if (fly_dir == up) {
				fly_dir = down;
			}
		}
	}
	if (get_health() <= 0) {
		dead_time += dt;
	}

	In_collision = false;
	if (squash_time <= 0) {
		squashed = false;
		set_speedx(100 * dir);
	}
	grounded = false;
	

	collision_count = 0;
	

}