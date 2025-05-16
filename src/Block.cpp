#include "../headers/collisions.h"
#include "../headers/Block.h"


block_type Block::getB_type() {
	return B_T;
}

Vector2 Block::GetPosition() {
	if (B_T != platform) {
		return(O_pos);
	}
	else {
		return(GetPosition());
	}
}
Vector2 Block::GetSize() {
	Vector2 v = { get_rect().width, get_rect().height };
	return v;
}
void Block::kill() {
	this->~Block();
}
Rectangle Block::getHitBox() {
	return get_rect();
}
void Block::acc(float dt) {

	if (is_dynamic() && B_T < platform) {
		if (get_speed().y < MAX_SPEED * 2) {
			set_speedy(get_speed().y + 1800 * dt);
		}
		else {
			set_speedy(MAX_SPEED * 5);
		}
	}

	if (!started_cycle) {
		set_speedx(0);
		set_speedy(0);
	}
}
void Block::update(float dt) {

	if (B_T == bridge && bridge_time <= 0) {
		bridge_time = 0.1;
		set_health(0);
		In_collision = false;
		spawn.type = none;
		Sound bridge_break = LoadSound("../assets/audio/brick_break.wav");
		SetSoundVolume(bridge_break, 10);
		StopSound(bridge_break);
		PlaySound(bridge_break);
	}
	if (B_T == stop && started_cycle) {
		fixed_cam = true;
		set_health(0);
		
	}
	if (started_timer && new_frame_rule()) {
		active_time++;
	}
	if (active_time > 11 && block_item == multi_coin) {
		block_item = coin;
	}
	if (hit.hit && !is_dynamic()) {

		if (hidden) {
			hidden = false;
		}

		if (B_T == item_block || B_T == brick) {
			if (!is_dynamic()) {
				set_speedy(-300);
			}
			if (bounce_time == 0) {
				bounce_time = 0.36;
			}
		}

		
			switch (block_item) {
			case(used): {
				switch (B_T) {
				case(item_block): {
					B_T = empty_block;
					Sound coin_s = LoadSound("../assets/audio/coin.wav");
					SetSoundVolume(coin_s, 10);
					PlaySound(coin_s);
					spawn.type = item;
					spawn.specify = coin;
					break;
				}
				case(brick): {
					if (hit.player_pow != tiny && get_health() > 0) {
						Sound break_brick = LoadSound("../assets/audio/brick_break.wav");
						SetSoundVolume(break_brick, 10);
						PlaySound(break_brick);
						lose_health();
						spawn.type = projectile;
						spawn.specify = brick_part;
					}
					break;
				}
				}
				break;
			}
			case(mushroom):case(flower): {
				B_T = empty_block;
				if (hit.player_pow != tiny) {
					spawn.type = item;
					spawn.specify = flower;
				}
				else {
					spawn.type = item;
					spawn.specify = mushroom;
				}
				break;
			}
			case(coin): case(multi_coin): {
				if (block_item == multi_coin) {
					started_timer = true;
				}
				else {
					B_T = empty_block;
				}
				Sound coin_s = LoadSound("../assets/audio/coin.wav");
				SetSoundVolume(coin_s, 10);
				PlaySound(coin_s);
				spawn.type = item;
				spawn.specify = coin;
				break;
			}
			default: {
				B_T = empty_block;
				spawn.type = item;
				spawn.specify = block_item;
				break;
			}

			}

			if (block_item != multi_coin) {
				block_item = used;
			}
		


		
	}
	
	if (fabsf(0 - bounce_time) > 0.02) {
		bounce_time -= dt;
		set_dynamic(true);
		
	}
	else if(B_T < platform){
		bounce_time = 0;
		
		if (get_health() < 2 && B_T == item_block) {
			B_T = empty_block;
		}
		set_cord(O_pos.x, O_pos.y);
		set_dynamic(false);
		
	}
	if (B_T == platform) {
		switch (c) {
		case(0): {
			if (get_speed().y > 0 && ((get_rect().y - get_rect().height / 2) - cam_cord.y) > window_height / 2) {
				set_cord(get_cord().x, cam_cord.y - window_height / 2 - get_rect().height / 2);
			}
			else if (get_speed().y < 0 && ((get_rect().y + get_rect().height / 2) - cam_cord.y) < -window_height / 2) {
				set_cord(get_cord().x, cam_cord.y + window_height / 2 + get_rect().height / 2);
			}
			break;
		}
		case(1): {
			if (get_speed().y > 0 && (get_rect().y) > O_pos.y + 120 * 4) {
				set_speedy(get_speed().y * -1);
			}
			else if (get_speed().y < 0 && (get_rect().y) < O_pos.y) {
				set_speedy(get_speed().y * -1);
			}
			break;
		}
		case(2): {
			if (get_speed().x > 0 && (get_rect().x) > O_pos.x + 64 * 4 ) {
				set_speedx(get_speed().x * -1);
			}
			else if (get_speed().x < 0 && (get_rect().x) < O_pos.x - 64 * 4 ) {
				set_speedx(get_speed().x * -1);
			}
			break;
		}
		}
	}

	
	if (is_dynamic()) {
 		move(dt);
	}
	else {
		set_speedx(0);
		set_speedy(0);
	}
	if (get_speed().y >= 0) {
		hit.hit = false;
		hit.hit_up = false;
	}
	if (!In_collision) {
		bounce = false;
	}
	if (collision_count < 1) {
		In_collision = false;
	}
	collision_count = 0;
	
}


bool Block::is_dead() {
	if (!In_collision && get_health() <= 0) {
		return true;
	}
	return false;
	
}

Block::Block(block_type b, Rectangle rect, items item, bool is_hidden, bool is_on_top, int color,bool is_edge) : ent({rect.x,rect.y},{0,0},{rect.width,rect.height},0,0) {
	B_T = b;
	c = color;
	top = is_on_top;
	block_item = item;
	moving_ent = false;
	edge_block = is_edge;

	
	if (B_T == platform) {
		transparent = true;
		moving_ent = true;
		set_rect({rect.x,rect.y,48*4,32});
		if (is_edge) {
			set_rect({ rect.x,rect.y,48 * 4,32 });

		}
		else if (top) {
			if (c != 2) {
				set_speedy(-150);
			}
			else {
				set_speedx(150);
			}

		}
		else {
			if (c != 2) {
				set_speedy(150);
			}
			else {
				set_speedx(-150);
			}
		}
			
		set_dynamic(true);
	}
	else {
		transparent = false;
		
	}
	
	O_pos = { rect.x,rect.y };
	//set_cord(rect.x, rect.y);

	bounce = false;

	switch (B_T) {
	
		case(item_block): {
			set_health(2);
			break;
		}
		case(death): case(axe) : {
			set_health(0);
			break;
		}

		default : { 
			set_health(1);
			break;
		}
	}
	hit.hit = false;
	hit.origin = none;
	hit.hit_up = false;
	hit.item_type = used;
	hidden = is_hidden;
	
}
obj_type Block::get_type() {
	switch (B_T) {
	case(death): {
		return block;
		break;
	}
	case(axe):  {
		if (axe_struck) {
			return none;
		}
		return block;
		break;
	}
	default: {
		if (get_health() <= 0 && !In_collision && spawn.type == none) {
			return none;
		}

		return block;
		break;
	}
	}
	

}

void Block::ProcessCollision(collidable* other, float dt) {
	/*if (hit.hit && B_T != brick && B_T != item_block) {
		hit.hit = false;
	}*/
	Rectangle r = other->get_rect();
	Rectangle r2 = get_rect();
	Vector2 speed = { other->get_speed().x*dt,  other->get_speed().y * dt };
	if (B_T != item_block && B_T != brick) {
		hit.hit = false;
		hit.hit_up = false;
	}
	
	
	switch (other->get_type()) {
		case(enemy): {

			bool col = DynamicRectVsRect(r, speed, r2, &cp, &cn, &ct, prop_cp, dt);
			if (col) {
				In_collision = true;
				if (hit.hit) {
					other->hit.hit = true;
					other->hit.origin = block;
				}
				
			}
			break;
		}
		
		case(item): {
			r = get_rect();
			r2 = other->get_rect();
			Vector2 speed = { get_speed().x * dt,  get_speed().y * dt };
			bool col = DynamicRectVsRect(r, speed, r2, &cp, &cn, &ct, prop_cp, dt);
			if (col && ct <= 1.0f ) {
				
			
				if (cn.y == 1 && is_dynamic() && !bounce) {
					other->set_speedx(other->get_speed().x * -1);
					other->set_speedy(-300);
					bounce = true;
				}
				In_collision = true;

			}
			break;
		}
		
		case(player): {
			r = other->get_HitBox();
			bool col = DynamicRectVsRect(r, speed, r2, &cp, &cn, &ct, prop_cp, dt);
		
			if (col) {
				float remainingtime = 1.0f - ct;
				if (AABB(r, r2) && fabsf(remainingtime) > 1) {
					Resolve_AABB(&r, &r2, &cp, &cn);
					if (cn.x != 0) {
						r.x = cp.x;
					}
					if (cn.y != 0) {
						r.y = cp.y;
					}
					remainingtime = 1;
				}
				if (!AABB(r, r2) && fabsf(remainingtime) > 1) {
					remainingtime = 0;
					cn.x = 0;
					cn.y = 0;

				}
				if (B_T == death) {
					//printf("dead\n");
					if (other->get_health() > 0) {
						 
						Sound die = LoadSound("../assets/audio/mario_death.wav");
						SetSoundVolume(die, 10);
						PlaySound(die);

						other->kill();
					}
				
					
				}
				if (B_T == axe && r.y+r.height/2 <= r2.y + r2.height/2 - 5) {
					axe_struck = true;
					set_health(0);
				}
				
			}
			break;
		}
	}
}
	   

	
void Block::ResolveCollision(collidable* other, float dt) {


}
items Block::what_item() {
	return block_item;
}
void Block::draw(Texture2D* t, float dt) {
	if (!hidden) {
		Rectangle r = get_rect();
		switch (B_T) {
		case(brick): {
			if (get_health() > 0) {
				DrawTexturePro(*t, { float(16 * c) + 48 * !top,0,16,16 }, r, { r.width / 2,r.height / 2 }, 0, RAYWHITE);
			}
			break;
		}
		
		case(item_block): {
			if (get_health() > 0) {
				if (block_frame > 0) {
					if (hit.hit) {
						DrawTexturePro(*t, { float(64 * c + 16 * int(fabsf(block_frame))),float(16),16,16 }, r, { r.width / 2,r.height / 2 }, 0, RED);
					}
					else {
						DrawTexturePro(*t, { float(64 * c + 16 * int(fabsf(block_frame))),float(16),16,16 }, r, { r.width / 2,r.height / 2 }, 0, RAYWHITE);
					}
				}
				else {
					DrawTexturePro(*t, { float(64 * int(c)),16,16,16 }, r, { r.width / 2,r.height / 2 }, 0, RAYWHITE);

				}
			}
			 
			break;
		}
		case(axe): {

			if (block_frame > 0) {
				
				DrawTexturePro(*t, { float(64 + 16 * int(fabsf(block_frame))),float(48),16,16 }, r, { r.width / 2,r.height / 2 }, 0, RAYWHITE);
				
			}
			else {
				DrawTexturePro(*t, { float(64),48,16,16 }, r, { r.width / 2,r.height / 2 }, 0, RAYWHITE);

			}

			break;
		}
		case(empty_block): {
			if (get_health() > 0) {
				DrawTexturePro(*t, { float(16 * int(c)),48,16,16 }, r, { r.width / 2,r.height / 2 }, 0, RAYWHITE);
			}
			break;
		}
		case(platform): {
			if (get_health() > 0) {
				DrawTexturePro(*t, { float(0),175,48 ,8 }, r, { r.width / 2,r.height / 2 }, 0, RAYWHITE);
			}
			break;
		}
		case(bridge): {
			if (get_health() > 0) {
				DrawTexturePro(*t, { float(80),32,16,16 }, r, { r.width / 2,r.height / 2 }, 0, RAYWHITE);
			}
			break;
		}
		}
	}
}
