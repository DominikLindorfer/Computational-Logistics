//============================================================================
// Name        : Simplified_B.cpp
// Author      : lindorfer
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include <bits/stdc++.h>
#include "io.hpp"
#include "datastructures.hpp"
#include "metaheuristics.hpp"
using namespace std;

int main() {

	string TravTime = "Data/TravelTim1_test.csv";
	string Demand = "Data/Demand1_test.csv";

	//	ofstream scores ("scores.dat");
	//	ofstream sol ("solutions.dat");

	vector<vector <string> > data;
	vector< tuple<string,string,string>> distances;
	vector< tuple<string,string,string,tuple<string,string>>> demands;

	read_csv_distance(TravTime, data,distances);
	read_csv_demand(Demand,demands);
	//	vector< tuple<string,string,string>> distances(data.size());

	//-----initialize distance matrix-----
	int n_stores = sqrt(distances.size());
	dist_mat dist(n_stores);

	for(auto d : distances){

		int i = stoi(get<0>(d)) - 1;
		int j = stoi(get<1>(d)) - 1;

		dist(i, j) = stoi(get<2>(d));
	}

	//-----initialize stores-----
	vector< store > stores(n_stores);
	stores.at(0).tot_demand = 0;
	stores.at(0).cur_demand = 0;



	//-----set demands for stores-----
	for(auto s : demands){
		int id = stoi(get<0>(s));
		int d = stoi( get<1>(get<3>(s)) );

		stores.at(id - 1).tot_demand += d;
		stores.at(id - 1).cur_demand += d;
	}

	for(auto s : stores){
		cout << s.tot_demand << endl;
	}
	//-----initialize trucks-----
	vector< truck > truck(1);
	truck.at(0).capacity = 165;
	truck.at(0).load = truck.at(0).capacity;



	//-----build initial solution-----
	list< int > solution;
	initial_solution(solution, stores, dist, truck);

	for(auto i : solution){
		cout << i+1 << ",";
	}
	cout << endl;

	int result = 0;
	evaluate_solution(result, solution, dist);

	cout << "Result: " << result << endl;

	//-----split into sublists-----
	int n_runs =1000;
	int n_trys = 2500;
	int ratio_=750;
	int total_demand;
	int level =0;
	int not_changed=0;
	int not_changed_best=0;
	int temp_result =0;
	int max_level = 100;
	double tempscale=.01*result;
	double scalingfactor=1;
	int max_notchange =0;


	auto best_solution = solution ;
	int best_result =result;
	auto temp_solution = solution;
	vector<int> it_routes_size;
	vector< list<int>::iterator > it_routes;
	updateRoutes(solution,it_routes_size,it_routes);
	vector<int> test;

	long accept_ =0;

	for(int runs =0 ;runs <= n_runs ; runs++ ) {
		level =0;
		while(level<=max_level) {
			auto max_steps=(10.*n_trys/(level/3+1));
			not_changed=0;
			max_notchange=0;
			accept_=0;
			for(int i = 0; i < max_steps; i++) {
				int select_1=0;
				int select_2=0;
				temp_solution=solution;
				updateRoutes(temp_solution,it_routes_size,it_routes);
				if(it_routes_size.size() - 1)
					select_1 = rand() % (it_routes_size.size());
				if(it_routes_size.size() - 1)
					select_2 = rand() % (it_routes_size.size());
				vector<list<int>> routes;
				switch(level) {
				case 0:
					splitRoutes(temp_solution,routes);
					opt_2(it_routes.at(select_1), it_routes.at(select_1 + 1), it_routes_size.at(select_1), dist);
					checkSolution(temp_solution,stores,dist,truck,total_demand,true);
					break;
				case 1:
					move(it_routes.at(select_1), it_routes.at(select_1), it_routes_size.at(select_1),it_routes_size.at(select_1), temp_solution, stores,dist,truck);
					cout << i << endl;
					break;
				case 2:
					swap_2(it_routes.at(select_1), it_routes.at(select_1), it_routes_size.at(select_1),it_routes_size.at(select_1), temp_solution, stores,dist,truck);
					break;
				case 3:
					movePartsOfRoutes(temp_solution,stores,dist,truck,select_1,select_2,2+rand()%4);
					break;
				case 4:
					swap_2(it_routes.at(select_1), it_routes.at(select_2), it_routes_size.at(select_1),it_routes_size.at(select_2), temp_solution, stores,dist,truck);
					break;
				case 5:
					//					updateRoutes(temp_solution,it_routes_size,it_routes);
					move(it_routes.at(select_1), it_routes.at(select_2), it_routes_size.at(select_1),it_routes_size.at(select_2), temp_solution, stores,dist,truck);
					break;
				case 6:
					swap_n(temp_solution, stores,dist,truck,rand()%temp_solution.size()/4+1);
					break;
				case 7:
					temp_solution= (rand()%4) ? best_solution : solution;
					updateRoutes(solution,it_routes_size,it_routes);

					if(rand()%2)
						destroyAndRepairStations(temp_solution,stores,dist,truck,(rand()%solution.size()/2)+1);
					else if(rand()%2) {
						if(it_routes_size.size()>1)
							destroyAndRepairRoutes(temp_solution,stores,dist,truck,(rand()%it_routes.size()/2)+1);
					}
					else {
						randomRouteOrder(temp_solution,stores,dist,truck);
					}

					cout << "solution after destroy" << endl;
					evaluate_solution(result,temp_solution,dist);
					cout << "solution: " << result << endl;
					for(auto i : temp_solution){
						cout << i+1 << ",";
					}
					cout << endl;
					i=max_steps;
					not_changed=max_steps;
					solution = temp_solution;
					level++;
					break;
				default:
					level=max_level+1;
				}

				evaluate_solution(temp_result,temp_solution,dist);
				//				auto accept=min(exp((result-temp_result)/tempscale),1.) >= (1.*rand())/RAND_MAX;

				int accept = (-result+temp_result < best_result*.001*pow(scalingfactor,3)) ? rand()%3 : false;
				if(accept) {
					//				if(temp_result<=result) {
					solution=temp_solution;
					result = temp_result;
					accept_++;
				}
				if(accept) {
					//				if(temp_result<result) {
					not_changed=0;
				}
				else {
					not_changed++;
				}

				if(result<best_result) {
					best_solution = solution;
					best_result = result;
					//					level =0;
					//					for(auto i : best_solution){
					//						sol << i+1 << ",";
					//					}
					//					sol << endl;
					not_changed_best=0;
				}
				else {
					not_changed_best++;
				}
				max_notchange = max(max_notchange,not_changed);
				//				scores << temp_result << endl;

			}
			cout << "---------------------------------------------" << endl;
			cout << "current level: " << level << " i_runs: " << runs << "/" << n_runs << endl;
			if(max_notchange>=max_steps/ratio_) {
				level++;
			}
			cout <<  "tempscale: " << tempscale << " max_notchange: " << max_notchange  << " " << "not_changed_best: " << not_changed_best  << endl;
			//			updateDemand(temp_solution,stores,truck,test,total_demand);
			//			if(total_demand)
			//				cout << "total_demand:" << total_demand << endl;
			cout << "Best solution: " << best_result << endl;

			cout << "acceptance ration: " << (1.*accept_)/max_steps << " " << scalingfactor <<  endl;

			if((1.*accept_)/max_steps>=0.15)
				scalingfactor*=.99;
			else
				scalingfactor*=1.01;

			if(not_changed_best>1000*1000*10)
				tempscale*=.9;
		}
		tempscale*=.99;

		evaluate_solution(result,solution,dist);
		cout << "solution: " << result << endl;
		for(auto i : solution){
			cout << i+1 << ",";
		}
		cout << endl;
		evaluate_solution(best_result,best_solution,dist);
		cout << "best solution: " << best_result << endl;
		for(auto i : best_solution){
			cout << i+1 << ",";
		}
		cout << endl;

	}


	return 0;
}
