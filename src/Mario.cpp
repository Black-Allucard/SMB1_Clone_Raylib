#pragma once
#include "../headers/Mario.h"

float show = false;
int firecount = 0;
float down_time = 0;
bool grounded = false;
bool colliding;
float frame = 0;
float star_frame = 0;
float frame_speed;
bool jump;
int speed;
Vector2 warp_point;
Vector2 new_cords;
Vector2 warp_cam_cord;
Vector2 new_speed;
setting new_s;
float target = 0;
bool ducked;
int run = 1;
direc previous_dir;
states previous_stat;
float inv_timer = 0;
float t_time = 0;
float flash_frame;
bool hit_block;
bool fixed;
power previous_pow = tiny;
power Item_pow;
void mario::kill() {
	stat = dead;
	set_health(0);
	pow = tiny;
	jump = false;
	grounded = false;
	win = false;
	in_star = false;
}
void mario::set_stat(states new_stat) {
	stat = new_stat;
}
void mario::reset(float x, float y) {
	if (stat == dead || (win && axe_struck && bowser_died)) {
		pow = tiny;
	}
	win = false;
	axe_struck = false;
	bowser_died = true;
	fixed_cam = false;
	win_timer = 0;
	set_cord(x, y);
	
	dead_time = 0;
	warp_timer = 0;
	win_timer = 0;

	
	stat = still;
	dir = right;
	down_timer = 0; 
	grounded = true;
	update_rec();
	hit.hit = false;
	hit.hit_up = false;
	in_warp = false;
	switch (pow) {
	case(tiny): {
		set_health(1);
		break;
	}
	default: {
		set_health(2);
		break;
	}
	}
}
float sign(float x) {
	if (x > 0) {
		return 1.0f;
	}
	if (x < 0) {
		return -1.0f;
	}
	if (x == 0) {
		return 0.0f;
	}
}

Rectangle mario::get_source_rect(float dt,states stat) {
	Rectangle source;
	source.width = 16 * dir;
	if (stat == dead) {
		source = { 96,32,16,16 };
	}
	else {
		if (pow == tiny) {
			source.height = 16;

		}
		else {
			source.height = 32;
		}

		switch (pow) {
		case(tiny): {
			source.y = 32;
			break;
		}
		case(shroom): {
			source.y = 0;
			break;
		}
		case(flower): {
			source.y = 96;
			break;
		}
		}
		switch(stat){
		case(duck): {
			if (pow == tiny) {
				source.x = 16 + 16 * int(frame);
			}
			else {
				source.x = 96;
				source.y += 8;
				source.height = 24;
			}
			if (!ducked) {
				ducked = true;
			}
			break;
		}
		case(climb): {
			source.x = 112 + 16 * int(frame);
			break;
		}
		case(still):case(warping): {
			if (throw_time > 0) {
				source.x = 256;
			}
			else {
				source.x = 0;
			}
			if(jump && stat != climb) {
				if (throw_time > 0) {
					source.x = 256;
				}
				else {
					source.x = 80;
				}
				dir = previous_dir;
			}
			break;
		}
		case(brake): {
			source.x = 64;
			break;
		}

		default: {
			if (throw_time > 0) {
				source.x = 240;
				source.x += 16 + 16 * int(frame);
			}
			else {
				source.x = 16 + 16 * int(frame);
			}
			if (jump && stat != climb) {
				if (throw_time > 0) {
					source.x = 256;
				}
				else {
					source.x = 80;
				}
				dir = previous_dir;
			}
			break;
		}
		}


		if (in_star && star_frame < 8) {
			star_frame += 25 * dt;
		}
		else {
			star_frame = 0;
		}

		
		

		if (in_star) {
			source.y += 144 + int(star_frame) * 48;
		}

	}
	return source;

}

mario::mario(Vector2 cord, Vector2 s, Vector2 size, bool d, float g) : ent(cord,s,size,d,g) {
	pow = tiny;
	stat = still;
	dir = right;
	moving_dir = right;
	down_timer = 0;
	bowser_died = true;
	axe_struck = false;

	lives = 1;
	grounded = true;
	update_rec();
	hit.hit = false;
	hit.hit_up = false;
	in_warp = false;
	switch (pow) {
	case(tiny): {
		set_health(1);
		break;
	}
	default: {
		set_health(2);
		break;
	}
	}
}

void mario::update_rec() {
	Vector2 cord = get_cord();
	switch (pow) {
	case(flower): {
		set_rect(cord.x, cord.y, 64, 128 );

		break;
	}
	case(shroom): {
		set_rect(cord.x, cord.y, 64, 128);

		break;
	}
	case(tiny): {
		set_rect(cord.x, cord.y, 64, 64);

		break;
	}
	case(star): {
		set_rect(cord.x, cord.y, 64, 128);
		
		break;
	}
	}
	
	if (!is_warping()) {
		if (stat == duck) {
			set_rect(cord.x, cord.y, 64, 96);

			if (previous_stat != duck) {
				set_cord(cord.x, cord.y + 16);

			}
			
			
		}
		else if (previous_stat == duck) {
			set_cord(cord.x, cord.y - 16);
			grounded = true;

		}
	}

	
	previous_pow = pow;
}

int mario::get_dir() {
	return dir;
}

void mario::acc(float dt) {
	direc previous_moving_dir = moving_dir;
	if ((stat < shrink || stat == dead || win) && !is_warping()) {
		previous_stat = stat;
		previous_dir = dir;
		down_time -= dt;
		if (stat != dead && !win) {
			if (pow == flower && IsKeyPressed(KEY_E) && (stat == moving || stat == still)) {

				
				if (down_time <= 0) {
					spawn.type = projectile;
					spawn.specify = fire_ball;
					firecount++;
					if (firecount >= 2) {
						down_time = 0.5;
						firecount = 0;
					}
					else {
						down_time = 1.75 * dt;
					}
					throw_time = 8 * dt;

					Sound fire = LoadSound("../assets/audio/fireball.wav");
					SetSoundVolume(fire, 10);

					PlaySound(fire);
				}
				


			}

		
			run = 1 + 1*IsKeyDown(KEY_LEFT_SHIFT);
			

			

			

				if (((IsKeyDown(KEY_RIGHT) && IsKeyDown(KEY_LEFT)) || (!IsKeyDown(KEY_RIGHT) && !IsKeyDown(KEY_LEFT)))) {
					if (fabsf(get_speed().x) <= 50) {
						set_speedx(0);
						stat = still;
					}
				}

				else if (IsKeyDown(KEY_RIGHT) || IsKeyPressed(KEY_RIGHT)) {
					moving_dir = right;
					if (!jump) {
						dir = right;
					}
					stat = moving;
				}
				else if (IsKeyDown(KEY_LEFT) || IsKeyPressed(KEY_LEFT)) {
					moving_dir = left;
					if (!jump) {
						dir = left;
					}
					stat = moving;
				}

				else if (fabsf(get_speed().x) <= 20) {
					set_speedx(0);
					stat = still;
				}
				else {
					stat = moving;
				}

				if ((get_speed().x < 0 && IsKeyDown(KEY_RIGHT)) || (get_speed().x > 0 && IsKeyDown(KEY_LEFT))) {
					if (!jump) {
						stat = brake;
					}
				}

				if ((IsKeyDown(KEY_DOWN) || IsKeyPressed(KEY_DOWN) || ducking) && stat < shrink) {
					if (pow != tiny) {
						stat = duck;
					}
					else if (pow == tiny) {
						stat = still;
					}

				}
			
			

			if (IsKeyPressed(KEY_SPACE) && grounded && !jump && down_timer == 0) {

				grounded = false; 
				down_timer += dt;

				set_speedy(-MAX_SPEED);

				Sound tiny_jump = LoadSound("../assets/audio/tiny_jump.wav");
				Sound shroom_jump = LoadSound("../assets/audio/shroom_jump.wav");

				if (pow == tiny) {
					SetSoundVolume(tiny_jump, 10);
					PlaySound(tiny_jump);
				}
				else {
					SetSoundVolume(shroom_jump, 10);
					PlaySound(shroom_jump);
				}


			}


		

			if (IsKeyDown(KEY_SPACE) && down_timer > 0) {
				down_timer += dt;
			}
			else {
				down_timer = 0;
			}

			if (down_timer < 0.37 && down_timer > 0) {
				set_speedy(-MAX_SPEED * 2.5);
			}

		
			
		    jump = !grounded && stat != duck;
			
			
		

			switch (stat) {
			case(moving):case(brake): {
				
				if (((IsKeyDown(KEY_RIGHT) && IsKeyDown(KEY_LEFT)) || (!IsKeyDown(KEY_RIGHT) && !IsKeyDown(KEY_LEFT)))) {
				;
				
					if (fabsf(get_speed().x) <= 80) {
						set_speedx(0);
						stat = still;
					}
					else {
						if (get_speed().x > 0) {
							set_speedx(get_speed().x - 10 * run);
						}
						else if (get_speed().x < 0) {
							set_speedx(get_speed().x + 10 * run);
						}
					}
				}
				else if (fabsf(get_speed().x) <= MAX_SPEED * run || moving_dir != previous_moving_dir) {
					set_speedx(get_speed().x + 8 * moving_dir * run);
				}
			
				
				break;
				}
			
			case(still):case(duck): {
				if (fabsf(get_speed().x) <= 20) {
					set_speedx(0);
					if (stat != duck) {
						stat = still;
					}
				}
				else {
					if (get_speed().x > 0) {
						set_speedx(get_speed().x - 10 * run);
					}
					if (get_speed().x < 0) {
						set_speedx(get_speed().x + 10 * run);
					}
				}
				break;
				}

				}
			
		}


		if (down_timer <= 0 || down_timer >= 0.37) {
			if (get_speed().y < MAX_SPEED * 3) {
				set_speedy(get_speed().y + 72);
			}
			else {
				set_speedy(MAX_SPEED * 3);
			}
		}
		
		throw_time -= dt;
		
	}
}
void mario::draw(Texture2D* t, float dt) {
	Rectangle r = { get_rect() };
	states used_stat;
	used_stat = stat;
	

	
	if ((inv_timer > 0 && !in_star) || used_stat == grow) {
		if ((flash_frame > 2 && used_stat < shrink) || (used_stat == shrink && flash_frame > 1) || (used_stat == grow && flash_frame > 2.5)) {
			flash_frame = 0;
			show = !show;
		}
		else {
			flash_frame++;
		}
	}
	else {
		flash_frame = 0;
		show = true;

	}

	
	if (previous_stat == still || previous_stat == brake) {
		
		frame_speed = 0;
	}
	else {
		switch (stat) {
		case(climb):{
			frame += 8 * dt * run;
			if (frame >= 2) {
				frame = 0;
			}
			break;
		}
		case(grow): {
			if (frame >= 8) {
				frame = 0;
			}
			break;
		}
		default: {
			if (is_warping() || (win && win_timer > 0.5)){
				frame += 4 * dt * run;
				if (frame >= 3) {
					frame = 0;
				}
			}
			else if (frame_speed < 12 * dt * run) {
				if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_LEFT)) {
					frame_speed += fabsf(get_speed().x) / (MAX_SPEED) / 3 * dt * run;
				}
				else {
					frame_speed -= fabsf(get_speed().x) / (MAX_SPEED) / 3 * dt * run;
				}
				frame += frame_speed;
			}
			else {

				frame += 12 * dt * run;

			}
			if (frame >= 3) {
				frame = 0;

			}
			break;
		}
	}
	}

	if (fabsf(get_speed().x) <= 5 && win && !jump && stat != climb) {
		used_stat = still;
	}
	if ((show || used_stat == grow)) {
		switch (used_stat) {
		case(shrink): {


			if (t_time > 0.6) {
				DrawTexturePro(*t, { 0,0,16 * float(dir),32 }, get_rect(), { get_rect().width / 2,get_rect().height / 2 }, 0, RAYWHITE);
			}
			else if (t_time > 0.4) {
				DrawTexturePro(*t, { 144,0,16 * float(dir),32 }, get_rect(), { get_rect().width / 2,get_rect().height / 2 }, 0, RAYWHITE);
			}
			else if (t_time > 0) {
				DrawTexturePro(*t, { 0,32,16 * float(dir),16 }, get_rect(), {get_rect().width / 2,get_rect().height / 2}, 0, RAYWHITE);
			}
			break;

		}

		case(grow): {

			switch (Item_pow) {
			case(shroom): {
				if (t_time > 0.4) {
					if (show) {
						DrawTexturePro(*t, { 0,32,16 * float(dir),16 }, get_rect(), { get_rect().width / 2,get_rect().height / 2 }, 0, RAYWHITE);
					}
					else {
						if (in_star) {
							DrawTexturePro(*t, { 240,144,16 * float(dir),32 }, Rectangle{ get_cord().x, get_cord().y -32, 64, 128 }, { 32,64 }, 0, RAYWHITE);
						}
						else {
							DrawTexturePro(*t, { 240,0,16 * float(dir),32 }, Rectangle{ get_cord().x, get_cord().y-32, 64, 128 }, { 32,64 }, 0, RAYWHITE);
						}

					}
				}

				else if (t_time > 0) {

					DrawTexturePro(*t, { 0,0,16 * float(dir),32 }, get_rect(), { get_rect().width / 2,get_rect().height / 2 }, 0, RAYWHITE);

				}
				break;
			}
			case(flower): {
				frame += 35 * dt;
				float y = float(192 + (48 * int(frame)));
			
				if (pow == tiny) {
					DrawTexturePro(*t, { 0,y,16 * float(dir),32 }, Rectangle{ get_cord().x, get_cord().y - 32, 64, 128 }, { 32,64 }, 0, RAYWHITE);
				}
				else {
					DrawTexturePro(*t, { 0,y,16 * float(dir),32 }, Rectangle{ get_cord().x, get_cord().y, 64, 128 }, { 32,64 }, 0, RAYWHITE);
				}
				break;
			}

			}
			break;
		}
		
		case(dead): {
			DrawTexturePro(*t, get_source_rect(dt,used_stat), get_rect(), Vector2{ get_rect().width / 2, get_rect().height / 2 }, 0, RAYWHITE);
			break;
		}

		default: {
			DrawTexturePro(*t, get_source_rect(dt,used_stat), get_rect(), Vector2{ get_rect().width / 2, get_rect().height / 2 }, 0, RAYWHITE);
			break;
		}
		}
	}
	
}

Vector2 mario::GetSize() {
	Vector2 v = { get_rect().width, get_rect().height };
	return v;
}

states mario::get_stat() {
	return stat;
}

void mario::set_speed(float x, float y) {
	set_speedx(x);
	set_speedy(y);
}

obj_type mario::get_type() {
	return player;
}

void mario::reverse_dir() {
	if (dir == left) {
		dir = right;
	}
	else if(dir == right) {
		dir = left;
	}
}

Rectangle mario::get_HitBox() {
	Rectangle r = { get_rect().x,get_rect().y + get_rect().height * 1 / 16 ,get_rect().width * 6 / 8 ,get_rect().height * 7 / 8 };
	if (stat == duck && pow != tiny) {
		r.y += get_rect().height * 2 / 16;
		r.height -= get_rect().height * 2 / 8;
	}
	return r;
}

bool mario::is_warping() {
	return in_warp || out_warp;
}

void mario::ProcessCollision(collidable* other, float dt) {
	//update_rec();
	states m_stat = stat;
	obj_type t = other->get_type();
	Rectangle r = { get_rect().x,get_rect().y + get_rect().height * 1 / 16 ,get_rect().width * 6 / 8 ,get_rect().height * 7 / 8 };
	if (stat == duck && pow != tiny) {
		r.y += get_rect().height * 2 / 16;
		r.height -= get_rect().height * 2 / 8;
	}

	Rectangle r2 = other->get_rect();

	Vector2 speed1 = get_speed();

	Vector2 speed2 = other->get_speed();
	Vector2 speed = speed1;
	if (other->moving_ent) {
		speed.x -= speed2.x;
		speed.y -= speed2.y;
	}
	else {
		r2.x = other->GetPosition().x;
		r2.y = other->GetPosition().y;
	}
	bool col;

		
	if ( ((other->get_health() > 0&& ( stat < shrink || stat == climb || win )&& get_health() > 0) || t == warp ||( t == pole))) {
		switch (t) {
		case(block): {
			Rectangle rec = get_rect();
			
			rec.width = (64*6)/8 ;
			
			rec.height = 128;
			rec.y -= 16;

			if (stat == duck && r.y > r2.y && !other->transparent) {
				

				DrawRectangleLines(rec.x - rec.width / 2, rec.y - rec.height / 2, rec.width, rec.height, WHITE);

				if (AABB(rec,r2)) {
					if ((r.y + r.height / 2) - rec.y <= rec.height/2) {
						
						ducking = true;
						stat = duck;
						up_collision = true;
					}
				}
				else {
					float dif = rec.y - (r2.y + r2.height / 2) - rec.height / 2;
					if ( dif >= 0 && !up_collision) {
						ducking = false;
					}
				}
			}
		
			
			col = DynamicRectVsRect(r, { (speed.x) * dt,(speed.y) * dt }, r2, &cp, &cn, &ct, prop_cp, dt);

			if (col && !(other->transparent && (r.y + r.height/2 - (r2.y - r2.height/2) >= 5 || cn.y != -1)) ) {
				    In_collision = true;
					other->In_collision = true;
					
					up_collision = true;
					float remainingtime = 1.0f - ct;
					if (!is_warping()) {
						if (AABB(r, r2) && fabsf(remainingtime) > 1) {
							Resolve_AABB(&r, &r2, &cp, &cn);
							
							remainingtime = 1;
						}
						if (!AABB(r, r2) && fabsf(remainingtime) > 1) {
							remainingtime = 0;
							cn.x = 0;
							cn.y = 0;

						}

						if (cn.y > 0) {

							if ((fabsf(r2.x - (r.x + r.width / 2 * dir)) < r2.width / 2) && !hit_block && fabsf(1 - ct) <= 1) {
								if (!other->is_dynamic()) {
									Sound bump = LoadSound("../assets/audio/bump.wav");
									SetSoundVolume(bump, 10);
									PlaySound(bump);
								}
								if (hit_block == false) {

								}
								if (other->what_item() == coin || other->what_item() == multi_coin) {
									coins++;
								}
								hit_block = true;
								other->hit.origin = player;

								other->hit.player_pow = pow;
								down_timer = 0;

								other->hit.hit = true;
							}
						}
						
						if (!(other->hidden && (!other->hit.hit || cn.y != 1))) {

							if (AABB(r, r2) && fabsf(1.0f-ct) > 1) {
								
								if (cn.x != 0) {
									r.x = cp.x;
								}
								if (cn.y != 0) {
									r.y = cp.y;
								}
									remainingtime = 1;
							}
							speed1.x += fabsf(speed.x) * remainingtime * cn.x;
							set_speedx(speed1.x);
							speed1.y += fabsf(speed.y) * remainingtime * cn.y;
							set_speedy(speed1.y);

							if (cn.x != 0) {
								if (win) {
									show = false;
								}
								
								if (fabsf(get_speed().x) <= 20 || win) {
									if (stat != duck) {
										stat == still;
									}
									set_speedx(0);
								}

							}
							else if (cn.y != 0) {
									
								other->collision_count++;
								if (cn.y < 0) {
								
									jump = false;
									grounded = true;
									streak = 0;
									if (other->get_speed().x != 0) {
										carry_speed.x = other->get_speed().x;
									}
								}
								
								down_timer = 0;
							}

						}
					if (!ducking) {
						if (stat == duck && pow != tiny) {
							set_cord(r.x, r.y - get_rect().height * 3 / 16);
						}
						else {
							set_cord(r.x, r.y - get_rect().height * 1 / 16);
						}
					}
						





						else {
							

							if ((r.y - r.height / 2 == r2.y + r2.height / 2) && fabsf(r2.x - (r.x + r.width / 2 * dir) < r2.width / 2) && !hit_block && jump) {
								hit_block = true;

								other->hit.origin = player;
								other->hit.player_pow = pow;
								other->hit.hit = true;
								set_speedy(0);
								down_timer = 0;
							}

						}
					}
					break;
				}
			
			break;
		}

		case(enemy): {


			
 			r2 = other->get_HitBox();
			col = DynamicRectVsRect(r, { speed.x * dt,speed.y * dt }, r2, &cp, &cn, &ct, prop_cp, dt);

			if (col && (stat < shrink) && !is_warping()) {
				float remainingtime = 1.0f - ct;

				if (AABB(r, r2) && fabsf(remainingtime) > 1) {
					Resolve_AABB(&r, &r2, &cp, &cn);
					remainingtime = 1;
				}

				if (!AABB(r, r2) && fabsf(remainingtime) > 1) {
					remainingtime = 0;

				}
				if (in_star) {
					other->hit.player_pow = star;
					other->hit.origin = player;
					other->hit.hit = true;
					hit.hit = false;
					hit.hit_up = false;
				}


			}
		
			
			break;
		}

		case(warp): {


  			r2 = other->get_rect();
			
			r = get_rect();
			col = DynamicRectVsRect(r, { speed.x * dt,speed.y * dt }, r2, &cp, &cn, &ct, prop_cp, dt);
			if (col && !in_warp && !other->has_plant) {
				float remainingtime = 1.0f - ct;
				
				if (AABB(r, r2) && fabsf(remainingtime) > 1) {
					Resolve_AABB(&r, &r2, &cp, &cn);
					remainingtime = 1;
				}
				if (!AABB(r, r2) && fabsf(remainingtime) > 1) {
					remainingtime = 0;
					cn.x = 0;
					cn.y = 0;

				}

				if (other->side_warp()) {
					if ( (cn.x == -1 && (IsKeyDown(KEY_RIGHT) || IsKeyPressed(KEY_RIGHT))) || (cn.x == 1 && (IsKeyDown(KEY_LEFT) || IsKeyPressed(KEY_LEFT)) )) {
						
						if (grounded) {


							speed.x = 150 * cn.x * -1;
							speed.y = 0;
							target = r2.x - other->get_rect().width / 2 * sign(speed.x) + get_rect().width / 2 * sign(speed.x);
							stat = moving;
							warp_timer = 0;
							in_warp = true;

							Sound shrink_s = LoadSound("../assets/audio/shrink.wav");
							SetSoundVolume(shrink_s, 10);
							PlaySound(shrink_s);
							new_s = other->GetWarpSetting();
							new_cords = other->GetPosition();
							warp_cam_cord = other->GetCamPos();
							fixed_cam = other->Fixed_cam();
							new_speed = other->get_speed();
						}
					}
				}
				else {
					if ((cn.y == -1 && IsKeyDown(KEY_DOWN)) || (cn.y == 1 && IsKeyDown(KEY_UP))) {
						if (fabsf(r.x - r2.x) <= 70) {
							speed.y = 150 * cn.y * -1;
							speed.x = 0;
							target = r2.y - other->get_rect().height / 2 * sign(speed.y) + get_rect().height / 2 * sign(speed.y);
							r.x = r2.x;
							set_cord(r2.x, r.y);
						
							stat = still;
							in_warp = true;
						
							warp_timer = 0;
							Sound shrink_s = LoadSound("../assets/audio/shrink.wav");
							SetSoundVolume(shrink_s, 10);
							PlaySound(shrink_s);
							new_s = other->GetWarpSetting();
							new_cords = other->GetPosition();
							warp_cam_cord = other->GetCamPos();
							fixed_cam = other->Fixed_cam();
							new_speed = other->get_speed();
							warp_point = get_cord();

							
						}
					}
				}
				set_speedx(speed.x);
				set_speedy(speed.y);
			}


			break;
		}

		case(pole): {
			r = get_rect();
			col = DynamicRectVsRect(r, { speed.x * dt,speed.y * dt }, r2, &cp, &cn, &ct, prop_cp, dt);
			if (col && !win && stat != dead && !is_warping()) {
				float y = r2.y + r2.height / 2;
				if (stat != climb) {
					if (r.y <= y - r2.height * 4 / 5) {
						score_points(8);
					}
					else if (r.y <= y - r2.height * 3 / 5) {
						score_points(6);
					}
					else if (r.y <= y - r2.height * 2 / 5) {
						score_points(4);
					}
					else if (r.y <= y - r2.height * 1 / 5) {
					    score_points(2);
					}
					else {
						score_points(0);
					}
				}
				
				if (stat != climb) {
					Sound flag = LoadSound("../assets/audio/flag.wav");
					SetSoundVolume(flag, 10);
					PlaySound(flag);
					music = false;
					set_cord(other->get_rect().x - get_rect().width / 2, r.y);
					set_speed(0, 300);
					other->set_speedx(get_speed().x);
					other->set_speedy(get_speed().y);
				}

				stat = climb;

				 
			

			
				
				
			}

			break;
		}

		case(plant): {
			r2 = other->get_HitBox();
			bool col = DynamicRectVsRect(r, { speed.x * dt,speed.y * dt }, r2, &cp, &cn, &ct, prop_cp, dt);
			if (col && !is_warping() && !win) {
				if (!hit.hit) {
					hit.hit = true;
					hit.origin = enemy;
					hit.hit_up = false;
				}
			}
		}

		case(projectile): {
 			r2 = other->get_HitBox();
			bool col = DynamicRectVsRect(r, { speed.x * dt,speed.y * dt }, r2, &cp, &cn, &ct, prop_cp, dt);
			if (col && !is_warping() && !win && !other->is_friendly()) {
				if (!hit.hit) {
					hit.hit = true;
					hit.origin = enemy;
					hit.hit_up = false;
				}
			}
			break;
		}
		}

	}

	else {
		if (get_health() <= 0) {
			stat = dead;
		}
	}

	
}



void mario::update(float dt) {
	printf("%d\n", flag_down);
	if (axe_struck && !bowser_died) {
		set_speedx(0);
		set_speedy(0);
	}
	if (!up_collision){
		ducking = false;
	}
	else {
		
	}
	if (!In_collision) {
		
		grounded = false;
		hit_block = false;
	}

	if (hit.hit && !is_warping()) {
		switch (hit.origin) {
		case(enemy): {
			if (pow != tiny && stat < shrink && inv_timer == 0) {
				Sound shrink_s = LoadSound("../assets/audio/shrink.wav");
				PlaySound(shrink_s);
				SetSoundVolume(shrink_s, 10);
				set_health(get_health() - 1);
				hit.hit = false;
				inv_timer = 3;
				t_time = 0.8;
				stat = shrink;
				if (previous_stat == duck && stat != duck) {
					set_rect(get_cord().x, get_cord().y - 16, 64, 96);
					set_cord(get_cord().x, get_cord().y - 16);

				}
				break;

			}
			if (pow == tiny && get_health() > 0 && inv_timer == 0) {
				set_health(get_health() - 1);
				set_dynamic(false);
		
				stat = dead;
				Sound die = LoadSound("../assets/audio/mario_death.wav");
				SetSoundVolume(die, 10);
				PlaySound(die);
			}

			break;
		}
		case(item): {
			switch (hit.item_type) {
			case(mushroom): {
				if (pow < shroom) {
					Item_pow = shroom;
					stat = grow;
					t_time = 0.8;
				}
				score += 1000;
				Sound grow_s = LoadSound("../assets/audio/grow.wav");
				SetSoundVolume(grow_s, 10);
				PlaySound(grow_s);
				set_health(2);
				hit.hit = false;
				hit.origin = none;
				break;
			}
			case(coin): {
				Sound coin_s = LoadSound("../assets/audio/coin.wav");
				if (IsSoundPlaying(coin_s)) {
					StopSound(coin_s);
				}
				SetSoundVolume(coin_s, 10);
				PlaySound(coin_s);
				hit.hit = false;
				hit.origin = none;
				coins += 1;
				score += 200;
				break;
			}
			case(fire_flower): {

				if (pow < flower) {
					Item_pow = flower;
					stat = grow;
					t_time = 0.8;
				}
				Sound grow_s = LoadSound("../assets/audio/grow.wav");
				score += 1000;
				PlaySound(grow_s);
				SetSoundVolume(grow_s, 10);
				set_health(2);
				hit.hit = false;
				hit.origin = none;
				break;
			}
			case(power_star): {
				Sound grow_s = LoadSound("../assets/audio/grow.wav");
				PlaySound(grow_s);
				SetSoundVolume(grow_s, 10);
				inv_timer = 10;
				hit.hit = false;
				hit.origin = none;
				score += 1000;
				in_star = true;
				break;
			}
			}

			break;
		}
		}

		hit.hit = false;
		hit.hit_up = false;
	}
	else {
		hit.hit = false;
		hit.hit_up = false;
	}

	
		
	if (in_warp) {
		warp_timer += dt;
		if(fabsf(get_speed().x) >= 2){
			if (sign(get_speed().x) * (target - get_cord().x) <=  0.5) {
			
				warp_timer += dt;
				set_speedx(0);
				set_speedy(0);
				set_cord(target, get_cord().y);
			}
		}
		else if (fabsf(get_speed().y) >= 2) {
			if (sign(get_speed().y)*(target - get_cord().y) <= 0.5) {
				
				warp_timer += dt;
				set_speedy(0);
				set_speedy(0);
				set_cord(get_cord().x,target);
			}
		}
		if (warp_timer >= 1.5) {
			new_cam_cord = warp_cam_cord;
			enviroment = new_s;
			if (warp_timer > 2) {
				dir = right;
				set_speed(new_speed.x, new_speed.y);
				set_cord(new_cords.x, new_cords.y);

				if (new_speed.x != 0 || new_speed.y != 0) {
					out_warp = true;
					in_warp = false;
				}
				else {
					out_warp = false;
					in_warp = false;
				}
			}
		}
	}
	else if (out_warp) {

		if (get_speed().x != 0) {
			set_speedy(0);
			if ((get_cord().x - get_HitBox().width * sign(new_speed.x) <= new_cords.x && get_speed().x < 0) || (get_cord().x - get_HitBox().width * sign(new_speed.x) >= new_cords.x && get_speed().x > 0) ) {
				out_warp = false;
				warp_timer = 0;
				set_speedy(0);
				set_speedx(0);
				stat = still;
			}

		}
		if (get_speed().y != 0) {
			stat = still;
			if (fabsf(get_cord().y - get_rect().height / 2 * sign(new_speed.y) - new_cords.y) <= 5) {
				out_warp = false;
				warp_timer = 0;
				set_speedy(0);
				set_speedx(0);
				stat = still;
				set_cord(get_cord().x, new_cords.y - get_rect().height / 2);
			}

		}

		

	}
	
	if ((!win && stat == climb && flag_down && fabsf(get_speed().y) <= 5) || (bowser_died && axe_struck && win_timer <= 0)) {
		win = true;
		set_cord(get_cord().x + get_rect().width , get_cord().y);
		dir = left;
		music = false;
	}



	if (coins >= 100) {
		reset_coins();
	}

	if (get_health() <= 0) {
		
		stat = dead;
		music = false;
		set_speedx(0);
	}
	if (stat == dead) {
		dead_time += dt;
	}
	if (dead_time > 0.7 && !is_dynamic()) {
		set_dynamic(true);
		set_speedx(0);
		set_speedy(-1000);
		down_timer = 0;
	}
	if((win_timer > 0.5 && win_timer < 0.52 && get_speed().x == 0) || (axe_struck && win) ){
		fixed_cam = false;
		if ((axe_struck && win_timer <= 0) || !axe_struck) {
			Sound win_level = LoadSound("../assets/audio/castle_win.wav");
			if (!axe_struck) {
				win_level = LoadSound("../assets/audio/win.wav");
			}
			SetSoundVolume(win_level, 10);
			PlaySound(win_level);
			dir = right;
			stat = moving;
			set_speedx(200);
			
		}
	
		
	
		

	}
	if (win) {
		win_timer += dt;
	}
	
	if ((stat < shrink || stat > grow) && is_dynamic()) {
		move(dt);
	}

	if (t_time > 0 && stat <= grow) {
		if (t_time <= 0.4 && pow > tiny && stat == shrink) {
			pow = tiny;
			set_cord(get_rect().x, get_rect().y + 32);
			
		}
		if (t_time <= 0.4 && stat == grow && pow < Item_pow) {
			if (pow == tiny) {
				set_cord(get_rect().x, get_rect().y - 32);
			}
			pow = Item_pow;
			
		}
		set_speedx(0);
		set_speedy(0);
		
		t_time -= dt;
	}
	else {
		
		if (stat >= shrink && stat <= grow) {
			
			stat = still;
			down_timer = 0;

		}
		t_time = 0;
	}

	if (inv_timer > 0) {
		
			
		inv_timer -= dt;
		
	}
	else {
		if (in_star) {
			in_star = false;
		}
		inv_timer = 0;
	}
	
	carry_speed.x = 0;
	update_rec();
	up_collision = false;
	In_collision = false;
}

Vector2 mario::GetPosition() {
	return get_cord();
}
bool mario::is_star() {
	return in_star;
}
void mario::reset_coins() {
	coins = 0;
}
Vector3 mario::get_data() {
	return { float(lives),float(coins),float(score) };
}
