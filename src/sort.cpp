#include "../headers/sort.h"
void merge_combine_edges(std::vector<edge>* edges, int start, int middle, int end) {
	int index2;
	int index1;
	std::vector<edge> left(middle - start + 1);
	std::vector<edge> right(end - middle);

	for (int i = 0; i < left.size(); i++) {
		left[i] = (*edges)[start + i];
	}
	for (int i = 0; i < right.size(); i++) {
		right[i] = (*edges)[middle + 1 + i];
	}
	int i = 0, j = 0;
	int k = start;
	while (i < middle - start + 1 && j < end - middle) {
		if (left[i].x <= right[j].x) {
			(*edges)[k] = left[i];
			i++;
		}
		else if (left[i].x > right[j].x) {
			(*edges)[k] = right[j];
			j++;
		}
		k++;
	}
	for (int d = i; d < middle - start + 1; d++) {
		(*edges)[k] = left[d];
		k++;
	}
	for (int d = j; d < end - middle; d++) {
		(*edges)[k] = right[d];
		k++;
	}

}

void merge_sort_edges(std::vector<edge>* edges, int start, int end) {
	if (start >= end) {
		return;
	}
	int mid = start + (end - start) / 2;
	merge_sort_edges(edges, start, mid);
	merge_sort_edges(edges, mid + 1, end);
	merge_combine_edges(edges, start, mid, end);
}


void merge_combine_pairs(std::vector<Vector3>* pairs, int start, int middle, int end) {
	int index2;
	int index1;
	std::vector<Vector3> left(middle - start + 1);
	std::vector<Vector3> right(end - middle);
	for (int i = 0; i < left.size(); i++) {
		left[i] = (*pairs)[start + i];
	}
	for (int i = 0; i < right.size(); i++) {
		right[i] = (*pairs)[middle + 1 + i];
	}
	int i = 0, j = 0;
	int k = start;
	while (i < middle - start + 1 && j < end - middle) {
		if (left[i].z <= right[j].z) {
			(*pairs)[k] = left[i];
			i++;
		}
		else if (left[i].z > right[j].z) {
			(*pairs)[k] = right[j];
			j++;
		}
		k++;
	}
	for (int d = i; d < middle - start + 1; d++) {
		(*pairs)[k] = left[d];
		k++;
	}
	for (int d = j; d < end - middle; d++) {
		(*pairs)[k] = right[d];
		k++;
	}

}
void insertionsort_edges(std::vector<edge>* edges) {
	edge temp;

	for (int i = 1; i < edges->size(); i++) {
		temp = (*edges)[i];
		int j = i - 1;
		while (j >= 0 && (*edges)[j].x > temp.x) {
			   
			(*edges)[j + 1] = (*edges)[j];
			j--;
		}
		(*edges)[j + 1] = temp;
	}


}
void set_edges(std::vector<ent*>* entities, std::vector<edge>* edges,bool axis, float dt) {
	switch (axis) {
	case(0): {
		for (int i = 0; i < entities->size(); i++) {
			edge left;
			edge right;
			left.x = (*entities)[i]->calc_int_min(0, dt);
			left.is_left = true;
			left.obj_index = i;

			right.obj_index = i;
			right.is_left = false;
			right.x = (*entities)[i]->calc_int_max(0, dt);

			edges->push_back(left);
			edges->push_back(right);

		}
		break;
	}
	case(1): {
		for (int i = 0; i < entities->size(); i++) {
			edge left;
			edge right;
			left.x = (*entities)[i]->calc_int_min(1, dt);
			left.is_left = true;
			left.obj_index = i;

			right.obj_index = i;
			right.is_left = false;
			right.x = (*entities)[i]->calc_int_max(1, dt);

			edges->push_back(left);
			edges->push_back(right);

		}
		break;
	}
	}
	merge_sort_edges(edges, 0, edges->size() - 1);
}

void merge_sort_pairs(std::vector<Vector3>* pairs, int start, int end) {
	if (start >= end) {
		return;
	}
	int mid = start + (end - start) / 2;
	merge_sort_pairs(pairs, start, mid);
	merge_sort_pairs(pairs, mid + 1, end);
	merge_combine_pairs(pairs, start, mid, end);
}

void merge_combine(std::vector<ent*>* pairs, int start, int middle, int end) {
	int index2;
	int index1;
	std::vector<ent*> left(middle - start + 1);
	std::vector<ent*> right(end - middle);
	for (int i = 0; i < left.size(); i++) {
		left[i] = (*pairs)[start + i];
	}
	for (int i = 0; i < right.size(); i++) {
		right[i] = (*pairs)[middle + 1 + i];
	}
	int i = 0, j = 0;
	int k = start;
	while (i < middle - start + 1 && j < end - middle) {
		if (left[i]->get_rect().x <= right[j]->get_rect().x) {
			(*pairs)[k] = left[i];
			i++;
		}
		else if (left[i]->get_rect().x > right[j]->get_rect().x) {
			(*pairs)[k] = right[j];
			j++;
		}
		k++;
	}
	for (int d = i; d < middle - start + 1; d++) {
		(*pairs)[k] = left[d];
		k++;
	}
	for (int d = j; d < end - middle; d++) {
		(*pairs)[k] = right[d];
		k++;
	}

}

void merge_sort(std::vector<ent*>* objs, int start, int end) {
	if (start >= end) {
		return;
	}
	int mid = start + (end - start) / 2;
	merge_sort(objs, start, mid);
	merge_sort(objs, mid + 1, end);
	merge_combine(objs, start, mid, end);
}

int partition(std::vector<Vector3>* pairs, int start, int end) {
	int i = start - 1;
	float pivot;
	Vector3 temp;

	pivot = (*pairs)[end].z;
	for (int j = start; j < end; j++) {
		if ((*pairs)[j].z < pivot) {
			i++;
			temp = (*pairs)[i];
			(*pairs)[i] = (*pairs)[j];
			(*pairs)[j] = temp;
		}
	}


	i++;
	temp = (*pairs)[end];
	(*pairs)[end] = (*pairs)[i];
	(*pairs)[i] = temp;
	return i;
}

int partition(std::vector<ent*>* objs, int axis, int start, int end) {
	int i = start - 1;
	float pivot;
	ent* temp;
	switch (axis) {
	case(0): {
		pivot = (*objs)[end]->get_rect().x;
		for (int j = start; j < end; j++) {
			if ((*objs)[j]->get_rect().x < pivot) {
				i++;
				temp = (*objs)[i];
				(*objs)[i] = (*objs)[j];
				(*objs)[j] = temp;
			}
		}
		break;

	case(1): {
		pivot = (*objs)[end]->get_rect().y;
		for (int j = start; j < end; j++) {
			if ((*objs)[j]->get_rect().y < pivot) {
				i++;
				temp = (*objs)[i];
				(*objs)[i] = (*objs)[j];
				(*objs)[j] = temp;
			}
		}
		break;
	}
	}
	}
	i++;
	temp = (*objs)[end];
	(*objs)[end] = (*objs)[i];
	(*objs)[i] = temp;
	return i;
}

void quicksort_obj(std::vector<ent*>* objs, int axis, int start, int end) {
	if (end <= start) {
		return;
	}
	int pivot = partition(objs, axis, start, end);
	quicksort_obj(objs, axis, start, pivot - 1);
	quicksort_obj(objs, axis, pivot + 1, end);
}

void insertionsort_obj(std::vector<ent*>* objs, int axis) {
	ent* temp;
	switch (axis) {
	case(0): {
		for (int i = 1; i < objs->size(); i++) {
			temp = (*objs)[i];
			int j = i - 1;
			while (j >= 0 && (*objs)[j]->get_rect().x > temp->get_rect().x) {
				(*objs)[j + 1] = (*objs)[j];
				j--;
			}
			(*objs)[j + 1] = temp;
		}
		break;
	}
	case(1): {
		for (int i = 1; i < objs->size(); i++) {
			temp = (*objs)[i];
			int j = i - 1;
			while (j >= 0 && (*objs)[j]->get_rect().y > temp->get_rect().x) {
				(*objs)[j + 1] = (*objs)[j];
				j--;
			}
			(*objs)[j + 1] = temp;
		}
		break;
		break;
	}
	}
}

void quicksort_pairs(std::vector<Vector3>* pairs, int start, int end) {
	if (end <= start) {
		return;
	}
	int pivot = partition(pairs, start, end);
	quicksort_pairs(pairs, start, pivot - 1);
	quicksort_pairs(pairs, pivot + 1, end);
}