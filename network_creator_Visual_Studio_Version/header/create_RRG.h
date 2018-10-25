// This header file creates the network of Regular random graph.

#ifndef CREATE_RRG_H
#define CREATE_RRG_H
#include <iostream>
#include <vector>
#include "sort.h"

void creating_RRG(int N, int k, std::vector<std::vector<int>>* neighbor) {
	Sort sort;
	if (k % 2 != 0) {
		std::cout << "ERROR create_RRG.h" << std::endl;
		std::cout << "k is not a multiple of two" << std::endl;
		exit(EXIT_SUCCESS);
	}
	std::vector<std::vector<int>> _neighbor(N);
	for (int i = 0; i < N; i++) {
		std::vector<int> nb;
		for (int j = int(-k / 2); j <= int(k / 2); j++) if (j != 0) {
			int num = i + j;
			if (num < 0) num += N;
			if (num >= N) num -= N;
			nb.push_back(num);
		}
		_neighbor[i] = sort.mergesort.ascendingsort_data(nb);
	}
	(*neighbor) = _neighbor;
}

#endif // !CREATE_RRG_H