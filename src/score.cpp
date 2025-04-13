#include "../headers/score.h"
void Score::acc(float dt) {
	set_speedy(get_speed().y - 100 * dt);
}
Score::Score(Vector2 cord, score_count count) {
	set_cord(cord.x, cord.y);
	set_rect(cord.x, cord.y, 64, 32);
	score_text = count;
	set_speedy(-200);
}
obj_type Score::get_type() {
	if (time > 0) {
		return text;
	}
	return none;
}
void Score::draw(Texture2D* t,float dt) {
	DrawTexturePro(*t, { float(33 + 16 * score_text),8,16,8 }, get_rect(), { get_rect().width / 2,get_rect().height / 2 }, 0, WHITE);
}
void Score::update(float dt) {
	if (new_frame_rule()) {
		time -= 1;
	}
	move(dt);
}