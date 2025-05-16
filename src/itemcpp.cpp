#include "../headers/item.h"
#include "../headers/collisions.h"

items Item::get_Itype(){
	return I_T;
}

Vector2 Item::GetPosition() {

	return(get_cord());
}
Vector2 Item::GetSize() {
	Vector2 v = { get_rect().width, get_rect().height };
	return v;
}

void Item::acc(float dt) {
	if (I_T != coin && I_T != fire_flower) {
		if (get_speed().y < MAX_SPEED * 5) {
			set_speedy(get_speed().y + 1000 * dt);
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
void Item::update(float dt) {
	In_collision = false;
	if (rising) {
		if (get_cord().y <= O_pos.y - 64) {
			rising = false;
			set_speedx(200);
			set_speedy(0);
		}
		else {
			set_speedx(0);
			set_speedy(-80);
			move(dt);
		}
	}
	else if (I_T != coin && I_T != fire_flower) {
		move(dt);
	}
	

}

Item::Item(items it, Vector2 cord, int color) : ent(cord, { 0,0 }, { 64,64 }, 1, 0) {
	I_T = it;
	O_pos = cord;
	if (I_T != coin) {
		rising = true;
	}
	else {
		rising = false;;
	}
	c = color;
	item_frame = 0;
	set_speedx(0);
	set_health(1);
}


obj_type Item::get_type() {
	if (I_T == used) {
		return none;
	}
	return item;
}



void Item::ProcessCollision(collidable* other, float dt) {

	Rectangle r2 = other->get_rect();
	Rectangle r = get_rect();
	Vector2 speed = get_speed();
	r.width *= 0.8;
	r.height *= 0.8;
	r.y += get_rect().height * 0.1;


	   if(other->get_health() > 0 && !rising){
		   switch (other->get_type()) {
		   case(block): {
			   
			   bool col = DynamicRectVsRect(r, {speed.x*dt,speed.y*dt}, r2, &cp, &cn, &ct, prop_cp, dt);
			
			
			   if (col && I_T != coin && !(other->transparent && (r.y + r.height / 2 - (r2.y - r2.height / 2) >= 5 || cn.y != -1))) {
				   float remainingtime = 1 - ct;
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

				   In_collision = true;
				   if (cn.y != 0) {
					   if(cn.y == -1 && (I_T == power_star || other->is_dynamic()) ){
						   speed.y = -200;
					   }
					 
					   else {
						   speed.y += fabsf(speed.y) * remainingtime * cn.y;
					   }
					   set_speedy(speed.y);
					
					  
					   
				   }
				   if (cn.x != 0) {
					   speed.x *= -1;
					   r.x = cp.x;
					  
					   set_speedx(speed.x);
					 
					   
					   
				   }
				   set_cord(r.x, r.y - get_rect().height*0.1);
				  

			   }

			   break;
		   }
		   case(player): {
			   r2 = get_rect();
			   r = other->get_rect();
			   speed = other->get_speed();
			   r.width /= 2;
			   r.height /= 2;
			   bool col = DynamicRectVsRect(r, { speed.x * dt,speed.y * dt }, r2, &cp, &cn, &ct, prop_cp, dt);
			   
			   if (col) {
				if (I_T != coin && I_T != one_up) {
					score_points(5);
				}
				else if (I_T == one_up) {
					score_points(10);
				}
				other->hit.hit = true;
				other->hit.origin = item;
				other->hit.item_type = I_T;
				I_T = used;
			   }
			   
			   break;
		   }

		   }
	    }
}





void Item::draw(Texture2D* t, float dt) {
	if (get_health() > 0) {
		Rectangle r = get_rect();
		switch (I_T) {
		case(mushroom): {
			if (get_speed().x >= 0) {
				DrawTexturePro(*t, { 0,0,16,16 }, r, { r.width / 2,r.height / 2 }, 0, RAYWHITE);
			}
			else {
				DrawTexturePro(*t, { 0,0,-16,16 }, r, { r.width / 2,r.height / 2 }, 0, RAYWHITE);
			}
			break;
		}
		case(one_up): {
			DrawTexturePro(*t, {16,0,16,16 }, r, { r.width / 2,r.height / 2 }, 0, RAYWHITE);
			break;
		}
		case(power_star): {
			if (item_frame >= 3) {
				item_frame = -2;
			}
			if (item_frame > 0) {
				DrawTexturePro(*t, {float(16 * int(fabsf(item_frame)) ), 48 ,16,16 }, r, { r.width / 2,r.height / 2 }, 0, RAYWHITE);
			}
			else {
				DrawTexturePro(*t, { 0,48,16,16 }, r, { r.width / 2,r.height / 2 }, 0, RAYWHITE);
			}
			item_frame += 6 * dt;
			break;
		}
		case(coin): {
			
			if (block_frame > 0) {
				DrawTexturePro(*t, { float( (16 * int(fabsf(block_frame)) ) + 64*c),32,16,16 }, r, { r.width / 2,r.height / 2 }, 0, RAYWHITE);
			}
			else {
				DrawTexturePro(*t, { float(64*c),32,16,16 }, r, { r.width / 2,r.height / 2 }, 0, RAYWHITE);
			}
			
			break;
		}
		case(fire_flower): {
			if (item_frame >= 3.8) {
				item_frame = 0;
			}
			else {
				item_frame += 10*dt;
			}
		
			if (item_frame > 0) {
				DrawTexturePro(*t, { float(16 * int(fabsf(item_frame))), float(16) ,16,16 }, r, { r.width / 2,r.height / 2 }, 0, RAYWHITE);
			}
			else {
				DrawTexturePro(*t, { 0,16,16,16 }, r, { r.width / 2,r.height / 2 }, 0, RAYWHITE);
			}
			

		}
		}
	
	}
}