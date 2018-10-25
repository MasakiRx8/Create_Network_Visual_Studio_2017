// This header file creates the Small World network.
// This network forms shortcut links based on the Lattice network.

#ifndef CREATE_SW_H
#define CREATE_SW_H
#include <iostream>
#include <vector>
#include "random.h"
#include "sort.h"
#include "create_Lattice.h"

struct Branch {
	int left;
	int right;
};

class Agent_SW {
public:
	std::vector<std::vector<int>> normal;
	std::vector<std::vector<int>> neighbor;
	std::vector<Branch> branch;
};

void creating_SW(int N, int k, std::vector<std::vector<int>>* neighbor, double p);
void create_branch_list_SW(int N, Agent_SW* agent);
void create_shortcut_SW(int N, Agent_SW* agent, double p);
bool select_nodes_SW(int num1, int num2, int* s1, int* s2, int* o1, int* o2, Agent_SW* agent);
void connect_neighbor_SW(int s1, int s2, int o1, int o2, Agent_SW* agent);

void creating_SW(int N, int k, std::vector<std::vector<int>>* neighbor, double p) {
	Sort sort;
	int sqrt_N = int(sqrt(N));
	if (sqrt_N * sqrt_N != N) {
		std::cout << "ERROR create_SW.h" << std::endl;
		std::cout << "N is not an integer squared" << std::endl;
		exit(EXIT_SUCCESS);
	}
	Agent_SW agent;
	creating_Lattice(N, k, &agent.normal);
	agent.neighbor = agent.normal;
	create_branch_list_SW(N, &agent);
	create_shortcut_SW(N, &agent, p);
	for (int i = 0; i < N; i++) agent.neighbor[i] = sort.mergesort.ascendingsort_data(agent.neighbor[i]);
	(*neighbor) = agent.neighbor;
}

void create_branch_list_SW(int N, Agent_SW* agent) {
	std::vector<Branch> branch;
	for (int i = 0; i < N; i++) for (int j = 0; j < agent->neighbor[i].size(); j++) {
		int nb = agent->neighbor[i][j];
		if (i < nb) {
			Branch b;
			b.left = i;
			b.right = nb;
			branch.push_back(b);
		}
	}
	agent->branch = branch;
}

void create_shortcut_SW(int N, Agent_SW* agent, double p) {
	create_branch_list_SW(N, agent);
	int selected_number = agent->branch.size() * p;
	if (selected_number % 2 == 1) selected_number + 1;
	if (selected_number > agent->branch.size()) selected_number = agent->branch.size();
	std::vector<int> remaining_number;
	for (int i = 0; i < agent->branch.size(); i++) remaining_number.push_back(i);
	int selected = 0;
	while (selected < selected_number) {
		int rem_size = remaining_number.size();
		int xrem1 = random(rem_size - 1);
		int xrem2 = random(rem_size - 1);
		while (xrem1 == xrem2) xrem2 = random(rem_size - 1);
		int num1 = remaining_number[xrem1];
		int num2 = remaining_number[xrem2];
		int s1, s2;
		int o1, o2;
		bool fg = select_nodes_SW(num1, num2, &s1, &s2, &o1, &o2, agent);
		if (fg == true) {
			connect_neighbor_SW(s1, s2, o1, o2, agent);
			if (xrem1 > xrem2) {
				remaining_number.erase(remaining_number.begin() + xrem1);
				remaining_number.erase(remaining_number.begin() + xrem2);
			}
			else {
				remaining_number.erase(remaining_number.begin() + xrem2);
				remaining_number.erase(remaining_number.begin() + xrem1);
			}
			selected += 2;
		}
		if (remaining_number.size() == 0) break;
	}
}

bool select_nodes_SW(int num1, int num2, int* s1, int* s2, int* o1, int* o2, Agent_SW* agent) {
	std::vector<int> v(4);
	for (int i = 0; i < 4; i++) v[i] = i;
	bool fg = false;
	while (fg == false) {
		int v_size = v.size();
		int xv = random(v_size - 1);
		int numv = v[xv];
		v.erase(v.begin() + xv);
		switch (numv) {
		case 0:
			(*s1) = agent->branch[num1].left;
			(*s2) = agent->branch[num2].left;
			(*o1) = agent->branch[num1].right;
			(*o2) = agent->branch[num2].right;
			break;
		case 1:
			(*s1) = agent->branch[num1].left;
			(*s2) = agent->branch[num2].right;
			(*o1) = agent->branch[num1].right;
			(*o2) = agent->branch[num2].left;
			break;
		case 2:
			(*s1) = agent->branch[num1].right;
			(*s2) = agent->branch[num2].left;
			(*o1) = agent->branch[num1].left;
			(*o2) = agent->branch[num2].right;
			break;
		case 3:
			(*s1) = agent->branch[num1].right;
			(*s2) = agent->branch[num2].right;
			(*o1) = agent->branch[num1].left;
			(*o2) = agent->branch[num2].left;
			break;
		default:
			break;
		}
		bool flag = true;
		if ((*s1) == (*s2)) flag = false;
		if (flag == true) if ((*o1) == (*o2)) flag = false;
		if (flag == true) if ((*s1) != (*s2)) {
			for (int i = 0; i < agent->normal[(*s1)].size(); i++) if (agent->normal[(*s1)][i] == (*s2)) {
				flag = false;
				break;
			}
			if (flag == true) for (int i = 0; i < agent->neighbor[(*s1)].size(); i++) if (agent->neighbor[(*s1)][i] == (*s2)) {
				flag = false;
				break;
			}
			if (flag == true) for (int i = 0; i < agent->neighbor[(*s2)].size(); i++) if (agent->neighbor[(*s2)][i] == (*s1)) {
				flag = false;
				break;
			}
		}
		if (flag == true) if ((*o1) != (*o2)) {
			for (int i = 0; i < agent->normal[(*o1)].size(); i++) if (agent->normal[(*o1)][i] == (*o2)) {
				flag = false;
				break;
			}
			if (flag == true) for (int i = 0; i < agent->neighbor[(*o1)].size(); i++) if (agent->neighbor[(*o1)][i] == (*o2)) {
				flag = false;
				break;
			}
			if (flag == true) for (int i = 0; i < agent->neighbor[(*o2)].size(); i++) if (agent->neighbor[(*o2)][i] == (*o1)) {
				flag = false;
				break;
			}
		}
		if (flag == true) fg = true;
		if (v.size() == 0) break;
	}
	return fg;
}

void connect_neighbor_SW(int s1, int s2, int o1, int o2, Agent_SW* agent) {
	std::vector<int> nb1 = agent->neighbor[s1];
	std::vector<int> nb2 = agent->neighbor[s2];
	std::vector<int> nbo1 = agent->neighbor[o1];
	std::vector<int> nbo2 = agent->neighbor[o2];
	for (int i = 0; i < nb1.size(); i++) if (nb1[i] == o1) {
		nb1.erase(nb1.begin() + i);
		break;
	}
	for (int i = 0; i < nb2.size(); i++) if (nb2[i] == o2) {
		nb2.erase(nb2.begin() + i);
		break;
	}
	for (int i = 0; i < nbo1.size(); i++) if (nbo1[i] == s1) {
		nbo1.erase(nbo1.begin() + i);
		break;
	}
	for (int i = 0; i < nbo2.size(); i++) if (nbo2[i] == s2) {
		nbo2.erase(nbo2.begin() + i);
		break;
	}
	nb2.push_back(s1);
	nb1.push_back(s2);
	nbo1.push_back(o2);
	nbo2.push_back(o1);
	agent->neighbor[s1] = nb1;
	agent->neighbor[s2] = nb2;
	agent->neighbor[o1] = nbo1;
	agent->neighbor[o2] = nbo2;
}

#endif // !CREATE_SW_H