// This header file calculates cluster cluster coefficients and average path length of the network.
// The network data 'neighbor' must be sorted in ascending order.

#ifndef NETWORK_EVALUATION_H
#define NETWORK_EVALUATION_H
#include <vector>

double Cluster_Coefficient(std::vector<std::vector<int>>* neighbor);
double Average_Path_Length(std::vector<std::vector<int>>* neighbor);

double Cluster_Coefficient(std::vector<std::vector<int>>* neighbor) {
	double CC = 0;
	int N = (*neighbor).size();
	for (int i = 0; i < N; i++) {
		int cc_number = 0;
		for (int j = 0; j < (*neighbor)[i].size() - 1; j++) {
			int nb1 = (*neighbor)[i][j];
			for (int q = j + 1; q < (*neighbor)[i].size(); q++) {
				int nb2 = (*neighbor)[i][q];
				for (int w = 0; w < (*neighbor)[nb1].size(); w++) if ((*neighbor)[nb1][w] == nb2) {
					cc_number++;
					break;
				}
			}
		}		
		double cc = double( cc_number ) / ( double( (*neighbor)[i].size() * ( (*neighbor)[i].size() - 1 ) ) / 2.0 );
		CC += cc;
	}
	CC /= double(N);
	return CC;
}

double Average_Path_Length(std::vector<std::vector<int>>* neighbor) {
	double APL = 0;
	int N = (*neighbor).size();
	int unsettled_size = N * (N - 1) / 2;
	std::vector<int> rem_size(N, N - 1);
	std::vector<std::vector<bool>> flag(N, std::vector<bool>(N, false));
	std::vector<std::vector<int>> lengh(N, std::vector<int>(N, 0));
	for (int i = 0; i < N; i++) flag[i][i] = true;
	for (int i = 0; i < N - 1; i++) for (int j = 0; j < (*neighbor)[i].size(); j++) {
		int nb = (*neighbor)[i][j];
		if (flag[i][nb] == false) {
			lengh[i][nb] = 1;
			lengh[nb][i] = 1;
			flag[i][nb] = true;
			flag[nb][i] = true;
			rem_size[i]--;
			rem_size[nb]--;
			unsettled_size--;
		}
	}
	std::vector<std::vector<int>> opponent = (*neighbor);
	int n = 2;
	while (unsettled_size > 0) {
		std::vector<std::vector<int>> next_opponent(N);
		for (int i = 0; i < N - 1; i++) if (rem_size[i] > 0) for (int j = 0; j < (*neighbor)[i].size(); j++) {
			int nb = (*neighbor)[i][j];
			for (int q = 0; q < opponent[nb].size(); q++) {
				int num = opponent[nb][q];
				if (flag[i][num] == false) {
					lengh[i][num] = n;
					lengh[num][i] = n;
					flag[i][num] = true;
					flag[num][i] = true;
					next_opponent[i].push_back(num);
					next_opponent[num].push_back(i);
					rem_size[i]--;
					rem_size[num]--;
					unsettled_size--;
				}
				if (rem_size[i] == 0) break;
			}
			if (rem_size[i] == 0) break;
		}
		opponent = next_opponent;
		n++;
	}
	for (int i = 0; i < N - 1; i++) for (int j = i + 1; j < N; j++) APL += lengh[i][j];
	APL /= double(N * (N - 1)) / 2.0;
	return APL;
}

#endif // !NETWORK_EVALUATION_H
