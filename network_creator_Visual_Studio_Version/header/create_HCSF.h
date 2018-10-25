// This header file creates the Highly clustered scale-free networks. 
// please visit this URL website. https://archive.org/details/arxiv-cond-mat0107606/page/n0

#ifndef CREATE_HCSF_H
#define CREATE_HCSF_H
#include <iostream>
#include <vector>
#include "random.h"
#include "sort.h"
#include "create_BASF.h"

class Agent_HCSF {
public:
	std::vector<std::vector<int>> neighbor;
	std::vector<bool> inactive;
	std::vector<double> probability_inactive;
};

class Information {
public:
	int all_degree;
	int all_degree_active;
	double all_probability_inactive;
	std::vector<int> active_agent;
};

class AI {
public:
	Agent_HCSF agent;
	Information information;
	Initial initial;
};

void creating_HCSF(int N, int k, std::vector<std::vector<int>>* neighbor, double p);
void creating_initial_network_HCSF(int N, int k, AI* ai);
std::vector<int> select_selected_nodes_default(int n, AI* ai);
std::vector<int> select_selected_nodes_inactive(int n, AI* ai);
void select_inactived_nodes(AI* ai);

void creating_HCSF(int N, int k, std::vector<std::vector<int>>* neighbor, double p) {
	if (k % 2 != 0) {
		std::cout << "ERROR create_HCSF.h" << std::endl;
		std::cout << "k is not a multiple of two" << std::endl;
		exit(EXIT_SUCCESS);
	}
	AI ai;
	creating_initial_network_HCSF(N, k, &ai);
	for (int i = ai.initial.initial_nodes; i < N; i++) {
		double x = random(1.0);
		// add a new node
		std::vector<int> select_agent;
		if (x < p) select_agent = select_selected_nodes_default(i, &ai);
		else select_agent = select_selected_nodes_inactive(i, &ai);
		std::vector<int> neighbor;
		for (int j = 0; j < select_agent.size(); j++) {
			int num = select_agent[j];
			neighbor.push_back(num);
			ai.information.all_degree += 2;
			ai.information.all_degree_active++;
			std::vector<int> nb = ai.agent.neighbor[num];
			nb.push_back(i);
			ai.agent.neighbor[num] = nb;
			if (ai.agent.inactive[num] == false) {
				ai.information.all_degree_active++;
				ai.information.all_probability_inactive += 1.0 / double(ai.agent.neighbor[num].size() + ai.initial.added_branch) - 1.0 / double(ai.agent.neighbor[num].size() - 1 + ai.initial.added_branch);
			}
		}
		ai.agent.neighbor[i] = neighbor;
		ai.information.all_probability_inactive += 1.0 / double(ai.agent.neighbor[i].size() + ai.initial.added_branch);
		// select node that change to inactive
		ai.information.active_agent.push_back(i);
		select_inactived_nodes(&ai);
	}
	(*neighbor) = ai.agent.neighbor;
}

void creating_initial_network_HCSF(int N, int k, AI* ai) {
	ai->agent.neighbor = std::vector<std::vector<int>>(N);
	ai->agent.inactive = std::vector<bool>(N, false);
	ai->agent.probability_inactive = std::vector<double>(N, 0);
	ai->information.all_degree = 0;
	ai->information.all_degree_active = 0;
	ai->information.all_probability_inactive = 0;
	ai->initial = initial_setting_BASF(N, k);
	std::vector<int> active_agent;
	for (int i = 0; i < ai->initial.initial_nodes; i++) {
		active_agent.push_back(i);
		std::vector<int> neighbor;
		for (int j = 0; j < ai->initial.initial_nodes; j++) if (i != j) {
			neighbor.push_back(j);
			ai->information.all_degree++;
			ai->information.all_degree_active++;
		}
		ai->agent.neighbor[i] = neighbor;
		ai->information.active_agent = active_agent;
		ai->information.all_probability_inactive += 1.0 / double(ai->agent.neighbor[i].size() + ai->initial.added_branch);
	}
}

std::vector<int> select_selected_nodes_default( int n, AI* ai ) { return select_nodes_BASF( n, &ai->agent.neighbor, ai->initial.added_branch, ai->information.all_degree ); }

std::vector<int> select_selected_nodes_inactive(int n, AI* ai) {
	Sort sort;
	std::vector<int> selected_nodes;
	std::vector<int> remaining_people = ai->information.active_agent;
	std::vector<int> active_agent;
	for (int i = 0; i < n; i++) if (ai->agent.inactive[i] == false) active_agent.push_back(i);
	int all_probability = ai->information.all_degree_active;
	while (selected_nodes.size() < ai->initial.added_branch) {
		double x = random(1.0);
		double probability = 0;
		for (int i = 0; i < remaining_people.size(); i++) {
			int num = remaining_people[i];
			probability += double(ai->agent.neighbor[num].size()) / double(all_probability);
			if (x <= probability) {
				selected_nodes.push_back(num);
				remaining_people.erase(remaining_people.begin() + i);
				all_probability -= ai->agent.neighbor[num].size();
				break;
			}
		}
	}
	selected_nodes = sort.quicksort.descendingsort_data(selected_nodes);
	return selected_nodes;
}

void select_inactived_nodes(AI* ai) {
	double x = random(1.0);
	double probability = 0;
	std::vector<int> active_agent = ai->information.active_agent;
	for (int i = 0; i < active_agent.size(); i++) {
		int num = active_agent[i];
		probability += 1.0 / double(ai->agent.neighbor[num].size() + ai->initial.added_branch) / ai->information.all_probability_inactive;
		if (x <= probability) {
			ai->agent.inactive[num] = true;
			active_agent.erase(active_agent.begin() + i);
			ai->information.all_probability_inactive -= 1.0 / double(ai->agent.neighbor[num].size() + ai->initial.added_branch);
			ai->information.all_degree_active -= ai->agent.neighbor[num].size();
			break;
		}
	}
	ai->information.active_agent = active_agent;
}

#endif // !CREATE_HCSF_H