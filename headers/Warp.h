#pragma once
#include "ent.h"

class Warp: public ent {
private:
	int cutscene;
	float timer = 0;
	float Target_Height;
	Vector2 warp_to;
	Vector2 cam_pos;
	setting new_setting;
	Warp_type type = pipe;
	bool cam_fixed;
	bool side;
public:
	Vector2 GetCamPos();
	setting GetWarpSetting();
	Vector2 GetPosition();
	
	obj_type get_type();
	bool Fixed_cam();
	bool side_warp();
	Warp(Vector2 warp_cord, Vector2 warp_cam_cord, Rectangle rect,setting set,bool fix_cam,bool on_side,Vector2 Warp_speed);
	void ProcessCollision(collidable* other, float dt);
	void update(float dt);
};