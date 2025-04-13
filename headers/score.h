#pragma once
#include "ent.h"

class Score : public ent {
private:
	int time = 2;
	score_count score_text;
public:
	void acc(float dt);
	Score(Vector2 cord, score_count count);
	obj_type get_type();
	void draw(Texture2D* t,float dt);
	void update(float dt);
};