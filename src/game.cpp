#include "../headers/common.h"
#include "../headers/bowser.h"


struct tile {
	Rectangle source;
	Rectangle dest;
	Texture2D* tileset;
};

struct collision_point {
	Vector2 cn;
	Vector2 cp;
};

struct object {
	std::string name;
	std::string type;
	float x;
	float y;
	float width;
	float height;

};
struct cursor {
	int option;
};
enum options{
	single_player,
	two_player
};

enum world_state {
	playing_level,
	transition,
	game_over,
	start
};

std::vector<int> touching;

std::vector<edge> edges;

std::vector<ent*> layers[5];
std::vector<ent*> entities;

std::vector<tile> sky_tiles;
std::vector<tile> block_tiles;

std::vector<Vector3> collision_pairs_x;
std::vector<Vector3> collision_pairs;

std::vector<Vector3> *ptr = &collision_pairs_x;
std::vector<collision_point> contacts;

Vector2 Spawn_cords = {96,800};

setting level_setting;

int axis = 0;

ray ry;

bool y_axis = false;

int first_pair = 0;

int timer = 400;

int frames = 0;

int current_level = 0;

void tick_timer(Sound*s) {
	
}
void load_layers() {
	
	//printf("%d,%d\n", index,int(entities.size()));
	//int item_count = layers[0].size() + layers[1].size() + layers[2].size() + layers[3].size() + layers[4].size();
	//if (entities.size() != item_count) {
		
		layers[0].clear();
		layers[1].clear();
		layers[2].clear();
		layers[3].clear();
		layers[4].clear();
		for (int i = 0; i < entities.size(); i++) {
			switch (entities[i]->get_type()) {
			case(player):case(warp): {
				layers[0].push_back(entities[i]);
				break;
			}
			case(item): case(plant) : {
				layers[1].push_back(entities[i]);
				break;
			}
			case(block): case(pole): {
				layers[2].push_back(entities[i]);
				break;
			}
			case(enemy): case(projectile): case(boss) :{
				layers[3].push_back(entities[i]);
				break;
			}
			case(text): {

				layers[4].push_back(entities[i]);
				break;
			}
			}
		}
	//}
}

void remove_layer_object(ent* e) {
	int layer_index = 3;
	
	
	switch (e->get_type()) {
	case(item): {
		layer_index = 0;
		break;
	}
	case(block): case(pole): {
		layer_index = 1;
		break;
	}
	case(enemy): case(projectile): {
		layer_index = 2;
		break;
	}
	case(text): {
		layer_index = 3;
		break;
	}
	}

	auto i = find(layers[layer_index].begin(), layers[layer_index].end(),e);
	if (i != layers[layer_index].end()) {
		layers[layer_index].erase(i);
	}


}
Bowser* Spawn_Boss(Vector2 cord, direc speed_dir) {
	Bowser* b = new Bowser(cord, speed_dir);
	return b;
}
Score* Spawn_Score_text(Vector2 cord, score_count type) {
	Score* p = new Score(cord, type);
	return p;
}

Flag_pole* Spawn_flag_pole(Vector2 cord, int flag_color) {
	Flag_pole* p = new Flag_pole(cord,flag_color);
	return p;
}

Warp* Spawn_Warp(Vector2 warp_cord, Vector2 warp_cam, setting s, Rectangle r,bool fix_cam,bool side_warp,Vector2 warp_speed) {
	Warp* w = new Warp(warp_cord, warp_cam, r,s,fix_cam,side_warp,warp_speed);
	return w;
}

mario* Spawn_player(Vector2 cord, Vector2 speed, Vector2 size, bool dynamic, float gravity) {
	mario* m = new mario(cord,speed,size, dynamic, gravity);
	return m;
}

Block* Spawn_block(block_type b, Rectangle rect,items item_in_block, bool hidden,bool ontop,int color,bool is_edge) {
	Block* block = new Block(b, rect, item_in_block,hidden,ontop,color,is_edge);
	return block;
}

Enemy* Spawn_enemy(Vector2 cord, direc dir,enemies type,int color,bool flying) {
	switch (type){
	case(Goomba): {
		goomba* g = new goomba(cord, dir,color,flying);
		return g;
	}
	case(Koopa): {
		koopa* k = new koopa(cord, dir,color,flying);
		return k;
	}
	}
}
FireBar* Spawn_FireBar(Vector2 cord, int angle,direc spin_dir) {
	FireBar* I = new FireBar(cord,spin_dir,angle);
	return I;
}

Item* Spawn_item(items it, Vector2 cord,int col) {
	Item* I = new Item(it, cord,col);
	return I;
}

Pirahna_Plant* Spawn_Plant(Vector2 cord,int color) {
	Pirahna_Plant* p = new Pirahna_Plant(cord, color);
	return p;
}

Projectile* Spawn_projectile(projectiles pt, Vector2 cord,direc dir,Vector2 speed, int color) {
	switch (pt) {
	case(brick_part): {
		Brick_part* b = new Brick_part(cord, dir,color,speed);
		return b;
	}
	case(spin_coin): {
		SpinCoin* c = new SpinCoin(cord);
		return c;
	}
	case(fire_ball): {
		FireBall* f = new FireBall(cord, dir);
		return f;
	}
	case(dragon_breath) : {
		DragonBreath* d = new DragonBreath(cord,dir);
		return d;
	}
	}
}
void load_objects(Json::Value* actual_json,Json::FastWriter* writer) {
	for (int j = 0; j < 1; j++) {
		std::string prop = (*writer).write((*actual_json)["properties"][j]["name"]);

		std::string value = (*writer).write((*actual_json)["properties"][j]["value"]);

		if (!prop.compare("\"setting\"\n")) {
			if (!value.compare("\"sky\"\n")) {
				level_setting = sky;
			}
			if (!value.compare("\"cave\"\n")) {
				level_setting = cave;
			}
			if (!value.compare("\"castle\"\n")) {
				level_setting = castle;
			}
			if (!value.compare("\"water\"\n")) {
				level_setting = water;
			}
		}



	}
	int i = 0;
	while (true) {


		try {
			object obj;
			obj.name = (*writer).write((*actual_json)["layers"][3]["objects"][i]["name"]);
			obj.type = (*writer).write((*actual_json)["layers"][3]["objects"][i]["type"]);

			obj.x = std::stof((*writer).write((*actual_json)["layers"][3]["objects"][i]["x"])) * 4;
			obj.y = std::stof((*writer).write((*actual_json)["layers"][3]["objects"][i]["y"])) * 4;
			obj.width = std::stof((*writer).write((*actual_json)["layers"][3]["objects"][i]["width"])) * 4;
			obj.height = std::stof((*writer).write((*actual_json)["layers"][3]["objects"][i]["height"])) * 4;
			int col = 0;


			if (!obj.type.compare("\"block\"\n")) {
				items item = used;
				bool hidden = false;
				bool onTop = false;
				bool edge = false;
				std::string prop;
				std::string value;
				for (int j = 0; j <= 4; j++) {
					prop = (*writer).write((*actual_json)["layers"][3]["objects"][i]["properties"][j]["name"]);

					value = (*writer).write((*actual_json)["layers"][3]["objects"][i]["properties"][j]["value"]);

					if (!prop.compare("\"block_item\"\n")) {
						if (!value.compare("\"mushroom\"\n") || !value.compare("\"flower\"\n")) {
							item = mushroom;
						}
						else if (!value.compare("\"one_up\"\n")) {
							item = one_up;
						}
						else if (!value.compare("\"star\"\n")) {
							item = power_star;
						}
						else if (!value.compare("\"coin\"\n")) {
							item = coin;
						}
						else if (!value.compare("\"multi_coin\"\n")) {
							item = multi_coin;
						}
						else {
							item = used;
						}
					}
					if (!prop.compare("\"hidden\"\n") && !value.compare("true\n")) {
						hidden = true;

					}
					if (!prop.compare("\"top\"\n") && !value.compare("true\n")) {
						onTop = true;

					}
					if (!prop.compare("\"is_edge\"\n") && !value.compare("true\n")) {
						edge = true;

					}
					if (!prop.compare("\"color\"\n")) {
						std::string color = value.substr(0, 1);
						col = std::stoi(color);
					}

				}

				if (!obj.name.compare("\"static\"\n")) {

					entities.push_back(Spawn_block(static_block, { obj.x + obj.width / 2 , obj.y + obj.height / 2 ,obj.width,obj.height }, item, hidden, onTop, col, edge));

				}
				else if (!obj.name.compare("\"item_block\"\n")) {

					entities.push_back(Spawn_block(item_block, { obj.x + obj.width / 2 , obj.y + obj.height / 2,obj.width,obj.height }, item, hidden, onTop, col, edge));
				}
				else if (!obj.name.compare("\"brick\"\n")) {

					entities.push_back(Spawn_block(brick, { obj.x + obj.width / 2 , obj.y + obj.height / 2 ,obj.width,obj.height }, item, hidden, onTop, col, edge));
				}
				else if (!obj.name.compare("\"death\"\n")) {

					entities.push_back(Spawn_block(death, { obj.x + obj.width / 2 , obj.y + obj.height / 2 ,obj.width,obj.height }, item, hidden, onTop, col, edge));
				}
				else if (!obj.name.compare("\"platform\"\n")) {

					entities.push_back(Spawn_block(platform, { obj.x + obj.width / 2 , obj.y + obj.height / 2 ,obj.width,obj.height }, item, hidden, onTop, col, edge));
				}

				else {

					entities.push_back(Spawn_block(static_block, { obj.x + obj.width / 2 , obj.y + obj.height / 2 ,obj.width,obj.height }, item, hidden, onTop, col, edge));
				}
			}
			if (!obj.type.compare("\"enemy\"\n")) {

				int dir = 0;
				direc d = right;
				std::string prop;
				std::string value;
				for (int j = 0; j <= 1; j++) {
					prop = (*writer).write((*actual_json)["layers"][3]["objects"][i]["properties"][j]["name"]);

					value = (*writer).write((*actual_json)["layers"][3]["objects"][i]["properties"][j]["value"]);

					if (!prop.compare("\"color\"\n")) {
						std::string color = value.substr(0, 1);
						col = std::stoi(color);
					}
					else if (!prop.compare("\"dir\"\n")) {
						int dir = std::stof(value);
						if (dir == 1) {
							d = right;
						}
						if (dir == -1) {
							d = left;
						}
					}


				}

				if (!obj.name.compare("\"Koopa\"\n")) {
					entities.push_back(Spawn_enemy({ (obj.x + obj.width / 2), (obj.y + obj.height / 2) }, d, Koopa, col,false));
				}
				else if (!obj.name.compare("\"Goomba\"\n")) {

					entities.push_back(Spawn_enemy({ (obj.x + obj.width / 2), (obj.y + obj.height / 2) }, d, Goomba, col,false));
				}

			}
			if (!obj.type.compare("\"item\"\n")) {


				std::string prop;
				std::string value;
				for (int j = 0; j < 1; j++) {
					prop = (*writer).write((*actual_json)["layers"][3]["objects"][i]["properties"][j]["name"]);

					value = (*writer).write((*actual_json)["layers"][3]["objects"][i]["properties"][j]["value"]);

					if (!prop.compare("\"color\"\n")) {
						std::string color = value.substr(0, 1);
						col = std::stoi(color);
					}


				}

				if (!obj.name.compare("\"coin\"\n")) {
					entities.push_back(Spawn_item(coin, { (obj.x + obj.width / 2), (obj.y + obj.height / 2) }, col));
				}


			}
			if (!obj.type.compare("\"warp\"\n")) {
				std::string prop;
				std::string value;
				Vector2 NewCam = { 0,0 };
				Vector2 NewPos = { 0,0 };
				Vector2 NewSpeed = { 0,0 };
				bool fix_cam = false;
				bool side = false;
				setting env = sky;
				for (int j = 0; j <= 8; j++) {
					prop = (*writer).write((*actual_json)["layers"][3]["objects"][i]["properties"][j]["name"]);

					value = (*writer).write((*actual_json)["layers"][3]["objects"][i]["properties"][j]["value"]);

					if (!prop.compare("\"New_cam_x\"\n")) {
						NewCam.x = stof(value) * 4;
					}
					if (!prop.compare("\"New_cam_y\"\n")) {
						NewCam.y = stof(value) * 4;
					}
					if (!prop.compare("\"New_x\"\n")) {
						NewPos.x = stof(value) * 4;
					}
					if (!prop.compare("\"New_y\"\n")) {
						NewPos.y = stof(value) * 4;
					}
					if (!prop.compare("\"New_speed_x\"\n")) {
						NewSpeed.x = stof(value) * 4;
					}
					if (!prop.compare("\"New_speed_y\"\n")) {
						NewSpeed.y = stof(value) * 4;
					}
					if (!prop.compare("\"side\"\n")) {
						if (!value.compare("true\n")) {
							side = true;

						}
						else {
							side = false;
						}
					}
					if (!prop.compare("\"fix_cam\"\n")) {
						if (!value.compare("true\n")) {
							fix_cam = true;

						}
						else {
							fix_cam = false;
						}
					}
					if (!prop.compare("\"New_region\"\n")) {
						if (!value.compare("\"cave\"\n")) {
							env = cave;
						}
						if (!value.compare("\"castle\"\n")) {
							env = castle;
						}

					}

				}
				entities.push_back(Spawn_Warp(NewPos, NewCam, env, { (obj.x + obj.width / 2), (obj.y + obj.height / 2), obj.width, obj.height }, fix_cam, side, NewSpeed));
			}
			if (!obj.type.compare("\"Flag_pole\"\n")) {
				std::string prop;
				std::string value;
				Vector2 NewCam = { 0,0 };
				Vector2 NewPos = { 0,0 };
				Vector2 NewSpeed = { 0,0 };
				bool fix_cam = false;
				bool side = false;
				setting env = sky;
				for (int j = 0; j < 1; j++) {
					prop = (*writer).write((*actual_json)["layers"][3]["objects"][i]["properties"][j]["name"]);

					value = (*writer).write((*actual_json)["layers"][3]["objects"][i]["properties"][j]["value"]);

					if (!prop.compare("\"flag_color\"\n")) {
						col = stof(value) * 4;
					}
					entities.push_back(Spawn_flag_pole({ obj.x + obj.width / 2,obj.y + obj.height / 2 }, col));
					 
				}
			}
			i++;

		}
		catch (...) {
			break;
		}
	}
}
void load_tiles(tson::Map& level,Texture2D* Layer1Texture, Texture2D* Layer2Texture, Texture2D* Layer3Texture) {
	sky_tiles.clear();
	block_tiles.clear();

	Spawn_cords = { level.get<float>("Spawn_x")*4,level.get<float>("Spawn_y")*4 };



	std::string s = level.get<std::string>("setting");
	level_setting = sky;
	
	if (!s.compare("sky")) {
		level_setting = sky;
	}
	if (!s.compare("cave")) {
		level_setting = cave;
	}
	if (!s.compare("castle")) {
		level_setting = castle;
	}
	if (!s.compare("water")) {
		level_setting = water;
	}
	

	if (level.getStatus() == tson::ParseStatus::OK) {
		tson::Layer* tileLayer = level.getLayer("sky"); //This is a Tile Layer.

		if (tileLayer->getType() == tson::LayerType::TileLayer)
		{


			for (auto& [pos, tileObject] : tileLayer->getTileObjects()) //Loops through absolutely all existing tileObjects
			{
				tson::Tileset* tileset = tileObject.getTile()->getTileset();
				std::string name = tileset->getName();
				tson::Rect drawingRect = tileObject.getDrawingRect();
				tson::Vector2f position = tileObject.getPosition();

				Rectangle source = { drawingRect.x,drawingRect.y,drawingRect.width,drawingRect.height };
				Rectangle dest = { position.x * 4,position.y * 4,32,32 };

				Texture2D* t;

				t = Layer1Texture;

				tile Tile;
				Tile.source = source;
				Tile.dest = dest;
				Tile.tileset = t;
				sky_tiles.push_back(Tile);
			}
		}

		tson::Layer* tileLayer2 = level.getLayer("backround"); //This is a Tile Layer.

		if (tileLayer2->getType() == tson::LayerType::TileLayer)
		{


			for (auto& [pos, tileObject] : tileLayer2->getTileObjects()) //Loops through absolutely all existing tileObjects
			{
				tson::Tileset* tileset = tileObject.getTile()->getTileset();
				std::string name = tileset->getName();
				tson::Rect drawingRect = tileObject.getDrawingRect();
				tson::Vector2f position = tileObject.getPosition();

				Rectangle source = { drawingRect.x,drawingRect.y,drawingRect.width,drawingRect.height };
				Rectangle dest = { position.x * 4,position.y * 4,32,32 };

				Texture2D* t;

				t = Layer1Texture;

				tile Tile;
				Tile.source = source;
				Tile.dest = dest;
				Tile.tileset = t;
				block_tiles.push_back(Tile);

				//DrawTexturePro(*t, source, dest, { 0,0 }, 0, RAYWHITE);

			}
		}

		tson::Layer* tileLayer3 = level.getLayer("blocks");

		if (tileLayer3->getType() == tson::LayerType::TileLayer)
		{


			for (auto& [pos, tileObject] : tileLayer3->getTileObjects()) //Loops through absolutely all existing tileObjects
			{
				tson::Tileset* tileset = tileObject.getTile()->getTileset();
				std::string name = tileset->getName();
				tson::Rect drawingRect = tileObject.getDrawingRect();
				tson::Vector2f position = tileObject.getPosition();

				Rectangle source = { drawingRect.x,drawingRect.y,drawingRect.width,drawingRect.height };
				Rectangle dest = { position.x * 4,position.y * 4,32,32 };

				Texture2D* t;

				t = Layer3Texture;



				tile Tile;
				Tile.source = source;
				Tile.dest = dest;
				Tile.tileset = t;
				block_tiles.push_back(Tile);

				//DrawTexturePro(*t, source, dest, { 0,0 }, 0, RAYWHITE);

			}
		}

		tson::Layer* object_layer = level.getLayer("entities");

		for (auto& obj : object_layer->getObjects()) {
			
			Vector2 size = { obj.getSize().x*4,obj.getSize().y *4};
			Vector2 position = { obj.getPosition().x*4 + size.x/2,obj.getPosition().y*4 + size.y/2 };
			std::string name = obj.getName();
			std::string type = obj.getType();
		
			if (!type.compare("enemy")) {
				enemies e_type = Goomba;
				if (!name.compare("Goomba")) {
					e_type = Goomba;
				}
				if (!name.compare("Koopa")) {
					e_type = Koopa;
				}
			
				entities.push_back(Spawn_enemy({ position.x,position.y }, direc(obj.get<int>("dir")), e_type, obj.get<int>("color"), obj.get<bool>("flying")));
			
			}

			else if (!type.compare("block")) {
				Block_type B_T = static_block;
				items I_T = used;
				if (!name.compare("item_block")) {
					B_T = item_block;
				}
				else if (!name.compare("brick")) {
					B_T = brick;
				}
				else if (!name.compare("static")) {
					B_T = static_block;
				}
				else if (!name.compare("death")) {
					B_T = death;
				}
				else if (!name.compare("platform")) {
					B_T = platform;
				}
				else if (!name.compare("axe")) {
					B_T = axe;
				}
				else if (!name.compare("bridge")) {
					B_T = bridge;
				}
				else if (!name.compare("stop")) {
					B_T = stop;
				}

				if (!obj.get<std::string>("block_item").compare("none")) {
					I_T = used;
				}
				else if (!obj.get<std::string>("block_item").compare("mushroom") || !obj.get<std::string>("block_item").compare("flower")) {
					I_T = mushroom;
				}
				else if (!obj.get<std::string>("block_item").compare("one_up")) {
					I_T = one_up;
				}
				else if (!obj.get<std::string>("block_item").compare("star")) {
					I_T = power_star;
				}
				else if (!obj.get<std::string>("block_item").compare("coin")) {
					I_T = coin;
				}
				else if (!obj.get<std::string>("block_item").compare("multi_coin")) {
					I_T = multi_coin;
				}


 				entities.push_back(Spawn_block(B_T,{ position.x,position.y,size.x,size.y },I_T, obj.get<bool>("hidden"), obj.get<bool>("top"), obj.get<int>("color"), obj.get<bool>("is_edge")));
			}

			else if (!type.compare("item")) {
				items I_T = used;
				if (!name.compare("mushroom")) {
					I_T = mushroom;
				}
				else if (!name.compare("flower")) {
					I_T = fire_flower;
				}
				else if (!name.compare("star")) {
					I_T = power_star;
				}
				else if (!name.compare("coin")) {
					I_T = coin;
				}
				else if (!name.compare("one_up")) {
					I_T = one_up;
				}
				

				entities.push_back(Spawn_item(I_T, { position.x,position.y},obj.get<int>("color") ) );
			}

			else if(!type.compare("Warp")) {
				Vector2 Warp_cord = { obj.get<float>("New_x")*4, obj.get<float>("New_y")*4 };
				Vector2 Warp_cam = { obj.get<float>("New_cam_x")*4, obj.get<float>("New_cam_y")*4 };
				Vector2 Warp_speed = { obj.get<float>("New_speed_x"), obj.get<float>("New_speed_y") };
				Rectangle r = { position.x,position.y,size.x,size.y };
				setting s = sky;
				if (!obj.get<std::string>("New_region").compare("cave")) {
					s = cave;
				}
				else if (!obj.get<std::string>("New_region").compare("castle")) {
					s = castle;
				}
				entities.push_back(Spawn_Warp(Warp_cord,Warp_cam,s,r, obj.get<bool>("fix_cam"),obj.get<bool>("side"),Warp_speed ));
			}

			else if (!type.compare("Flag_pole")) {
				entities.push_back(Spawn_flag_pole({ position.x,position.y}, obj.get<int>("Flag_color")));
			}
			else if (!type.compare("Plant")) {
				entities.push_back(Spawn_Plant({position.x,position.y},obj.get<int>("color") ) );
			}
			else if (!type.compare("FireBar")) {
				entities.push_back(Spawn_FireBar({ position.x,position.y }, obj.get<int>("angle"), direc(obj.get<int>("spin_dir"))));
			}
			else if (!type.compare("Boss")) {
				entities.push_back(Spawn_Boss({position.x,position.y},direc(obj.get<int>("vel_dir"))));
				bowser_died = false;
			}
		}

	}

}
void newsweep(std::vector<edge>* edges, float dt) {

	for (int i = 0; i < edges->size(); i++) {
		if ((*edges)[i].is_left) {
			
			touching.push_back((*edges)[i].obj_index);
			if (touching.size() > 1) {
				for (int j = 0; j < touching.size() - 1; j++) {
					int index1 = (touching)[j];
					int index2 = (touching)[touching.size() - 1];
					
					float dy = entities[index1]->get_cord().y-entities[index2]->get_cord().y;
					float dx = entities[index1]->get_cord().x - entities[index2]->get_cord().x;
					if ( !( (entities)[index1]->get_type() == block && (entities)[index2]->get_type() == block )) {
						
						collision_pairs_x.push_back(Vector3{ float(index1),float(index2),sqrt(dx*dx + dy*dy)});
					}
				}
			}
			
		}
		else {
			auto it = find(touching.begin(), touching.end(), (*edges)[i].obj_index);
			
			int temp = (touching)[distance(touching.begin(), it)];
			(touching)[distance(touching.begin(), it)] = (touching)[touching.size() - 1];
			(touching)[touching.size() - 1] = temp;
			(touching).pop_back();
			
		}

		//printf("%d\n", i);
		

	}
}

void sweep(std::vector<ent*>* objs,float dt) {

	for (int i = 0; i < objs->size(); i++) {

		for (int j = i + 1; j < objs->size(); j++) {
			float max_x = (*objs)[i]->calc_int_max(0, dt);
			float min2_x = (*objs)[j]->calc_int_min(0, dt);
			

			if (max_x > min2_x) {
				if (!((*objs)[i]->get_type() == block && (*objs)[j]->get_type() == block)) {
					collision_pairs_x.push_back(Vector3{ float(i),float(j),fabsf(max_x - min2_x)});
				}
			}
			else {
				break;
			}
			
		}
	}
	

}


void clear_level(std::vector<ent*>* objs) {
	for (int i = 0; i < objs->size(); i++) {
		if ((*objs)[i]->get_type() != player) {
			delete((*objs)[i]);
		}
	}
	objs->clear();
}
int count_digits(int i) {
	int number ;
	int digits = 1;
	if (i < 0) {
		number = fabsf(i);
	}
	else if(i == 0) {
		return digits;
	}
	else {
		number = i;
	}
	while (number >= 10) {
		number /= 10;
		digits++;
	}
	return digits;
}
bool in_bounds(ent* e,Vector2 cam) {
	Vector2 pos = e->get_cord();
	if (pos.x + e->get_rect().width/2 >= cam.x - window_width * 0.5 && pos.x - e->get_rect().width / 2 <= cam.x + window_width * 0.5) {
		if (pos.y + e->get_rect().height / 2 >= cam.y - window_height * 0.5 && pos.y - e->get_rect().height / 2 <= cam.y + window_height * 0.5) {
			return true;
		}
		return false;
	}
	return false;
}
bool out_bounds(ent* e, Vector2 cam) {
	Vector2 pos = e->get_cord();
	if (pos.x - e->get_rect().width / 2 >= cam.x + window_width * 0.65 || pos.x + e->get_rect().width / 2 <= cam.x - window_width * 0.65) {
		if (pos.y + e->get_rect().height / 2 >= cam.y - window_height * 0.5 && pos.y - e->get_rect().height / 2 <= cam.y + window_height * 0.5) {
			return true;
		}
		return false;
	}
	return false;
}
void draw_player_stats(mario*m1,Font menu_text,Texture2D*menu_s,float menu_frame) {

	int zeros = 6;
	int s = m1->get_data().z;
	zeros -= count_digits(s);
	std::string score = "";
	std::string coins = "";
	std::string time = "";
	for (int i = 0; i < zeros; i++) {
		score += "0";
	}
	score += std::to_string(s);

	s = m1->get_data().y;
	zeros = 2 - count_digits(s);
	for (int i = 0; i < zeros; i++) {
		coins += "0";
	}
	coins += std::to_string(s);

	s = timer;
	zeros = 3 - count_digits(s);
	for (int i = 0; i < zeros; i++) {
		time += "0";
	}
	time += std::to_string(s);
	std::string r = std::to_string(current_level+1);
	char text[1024];
	char text1[1024];
	char text2[1024];
	char text3[2];
	strcpy_s(text, score.c_str());
	strcpy_s(text1, coins.c_str());
	strcpy_s(text2, time.c_str());
	strcpy_s(text3, r.c_str());
	  
	
	DrawTexturePro(*menu_s, { 207,0,208,16 }, { 96,64,208 * 4,64 }, { 0,0 }, 0, WHITE);
	if (menu_frame < 0) {
		DrawTexturePro(*menu_s, { 0,0,8,8 }, { 348,96,32,32 }, { 0,0 }, 0, WHITE);
	}
	else {
		DrawTexturePro(*menu_s, { float(8 * int(menu_frame)),0,8,8 }, { 348,96,32,32 }, { 0,0 }, 0, WHITE);
	}
	DrawTextPro(menu_text, text, { 96,96 }, { 0,0 }, 0, 36, 1, WHITE);
	DrawTextPro(menu_text, text1, { 412,96 }, { 0,0 }, 0, 36, 1, WHITE);
	DrawTextPro(menu_text, text2, { 820,96 }, { 0,0 }, 0, 36, 1, WHITE);
	DrawTextPro(menu_text, "1", {602,96}, {0,0}, 0, 36, 1, WHITE);
	DrawTextPro(menu_text, text3, { 682,96 }, { 0,0 }, 0, 36, 1, WHITE);
}

int main() {
	
	int frame_rules = 0;
	world_state game_state = start;
	
	timer = 50;
	
	char levels[4][80] = { "../assets/1-1.tmj" ,"../assets/1-2.tmj" ,"../assets/1-3.tmj", "../assets/1-4.tmj" };
	
	InitWindow(window_width, window_height, "mario bros");

	Image i1 = LoadImage("../assets/Player.png");
	Image i3 = LoadImage("../assets/Blocks.png");
	Image i2 = LoadImage("../assets/enemies.png");
	Image i4 = LoadImage("../assets/items.png");
	Image i5 = LoadImage("../assets/projectiles.png");
	Image i6 = LoadImage("../assets/Backround.png");
	Image i7 = LoadImage("../assets/menu.png");

	Texture2D player_s = LoadTextureFromImage(i1);
	Texture2D enemies_s = LoadTextureFromImage(i2);
	Texture2D block_s = LoadTextureFromImage(i3);;
	Texture2D items_s = LoadTextureFromImage(i4);
	Texture2D BackGround_s = LoadTextureFromImage(i6);
	Texture2D projectiles_s = LoadTextureFromImage(i5);
	Texture2D menu_s = LoadTextureFromImage(i7);

	SetTargetFPS(60);

	InitAudioDevice();

	Sound star_s = LoadSound("../assets/audio/star_theme.wav");
	Sound overworld_s = LoadSound("../assets/audio/overworld_theme.wav");
	Sound cave_s = LoadSound("../assets/audio/cave_theme.wav");
	Sound water_s = LoadSound("../assets/audio/underwwater_theme.wav");
	Sound castle_s = LoadSound("../assets/audio/castle_theme.wav");
	Sound coin_s = LoadSound("../assets/audio/coin.wav");
	SetMasterVolume(0.01);
	SetSoundVolume(coin_s, 10);
	SetSoundVolume(overworld_s, 10);
	SetSoundVolume(cave_s, 10);
	SetSoundVolume(water_s, 10);
	SetSoundVolume(castle_s, 30);

	Sound theme = overworld_s;

	Rectangle rec = { 500,600,100,100 };
	SetSoundVolume(star_s, 100);
	
	Font menu_text = LoadFontEx("../assets/Game_font.ttf", 128, 0, 250);

	Color c;
	c.r = 137;
	c.g = 110;
	c.b = 245;
	c.a = 0;

	ray r;
	r.point = { 600,600 };

	Vector2 end = { GetMousePosition() };

	

	mario* m1 = Spawn_player(Vector2{ 128,800 }, Vector2{ 0,0 }, Vector2{ 1,1 }, true, 292);

	float dt = GetFrameTime();
	float previous_time = 0;
	float current_time;
	float transition_time = 0;

	int options[1] = {0};
	SetTargetFPS(70);

	std::ifstream file = std::ifstream(levels[current_level]);
	Json::Value actual_json;
	
	Json::Reader reader;
	
	//reader.parse(file, actual_json);

	Json::FastWriter writer;
	actual_json = writer.write(levels[current_level]);
	
	Vector2 x_bounds = { 0,0 };
	Vector2 y_bounds = { 0,0 };
	
	
	tson::Tileson t;
	std::unique_ptr<tson::Map> level;
	level = t.parse(fs::path(levels[current_level]));
	
	float menu_frame = 0;
	
	cursor game_cursor;
	game_cursor.option = 0;
	
	Vector2 cord = { 200, 300 };
  

	Camera2D cam;
	cam.zoom = 1;
	cam.target = { window_width / 2,window_height / 2 };
	cam.offset = { window_width / 2 ,window_height / 2 };
	cam.rotation = 0;
	cam_cord = cam.target;
	int draw_it = 0;
	int rule_count = 0;

	bool reached = false;
	bool reached1 = false;
	bool reached2 = false;
	bool reached3 = false;
	

	while (true) {
		//printf("%d\n", int(entities.size()));

		current_time = GetTime();

		float dt = GetFrameTime();
		draw_it = 0;
		frame_count++;
		
		frame_rules = int(frame_count / frame_rule);
	
		menu_frame += 6 * dt;;
		if (menu_frame >= 4) {
			menu_frame = -2;
		}
		block_frame = menu_frame;
		if (m1->warp_timer >= 1.55 && m1->is_warping()) {
			cam.target = m1->new_cam_cord;
			level_setting = m1->enviroment;
		}
		
		if (WindowShouldClose()) {
			break;
		}
		x_bounds.x = cam.target.x - window_width * 0.5;
		x_bounds.y = cam.target.x + window_width * 0.5;
		y_bounds.x = cam.target.y - window_height * 0.5;
		y_bounds.y = cam.target.y + window_height * 0.5;
		switch (game_state) {
	
		case(playing_level): {
			frames++;
			player_cord = m1->get_cord();
			if (frame_count % frame_rule == 0) {
				if (frame_rules >= 1 && m1->get_stat() != dead && !m1->win && m1->get_stat() != climb) {
					timer--;
				}
				if (timer <= 0 && !m1->win && m1->get_stat() != dead && m1->lives > 0) {
					timer = 0;
					m1->set_stat(dead);
					m1->set_health(0);
					m1->pow = tiny;
					m1->update_rec();
					m1->set_dynamic(false);
					m1->lives--;
					Sound die = LoadSound("../assets/audio/mario_death.wav");
					SetSoundVolume(die, 10);
					PlaySound(die);
				}

			}
			if (m1->get_health() <= 0) {
				if (m1->dead_time > 4) {
					m1->lives -= 1;
					layers[0].clear();
					layers[1].clear();
					layers[2].clear();
					layers[3].clear();
					if (m1->lives <= 0) {
						game_state = game_over;
						transition_time = 0;
						Sound die = LoadSound("../assets/audio/game_over.wav");
						SetSoundVolume(die, 10);
						PlaySound(die);
						clear_level(&entities);
						cam.target = { window_width / 2,window_height / 2 };
						cam.offset = { window_width / 2,window_height / 2 };
						m1->reset(160, 800);
					}
					else {

						cam.target = { window_width / 2,window_height / 2 };
						cam.offset = { window_width / 2,window_height / 2 };
						game_state = transition;
						clear_level(&entities);
						m1->reset(160, 800);
					}

				}
				else {
					load_layers();
				}
			}
			if (m1->win) {
				if (fabsf(m1->get_speed().x) <= 5 && fabsf(m1->get_speed().y) <= 5 && m1->get_stat() != climb) {
					if (timer > 0 && !axe_struck) {
						timer--;
						score += 50;

						if (IsSoundPlaying(coin_s)) {
							StopSound(coin_s);
						}
						PlaySound(coin_s);
					}
					else if((timer <= 0 && !axe_struck) || (axe_struck && m1->win_timer >= 9)){
						rule_count += new_frame_rule();
						if (rule_count > 9) {
							flag_down = false;
							current_level++;
							
							if (current_level < 4) {
								game_state = transition;

							}
							else {
								game_state = start;
								current_level = 0;
								clear_level(&entities);
								m1->reset(160, 800);
								bridge_time = 0.1;
							}
							//file = std::ifstream(levels[current_level]);
							//reader.parse(file, actual_json);
							
							rule_count = 0;
							clear_level(&entities);
							cam.target = { window_width / 2,window_height / 2 };
							cam.offset = { window_width / 2,window_height / 2 };
							m1->reset(160, 800);
							
							break;
						}
					}

				}
			}
			if (m1->is_star() || !m1->music) {
				StopSound(theme);
			
				if (!IsSoundPlaying(star_s) && m1->music) {
					PlaySound(star_s);
				}
			}
			else {
				switch (level_setting) {
				case(sky): {

					theme = overworld_s;
					break;
				}
				case(cave): {


					theme = cave_s;
					break;
				}
				case(castle): {

					theme = castle_s;
					break;
				}
				case(water): {

					theme = water_s;
					break;
				}
				}

				if (!m1->is_warping() && m1->music) {
					if (!IsSoundPlaying(theme)) {
						PlaySound(theme);
					}
				}
				else {
					StopSound(star_s);
					StopSound(theme);
				}

				

				
			}

			
			
		
		

			insertionsort_obj(&entities, 0);
			


			for (int i = 0; i < entities.size(); i++) {
				if (entities[i]->started_cycle) {
					entities[i]->acc(dt);
				}

			}

			//broad-Phase en Narrow-Phase Collision detection

			collision_pairs_x.clear();

			contacts.clear();
			touching.clear();

			edges.clear();

			set_edges(&entities, &edges, y_axis, dt);

			newsweep(&edges, dt);


			quicksort_pairs(ptr, 0, collision_pairs_x.size() - 1);
			//reverse(collision_pairs_x.begin(), collision_pairs_x.end());





			//Drawing
		


			BeginDrawing();
			BeginMode2D(cam);
			ClearBackground(c);

			

			for (int i = 0; i < sky_tiles.size(); i++) {
				if (sky_tiles[i].dest.x <= cam.target.x + window_width * 0.8 && sky_tiles[i].dest.x >= cam.target.x - window_width * 0.8) {
					DrawTexturePro(BackGround_s, sky_tiles[i].source, { sky_tiles[i].dest.x,  sky_tiles[i].dest.y,  sky_tiles[i].dest.width * 2,  sky_tiles[i].dest.height * 2 }, { 0,0 }, 0, RAYWHITE);
				}
			}

			Json::FastWriter w;
	
			if ((m1->is_warping() || (m1->win && fabsf(m1->get_speed().x) <= 5 && m1->get_stat() != climb))) {
				m1->draw(&player_s, dt);
			}

		


			EndMode2D();
			draw_player_stats(m1, menu_text, &menu_s, menu_frame);
			BeginMode2D(cam);

		
			for (int i = 0; i < layers[1].size(); i++) {
				if (layers[1][i]->started_cycle) {
					switch (layers[1][i]->get_type()) {
					case(item): {
						layers[1][i]->draw(&items_s, dt);
						break;
					}
					case(plant): {
						layers[1][i]->draw(&enemies_s, dt);
						break;
					}
					}
					
				}
			}

			for (int i = 0; i < block_tiles.size(); i++) {
				if (block_tiles[i].dest.x <= cam.target.x + window_width * 0.8 && block_tiles[i].dest.x >= cam.target.x - window_width * 0.8) {
					DrawTexturePro(*block_tiles[i].tileset, block_tiles[i].source, block_tiles[i].dest, { 0,0 }, 0, RAYWHITE);
				}
			}

			for (int i = 0; i < layers[2].size(); i++) {
				if (layers[2][i]->started_cycle) {
					layers[2][i]->draw(&block_s, dt);
				}
			}

			for (int i = 0; i < layers[3].size(); i++) {
				if (layers[3][i]->started_cycle) {
					switch (layers[3][i]->get_type()) {

					case(enemy): case(boss) : {
						layers[3][i]->draw(&enemies_s, dt);
						break;
					}
					case(projectile): {
						layers[3][i]->draw(&projectiles_s, dt);
						break;
					}
					}
				}
				
			}
			
			if (!(m1->is_warping() || (m1->win && fabsf(m1->get_speed().x) <= 5 && m1->get_stat() != climb))) {
				m1->draw(&player_s, dt);
			}
			DrawCircle(cam.target.x, cam.target.y, 4, RED);
			

			for (int i = 0; i < layers[4].size(); i++) {
				layers[4][i]->draw(&menu_s, dt);
			}

			for (int i = 0; i < collision_pairs_x.size(); i++) {
				int index1 = collision_pairs_x[i].x;
				int index2 = collision_pairs_x[i].y;
				
					if (entities[index1]->get_type() == player) {

						entities[index1]->ProcessCollision(entities[index2], dt);
						entities[index2]->ProcessCollision(entities[index1], dt);

					}
					else if (entities[index2]->get_type() == player) {

						entities[index2]->ProcessCollision(entities[index1], dt);
						entities[index1]->ProcessCollision(entities[index2], dt);

					}
					else {
						if (entities[index1]->started_cycle && entities[index2]->started_cycle) {

							entities[index1]->ProcessCollision(entities[index2], dt);
							entities[index2]->ProcessCollision(entities[index1], dt);

						}
					}

				if (m1->get_cord().x - m1->get_rect().width / 2 < cam.target.x - window_width / 2) {
					m1->set_cord(cam.target.x - window_width / 2 + m1->get_rect().width / 2, m1->get_cord().y);
					m1->set_speedx(0);
				}
				
				if (bowser_died && axe_struck && fixed_cam) {
					fixed_cam = false;
					cam.target.x = m1->GetPosition().x - 100;
				}

				
			}

			EndMode2D();

			if (m1 != NULL && !fixed_cam && !m1->is_warping()) {
			
				if (m1->win && axe_struck && bowser_died) {
					
					if (cam.target.x - m1->GetPosition().x < 100) {
						printf("CAM\n");
						cam.target.x += 200 * dt;
						
					}
					if (m1->win_timer > 5.5) {
						char s[60] = "thank you mario!";
						DrawTextPro(menu_text,s, { window_width / 4,window_height / 3 }, { 0,18}, 0, 36, 0, WHITE);
						if (m1->win_timer > 6.5) {
							char s2[60] = "but your princess is in";
							char s3[60] = "another castle!";
							DrawTextPro(menu_text, s2, { window_width / 6,window_height / 3 + 80 }, { 0,18 }, 0, 36, 0, WHITE);
							DrawTextPro(menu_text, s3, { window_width / 6,window_height / 3 + 116 }, { 0,18 }, 0, 36, 0, WHITE);
						}
					}
				}
				else {
					
					if (cam.target.x + 100 < m1->GetPosition().x) {
						cam_cord = cam.target;
						cam.target.x = m1->GetPosition().x - 100;

					}
				}


			}
			
		
			
			EndDrawing();

			//spawning new entities

			for (int i = 0; i < entities.size(); i++) {
				if (entities[i]->spawn.type != none) {
					Rectangle r = entities[i]->get_rect();
					switch (entities[i]->spawn.type) {
					case(item): {

						if (!entities[i]->is_dynamic() || entities[i]->spawn.specify == coin) {
							
							if (entities[i]->spawn.specify != coin) {
								Sound item_sound = LoadSound("../assets/audio/item_rise.wav");
								SetSoundVolume(item_sound, 10);
								PlaySound(item_sound);
							}
							switch (entities[i]->spawn.specify) {

							case(mushroom): {
								entities.push_back(Spawn_item(mushroom, { entities[i]->get_rect().x ,entities[i]->get_rect().y }, 0));
								break;
							}
							case(fire_flower): {
								entities.push_back(Spawn_item(fire_flower, { entities[i]->get_rect().x ,entities[i]->get_rect().y }, 0));
								break;
							}
							case(power_star): {
								entities.push_back(Spawn_item(power_star, { entities[i]->get_rect().x ,entities[i]->get_rect().y }, 0));
								break;
							}
							case(one_up): {
								entities.push_back(Spawn_item(one_up, { entities[i]->get_rect().x ,entities[i]->get_rect().y }, 0));
								break;
							}

							case(coin): {

								entities.push_back(Spawn_projectile(spin_coin, { entities[i]->get_rect().x,entities[i]->get_rect().y - entities[i]->get_rect().height }, right, {0,0}, color1));



							}
							}
						
							entities[i]->spawn.type = none;


						}
						break;
					}
					case(projectile): {
						switch (entities[i]->spawn.specify) {
						case(brick_part): {
							
							int col = entities[i]->c;
							score += 50;
							entities.push_back(Spawn_projectile(brick_part, { r.x + r.width / 2 ,r.y - r.height / 2 }, right, { 200,-1000 },col));
							entities.push_back(Spawn_projectile(brick_part, { r.x - r.width / 2 ,r.y - r.height / 2 }, left, { 200,-1000 },col));
							entities.push_back(Spawn_projectile(brick_part, { r.x + r.width / 2 ,r.y + r.height / 2 }, right, { 200,-400 },col));
							entities.push_back(Spawn_projectile(brick_part, { r.x - r.width / 2 ,r.y + r.height / 2 }, left, { 200,-400 },col));

							entities[i]->spawn.type = none;
							break;
						}
						case(fire_ball): {
						
							direc d = direc(m1->get_dir());

							entities.push_back(Spawn_projectile(fire_ball, { r.x + float((r.width) * m1->get_dir()) ,r.y + r.height / 8 }, d,{0,0}, color1));

							entities[i]->spawn.specify = none;
							entities[i]->spawn.type = none;
							break;
						}
						case(dragon_breath): {

							

							entities.push_back(Spawn_projectile(dragon_breath, { r.x + float((r.width) * entities[i]->get_dir()) ,r.y - r.height / 5 }, direc(entities[i]->get_dir()) ,{0,0}, 0));

							entities[i]->spawn.specify = none;
							entities[i]->spawn.type = none;
							break;
						}
						}
						break;
					}
					case(text): {
						entities.push_back(Spawn_Score_text({ entities[i]->get_cord().x,entities[i]->get_cord().y - entities[i]->get_rect().height },score_count(entities[i]->spawn.specify)));

						entities[i]->spawn.type = none;

						break;
					}
					}
				
				}
				
			
				if (entities[i]->started_cycle) {
					entities[i]->update(dt);
				}
				
				if (entities[i]->get_type() == none || (out_bounds(entities[i], cam.target) && entities[i]->started_cycle) && entities[i]->get_type() != boss) {
					ent* temp = entities[i];
					remove_layer_object(temp);
					entities[i] = entities[entities.size() - 1];
					entities[entities.size() - 1] = temp;
					entities.pop_back();
					
					continue;
				}

				if (in_bounds(entities[i], cam.target) || entities[i]->get_type() == player) {
					entities[i]->started_cycle = true;

				}
			
			}

			if (axe_struck) {
				bridge_time -= dt;
			}

			load_layers();

			if (one_ups > 0) {
				m1->lives += one_ups;
				one_ups = 0;
				Sound grow_s = LoadSound("../assets/audio/one_up.wav");
				SetSoundVolume(grow_s, 10);
				PlaySound(grow_s);
			}
			
		
			break;
		}
		case(start): {
			if (IsKeyPressed(KEY_DOWN)) {
				if (game_cursor.option >= 1) {
					game_cursor.option += 0;
				}
				else {
					game_cursor.option += 1;
				}

			}
			if (IsKeyPressed(KEY_UP)) {
				if (game_cursor.option <= 0) {
					game_cursor.option -= 0;
				}
				else {
					game_cursor.option -= 1;
				}
			}
			BeginDrawing();
			ClearBackground(BLACK);
		    DrawTexturePro(menu_s,{0,32,256,240},{0,0,256*4,240*4},{0,0},0,WHITE);
			DrawTexturePro(menu_s, { 128,0,8,8 }, { 80*4,float(147.5*4 + 64 * game_cursor.option),32,32 }, { 16,16 }, 0, WHITE);
			if (menu_frame < 0) {
				DrawTexturePro(menu_s, { 0,0,8,8 }, { 348,96,32,32 }, { 0,0 }, 0, WHITE);
			}
			else {
				DrawTexturePro(menu_s, { float(8 * int(menu_frame)),0,8,8 }, { 348,96,32,32 }, { 0,0 }, 0, WHITE);
			}
			EndDrawing();
			if (IsKeyPressed(KEY_ENTER)) {
				game_state = transition;
				m1->lives = 3;
			}
			
			break;
		}
		case(transition): {
			timer = 400;
			transition_time += dt;
			int s = m1->get_data().x;
			std::string lives = std::to_string(s);
			std::string r = std::to_string(current_level + 1);
			char text[1024];
			char text3[1024];
			strcpy_s(text, lives.c_str());
			strcpy_s(text3, r.c_str());
			BeginDrawing();
			ClearBackground(BLACK);
			DrawTexturePro(menu_s, { 256,32,64,64 }, { window_width / 2,window_height / 2,256,256}, { 128,128 }, 0, WHITE);
			
			DrawTextPro(menu_text, text, { window_width / 2 + 64,window_height/2 }, { 0,0 }, 0, 36, 1, WHITE);
			DrawTextPro(menu_text, "1", {window_width / 2 + 64 ,window_height / 2 - 132}, {0,0}, 0, 36, 1, WHITE);
			DrawTextPro(menu_text, text3, { window_width / 2 + 130 ,window_height / 2 - 132}, { 0,0 }, 0, 36, 1, WHITE);
			draw_player_stats(m1, menu_text, &menu_s, menu_frame);
		
			EndDrawing();

			if (transition_time > 1) {
				//file = std::ifstream(levels[current_level]);
				//reader.parse(file, actual_json);
				level = t.parse(fs::path(levels[current_level]));
				if (entities.size() == 0) {
					entities.push_back(m1);
				}

				//load_objects(&actual_json, &writer);
				load_tiles(*level, &BackGround_s, &BackGround_s, &block_s);
				load_layers();
				transition_time = 0;
				
				game_state = playing_level;
				m1->music = true;
				m1->set_cord(Spawn_cords.x, Spawn_cords.y);
			
				break;
			}
			break;
		}
		case(game_over): {
			transition_time += dt;
			BeginDrawing();
			ClearBackground(BLACK);
			draw_player_stats(m1, menu_text, &menu_s, menu_frame);
			if (timer == 0) {
				DrawTexturePro(menu_s, { 256,95,56,8 }, { window_width / 2,window_height / 2,56 * 4,32 }, { 56 * 2,16 }, 0, WHITE);
			}
			else {
				DrawTexturePro(menu_s, { 256,103,72,8 }, { window_width / 2,window_height / 2,72 * 4,32 }, { 72 * 2,16 }, 0, WHITE);
			}
			EndDrawing();
			if (transition_time > 6) {
				game_state = start;
				transition_time = 0;
				score = 0;
				m1->lives = 3;
				break;
			}
			break;
		}
		}
		if (frame_count % frame_rule == 0) {

			frame_count = 0;
		}

	}
	CloseAudioDevice();
}
