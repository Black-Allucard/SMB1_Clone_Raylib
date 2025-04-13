#pragma once
#include <raylib.h>
#include <iostream>
#include <vector>
#include <memory>

extern float bridge_time;
extern bool fixed_cam;
#define frame_rule (21)
#define window_width (256*4)
#define window_height (240*4)
extern int score; 
extern Vector2 cam_cord;
extern Vector2 player_cord;
extern int frame_count;
extern int win_time;
extern int one_ups;
extern bool flag_down;
extern int block_frame;
extern bool bowser_died;
extern bool axe_struck;

enum direc {
	left = -1, right = 1,
	up = -1, down = 1
};

enum score_count {
	score_100,
	score_200,
	score_400,
	score_500,
	score_800,
	score_1000,
	score_2000,
	score_4000,
	score_5000,
	score_8000,
	score_one_up
};
 
enum obj_type {
	player,
	block,
	enemy,
	projectile, 
	item,
	special,
	text,
	warp,
	pole,
	level_end,
	boss,
	plant,
	none
};
enum Warp_type {
	pipe,
	vine
};
enum power {
	tiny,
	shroom,
	flower,
	star,
};
enum items {
	coin,
	mushroom,
	fire_flower,
	power_star,
	one_up,
	multi_coin,
	used
};
enum projectiles {
	brick_part,
	fire_ball,
	hammer,
	spin_coin,
	dragon_breath,
};
enum sprite_color {
	color1,
	color2,
	color3,
};
enum BG_type {
	bush,
	cloud,
	tree,
	coral,
	large_bush
};
enum BG_size {
	small,
	medium,
	large
};
enum setting {
	sky,
	cave,
	water,
	castle
};
typedef struct hit {
	bool hit;
	bool hit_up;
	obj_type origin;
	items item_type;
	power player_pow;
}hit;
struct spawn{
	obj_type type;
	short int specify;
};




bool new_frame_rule();

class collidable{
private:
	
	float health;
	int bvh_index;
	
public:

	// used by blocks and platforms
	bool edge_block = false;
	Vector2 carry_speed = { 0,0 };

	// used by player and enemies

	int streak = 0; 
	bool player__col = false ;

	
	// used bij every child class

	bool started_cycle = false; 
	int c;
	int angle;
	bool edge;
	hit hit;
	power pow;
	bool In_collision = false;
	bool transparent = false;
	bool hidden;
	bool has_plant = false;
	spawn spawn;
	bool moving_ent = false;
	int collision_count;
	
	
	virtual bool is_warping(); // used by player
	virtual bool is_friendly(); // used by projectiles
 	virtual bool side_warp(); // used by pipe warps
	virtual bool Fixed_cam(); // used to fix the camera


	//used by every child class

	virtual Vector2 GetPosition();
	virtual Vector2 GetCamPos();
	virtual setting GetWarpSetting();
	virtual Rectangle get_rect();
	virtual Rectangle get_HitBox();
	 
	virtual std::unique_ptr<Rectangle[]> get_HitBoxes();

	virtual void set_dynamic(bool state);
	virtual bool is_dynamic();

	virtual void ProcessCollision(collidable* other,float dt); // collision interface

	virtual obj_type get_type();
	virtual Vector2 get_speed();
	virtual void set_speedx(float speed_x);
	virtual void set_speedy(float speed_y);
	virtual void draw(Texture2D*t,float dt);
	virtual void squash();
	virtual void lose_health();
	virtual float calc_int_min(int axis, float dt);
	virtual float calc_int_max(int axis, float dt);
	virtual void set_rect(Rectangle r);
	void score_points(int streak);
	void set_health(float hp);
	
	float get_health();
	virtual items what_item();
	virtual void kill();
	virtual bool is_dead();
	virtual void ResolveCollision(collidable* other, float dt);
	virtual void update(float dt);
};
