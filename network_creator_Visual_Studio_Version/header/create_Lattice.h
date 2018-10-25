// This header file creates the Lattice network.

#ifndef CREATE_LATTICE_H
#define CREATE_LATTICE_H
#include <iostream>
#include <vector>
#include <math.h>
#include "sort.h"

struct List {
	int x;
	int y;
};

void creating_Lattice(int N, int k, std::vector<std::vector<int>>* neighbor);
void creating_neighbor_list_Lattice(int sqrt_N, std::vector<std::vector<int>>* neighbor_list);
void connect_neighbor_Lattice(int N, int sqrt_N, int k, std::vector<std::vector<int>>* neighbor);

void creating_Lattice(int N, int k, std::vector<std::vector<int>>* neighbor) {
	int sqrt_N = int(sqrt(N));
	if (sqrt_N * sqrt_N != N) {
		std::cout << "ERROR create_Lattice.h" << std::endl;
		std::cout << "N is not an integer squared" << std::endl;
		exit(EXIT_SUCCESS);
	}
	switch (k) {
	case 4:
	case 8:
	case 12:
		break;
	default:
		std::cout << "ERROR create_Lattice.h" << std::endl;
		std::cout << "not handle " << k << std::endl;
		exit(EXIT_SUCCESS);
	}
	connect_neighbor_Lattice(N, sqrt_N, k, neighbor);
}

void creating_neighbor_list_Lattice(int sqrt_N, std::vector<std::vector<int>>* neighbor_list) {
	std::vector<std::vector<int>> _neighbor_list;
	int n = 0;
	for (int i = 0; i < sqrt_N; i++) {
		std::vector<int> nb;
		for (int j = 0; j < sqrt_N; j++) {
			nb.push_back(n);
			n++;
		}
		int nb_last = nb[nb.size() - 1];
		int nb_slast = nb[nb.size() - 2];
		int nb_start = nb[0];
		int nb_sstart = nb[1];
		nb.insert(nb.begin(), nb_last);
		nb.insert(nb.begin(), nb_slast);
		nb.push_back(nb_start);
		nb.push_back(nb_sstart);
		_neighbor_list.push_back(nb);
	}
	std::vector<int> nblast = _neighbor_list[_neighbor_list.size() - 1];
	std::vector<int> nbslast = _neighbor_list[_neighbor_list.size() - 2];
	std::vector<int> nbstart = _neighbor_list[0];
	std::vector<int> nbsstart = _neighbor_list[1];
	_neighbor_list.insert(_neighbor_list.begin(), nblast);
	_neighbor_list.insert(_neighbor_list.begin(), nbslast);
	_neighbor_list.push_back(nbstart);
	_neighbor_list.push_back(nbsstart);
	(*neighbor_list) = _neighbor_list;
}

void connect_neighbor_Lattice(int N, int sqrt_N, int k, std::vector<std::vector<int>>* neighbor) {
	Sort sort;
	std::vector<std::vector<int>> _neighbor(N);
	std::vector<std::vector<int>> neighbor_list;
	creating_neighbor_list_Lattice(sqrt_N, &neighbor_list);
	for (int i = 0; i < sqrt_N; i++) for (int j = 0; j < sqrt_N; j++) {
		List list;
		list.x = j + 2;
		list.y = i + 2;
		int n_num = neighbor_list[list.y][list.x];
		std::vector<int> nb;
		for (int q = 0; q < k; q++) {
			List l;
			switch (q) {
			case 0:
				l.x = list.x;
				l.y = list.y - 1;
				break;
			case 1:
				l.x = list.x + 1;
				l.y = list.y;
				break;
			case 2:
				l.x = list.x;
				l.y = list.y + 1;
				break;
			case 3:
				l.x = list.x - 1;
				l.y = list.y;
				break;
			case 4:
				l.x = list.x + 1;
				l.y = list.y - 1;
				break;
			case 5:
				l.x = list.x + 1;
				l.y = list.y + 1;
				break;
			case 6:
				l.x = list.x - 1;
				l.y = list.y + 1;
				break;
			case 7:
				l.x = list.x - 1;
				l.y = list.y - 1;
				break;
			case 8:
				l.x = list.x;
				l.y = list.y - 2;
				break;
			case 9:
				l.x = list.x + 2;
				l.y = list.y;
				break;
			case 10:
				l.x = list.x;
				l.y = list.y + 2;
				break;
			case 11:
				l.x = list.x - 2;
				l.y = list.y;
				break;
			default:
				std::cout << "ERROR create_Lattice.h" << std::endl;
				exit(EXIT_SUCCESS);
				break;
			}
			int nb_num = neighbor_list[l.y][l.x];
			nb.push_back(nb_num);
		}
		_neighbor[n_num] = sort.mergesort.ascendingsort_data(nb);
	}
	(*neighbor) = _neighbor;
}

#endif // !CREATE_LATTICE_H