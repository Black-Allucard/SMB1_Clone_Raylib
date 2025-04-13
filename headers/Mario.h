#pragma once
#include "ent.h"
#include "collisions.h"



enum states {
	still,
	moving,
	duck,
	brake,
	shrink,
	grow,
	warping,
	climb,
	win,
	dead
};
struct camera {
	Vector2 pos;
	float zoom;
};

class mario:public ent{
	
private:
	bool in_star = false;
	int cutscene = 0;
	
	states stat;
	direc dir;
	direc moving_dir;
	float throw_time = 0;
	
	float down_timer = 0;
	float flash_timer = 0;
	
	
	
	


public :
	float win_timer = 0;
	bool win = false;
	int lives = 3;
	float dead_time = 0;
	int coins = 0;
	bool ducking = false;
	bool up_collision = false;
	float warp_timer = 0;
	bool music = true;
	int get_dir();

	mario(Vector2 cord, Vector2 s, Vector2 size, bool d, float g);

	bool in_warp = false;
	bool out_warp = false;
	bool is_warping();
	bool Check_collision(Rectangle* Target, float dt);
	bool is_star();

	void reset_coins();
	void reset(float x, float y);
	setting enviroment;
	
	Vector3 get_data();

	Vector2 new_cam_cord;
	Vector2 GetSize();
	Vector2 GetPosition();

	Rectangle get_HitBox();

	void draw(Texture2D* t, float dt);
	void acc(float dt);
	void update(float);
	void update_rec();
	void ProcessCollision(collidable* other, float dt);
	void set_speed(float x, float y);
	void reverse_dir();
	void set_stat(states new_stat);
	void kill();
 
	states get_stat();

	obj_type get_type();
	Rectangle get_source_rect(float dt, states stat);

};

