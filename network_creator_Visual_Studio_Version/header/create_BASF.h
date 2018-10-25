// This header file creates Barabasi Albert-Laszlo network.

#ifndef CREATE_BASF_H
#define CREATE_BASF_H
#include <iostream>
#include <vector>
#include "random.h"
#include "sort.h"

struct Initial {
	int initial_nodes;
	int added_branch;
};

void creating_BASF(int N, int k, std::vector<std::vector<int>>* neighbor);
Initial initial_setting_BASF(int N, int k);
int create_initial_network_BASF(int N, std::vector<std::vector<int>>* neighbor, Initial initial);
std::vector<int> select_nodes_BASF(int number, std::vector<std::vector<int>>* neighbor, int added_branch, int all_degree);

void creating_BASF(int N, int k, std::vector<std::vector<int>>* neighbor) {
	std::vector<std::vector<int>> _neighbor;
	Initial initial = initial_setting_BASF(N, k);
	int all_degree = create_initial_network_BASF(N, &_neighbor, initial);
	for (int i = initial.initial_nodes; i < N; i++) {
		std::vector<int> selected_nodes = select_nodes_BASF(i, &_neighbor, initial.added_branch, all_degree);
		for (int j = 0; j < selected_nodes.size(); j++) {
			int num = selected_nodes[j];
			_neighbor[i].push_back(num);
			_neighbor[num].push_back(i);
			all_degree += 2;
		}
	}
	(*neighbor) = _neighbor;
}

Initial initial_setting_BASF(int N, int k) {
	Initial initial;
	if (k % 2 != 0) {
		std::cout << "ERROR create_BASF.h" << std::endl;
		std::cout << "k is not a multiple of two" << std::endl;
		exit(EXIT_SUCCESS);
	}
	initial.initial_nodes = k + 1;
	initial.added_branch = k / 2;
	if (initial.initial_nodes < initial.added_branch) {
		std::cout << "ERROR create_BASF.h or create_HCSF.h" << std::endl;
		std::cout << "number of initial nodes is smaller than the added branch" << std::endl;
		exit(EXIT_SUCCESS);
	}
	return initial;
}

int create_initial_network_BASF(int N, std::vector<std::vector<int>>* neighbor, Initial initial) {
	int all_degree = 0;
	std::vector<std::vector<int>> _neighbor(N);
	for (int i = 0; i < initial.initial_nodes; i++) for (int j = 0; j < initial.initial_nodes; j++) if (i != j) {
		_neighbor[i].push_back(j);
		all_degree++;
	}
	(*neighbor) = _neighbor;
	return all_degree;
}

std::vector<int> select_nodes_BASF(int number, std::vector<std::vector<int>>* neighbor, int added_branch, int all_degree) {
	Sort sort;
	std::vector<int> selected_nodes;
	std::vector<int> remaining_people;
	for (int i = 0; i < number; i++) remaining_people.push_back(i);
	while (selected_nodes.size() < added_branch) {
		double xp = random(1.0);
		double probability = 0;
		for (int i = 0; i < remaining_people.size(); i++) {
			int num = remaining_people[i];
			probability += double((*neighbor)[num].size()) / double(all_degree);
			if (xp <= probability) {
				selected_nodes.push_back(num);
				all_degree -= (*neighbor)[num].size();
				remaining_people.erase(remaining_people.begin() + i);
				break;
			}
		}
	}
	selected_nodes = sort.mergesort.ascendingsort_data(selected_nodes);
	return selected_nodes;
}

#endif // !CREATE_BASF_H