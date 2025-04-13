#pragma once
#include <vector>
#include "ent.h"
struct edge {
	float x;
	int obj_index;
	bool is_left;
};

void merge_combine_edges(std::vector<edge>* edges, int start, int middle, int end);

void merge_sort_edges(std::vector<edge>* edges, int start, int end);

void merge_combine_pairs(std::vector<Vector3>* pairs, int start, int middle, int end);

void insertionsort_edges(std::vector<edge>* edges);

void set_edges(std::vector<ent*>* entities, std::vector<edge>* edges, bool axis, float dt);

void merge_sort_pairs(std::vector<Vector3>* pairs, int start, int end);

void merge_combine(std::vector<ent*>* pairs, int start, int middle, int end);

void merge_sort(std::vector<ent*>* objs, int start, int end);

int partition(std::vector<Vector3>* pairs, int start, int end);

int partition(std::vector<ent*>* objs, int axis, int start, int end);

void quicksort_obj(std::vector<ent*>* objs, int axis, int start, int end);

void insertionsort_obj(std::vector<ent*>* objs, int axis);

void quicksort_pairs(std::vector<Vector3>* pairs, int start, int end);