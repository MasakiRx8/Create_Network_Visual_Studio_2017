/*
	This program outputs the specified network as a dat file.
	Also, this program outputs cluster cluster coefficients and average path length of the network.
	It outputs the neighbor number. The delimiter is ",", and ";" is output at the end of each line.
	The line number minus 1 is the focal agent number.
	If you can not build it please create a new empty project and copy and paste this souce code.
	Then copy and paste header file and Netowork file in the same way.
*/

#include <iostream>
#include <vector>
#include <fstream>
#include "header/random.h"
#include "header/String.h"
#include "header/create_BASF.h"
#include "header/create_HCSF.h"
#include "header/create_Lattice.h"
#include "header/create_SW.h"
#include "header/create_RRG.h"
#include "header/network_evaluation.h"

using namespace std;

#define BASF 0		// BA-SF
#define HCSF 1		// Highly clustered SF
#define Lattice 2	// Lattice
#define SW 3		// Small World
#define RRG 4		// Regular random graph

struct Evaluation {
	double cluster_coefficient;
	double average_path_length;
};

int main();
void _create_network(int N, int k, int network, int number_of_generating, double p_SW, double p_HCSF, string F_cd);
void _all_create_network(int N, int k, int number_of_generating, double p_SW, double p_HCSF, string F_cd);
void Write_network(std::vector<std::vector<int>>* neighbor, string F);
void Write_network_information(std::vector<Evaluation>* evaluation, string F);

int main() {
	// If you want to generate all networks, please set the following value the "true". 
	// If it is false, only one kind is generated. 
	// Please specify the type of network to generate later.
	bool all_create = false;

	int N = 10000;				// number of people
	int k = 8;				// value of average degree  ***you should write value that is a multiple of two***
	int network = BASF;			// network type
	int number_of_generating = 1; 		// number of networks to generate
	// Since Lattice and RRG each have the same output every time, they generate only once.
	double p_SW = 0.05;			// probability of creating shortcut in the creating SW network
	double p_HCSF = 0.05;			// probability of interrupting and creating default BASF in the creating HCSF network
	string F_cd = "../Network/";		// destination directory

	if (all_create == false) _create_network(N, k, network, number_of_generating, p_SW, p_HCSF, F_cd);
	else _all_create_network(N, k, number_of_generating, p_SW, p_HCSF, F_cd);
	// Network data file is output with such "BA-SF8_N10000.dat" name.
	// Network evaluation data file is output with such "network_evaluation_BA-SF8_N10000.dat" name.
	return 0;
}

void _create_network(int N, int k, int network, int number_of_generating, double p_SW, double p_HCSF, string F_cd) {
	std::vector<std::vector<int>> neighbor;
	std::vector<Evaluation> evaluation;
	bool fg = true;
	if (number_of_generating == 1) fg = false;
	string F_network;
	string F_ncd;
	for (int i = 0; i < number_of_generating; i++) {
		switch (network) {
		case BASF:
			creating_BASF(N, k, &neighbor);
			F_ncd = "BASF/";
			F_network = "BA-SF" + to_string(k);
			break;
		case HCSF:
			creating_HCSF(N, k, &neighbor, p_HCSF);
			F_ncd = "HCSF/";
			F_network = "HC-SF" + to_string(k) + "-" + double_to_string(p_HCSF, 2);
			break;
		case Lattice:
			creating_Lattice(N, k, &neighbor);
			F_ncd = "Lattice/";
			F_network = "Lattice" + to_string(k);
			fg = false;
			break;
		case SW:
			creating_SW(N, k, &neighbor, p_SW);
			F_ncd = "SW/";
			F_network = "SW" + to_string(k) + "-" + double_to_string(p_SW, 2);
			break;
		case RRG:
			creating_RRG(N, k, &neighbor);
			F_ncd = "RRG/";
			F_network = "RRG" + to_string(k);
			fg = false;
			break;
		default:
			cout << "ERROR TYPE" << endl;
			getchar();
			exit(EXIT_SUCCESS);
			break;
		}
		Evaluation eva;
		eva.cluster_coefficient = Cluster_Coefficient(&neighbor);
		eva.average_path_length = Average_Path_Length(&neighbor);
		evaluation.push_back(eva);
		string filename = F_cd + F_ncd + F_network + "_N" + to_string(N);
		if (fg == true) filename += "_" + to_string(i + 1);
		filename += ".dat";
		Write_network(&neighbor, filename);
		cout << F_network << " " << i + 1 << endl;
		if (fg == false) break;
	}
	string filename = F_cd + F_ncd + "network_evaluation_" + F_network + "_N" + to_string(N) + ".dat";
	Write_network_information(&evaluation, filename);
}

void _all_create_network(int N, int k, int number_of_generating, double p_SW, double p_HCSF, string F_cd) {
	for (int network = 0; network < 5; network++) {
		string F_network;
		string F_ncd;
		std::vector<std::vector<int>> neighbor;
		std::vector<Evaluation> evaluation;
		bool fg = true;
		if (number_of_generating == 1) fg = false;
		for (int i = 0; i < number_of_generating; i++) {
			switch (network) {
			case BASF:
				creating_BASF(N, k, &neighbor);
				F_ncd = "BASF/";
				F_network = "BA-SF" + to_string(k);
				break;
			case HCSF:
				creating_HCSF(N, k, &neighbor, p_HCSF);
				F_ncd = "HCSF/";
				F_network = "HC-SF" + to_string(k) + "-" + double_to_string(p_HCSF, 2);
				break;
			case Lattice:
				creating_Lattice(N, k, &neighbor);
				F_ncd = "Lattice/";
				F_network = "Lattice" + to_string(k);
				fg = false;
				break;
			case SW:
				creating_SW(N, k, &neighbor, p_SW);
				F_ncd = "SW/";
				F_network = "SW" + to_string(k) + "-" + double_to_string(p_SW, 2);
				break;
			case RRG:
				creating_RRG(N, k, &neighbor);
				F_ncd = "RRG/";
				F_network = "RRG" + to_string(k);
				fg = false;
				break;
			default:
				cout << "ERROR TYPE" << endl;
				getchar();
				exit(EXIT_SUCCESS);
				break;
			}
			Evaluation eva;
			eva.cluster_coefficient = Cluster_Coefficient(&neighbor);
			eva.average_path_length = Average_Path_Length(&neighbor);
			evaluation.push_back(eva);
			string filename = F_cd + F_ncd + F_network + "_N" + to_string(N);
			if (fg == true) filename += "_" + to_string(i + 1);
			filename += ".dat";
			Write_network(&neighbor, filename);
			cout << F_network << " " << i + 1 << endl;
			if (fg == false) break;
		}
		string filename = F_cd + F_ncd + "network_evaluation_" + F_network + "_N" + to_string(N) + ".dat";
		Write_network_information(&evaluation, filename);
	}
}

void Write_network(std::vector<std::vector<int>>* neighbor, string F) {
	const char* ch = F.c_str();
	ofstream ofs(ch);
	for (int i = 0; i < (*neighbor).size(); i++) {
		for (int j = 0; j < (*neighbor)[i].size(); j++) {
			ofs << (*neighbor)[i][j];
			if (j == (*neighbor)[i].size() - 1) ofs << ';';
			else ofs << ',';
		}
		ofs << endl;
	}
	ofs.close();
}

void Write_network_information(std::vector<Evaluation>* evaluation, string F) {
	const char* ch = F.c_str();
	ofstream ofs(ch);
	for (int i = 0; i < (*evaluation).size(); i++) ofs << i + 1 << ',' << (*evaluation)[i].cluster_coefficient << ',' << (*evaluation)[i].average_path_length << endl;
	ofs.close();
}
