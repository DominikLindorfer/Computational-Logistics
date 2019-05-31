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
	vector<vector <string> > data;
	vector< tuple<string,string,string>> distances;
	vector< tuple<string,string,string,tuple<string,string>>> demands;

	read_csv_distance(TravTime, data,distances);
	read_csv_demand(Demand,demands);
	//	vector< tuple<string,string,string>> distances(data.size());
	int t_load = 15 * 60;
	int t_fix_load = 10 * 60;

	//-----initialize distance matrix-----
	int n_stores = sqrt(distances.size());
	dist_mat dist(n_stores);

	for(auto d : distances){

		int i = stoi(get<0>(d)) - 1;
		int j = stoi(get<1>(d)) - 1;

		dist(i, j) = stoi(get<2>(d));
	}

//	for(int i = 0; i < n_stores; i++){
//		for(int j = 0; j < n_stores; j++){
//			cout << dist(i,j) << " ";
//		}
//		cout << endl;
//	}

	//-----initialize stores-----
	vector< store > stores(n_stores);

	cout << n_stores << endl;

//	//-----push back the HUBs-----
//	for(int i = 0; i < n_stores - demands.size(); i++){
//		stores.at(i).tot_demand = 0;
//		stores.at(i).cur_demand = 0;
//	}
	stores.at(0).tot_demand = 0;
	stores.at(0).cur_demand = 0;

	//-----set demands for stores-----
	for(auto s : demands){
		int id = stoi(get<0>(s));
		int d = stoi( get<1>(get<3>(s)) );

		stores.at(id - 1).tot_demand += d;
		stores.at(id - 1).cur_demand += d;
		stores.at(id - 1).id = id - 1;
	}

	for(auto s : stores){
		cout << s.tot_demand << endl;
	}
	cout << "demands finished" << endl;
	//-----initialize trucks-----
	int n_trucks = 10;
	vector< truck > trucks(n_trucks);

	for(auto& i : trucks){
		i.capacity = 20;
	}
//	truck.at(0).capacity = 100;

	//-----initialize docks-----
	int n_docks = 5;
	vector< dock > docks(n_docks);

	//-----sort store-demands-----
	sort(stores.begin(), stores.end(), [](store s1, store s2){
		return s1.tot_demand > s2.tot_demand;
	});

	cout << "demands finished" << endl;

	//-----build initial solution-----
	initial_solution_A(docks, stores, dist, trucks, t_load, t_fix_load);


	for(auto i : docks){
		//list< tuple <int, int, int> > jobs;
		for(auto j : i.jobs){
			cout << "{";
			cout << get<0>(j) << " , " << get<1>(j) << " , " << get<2>(j);
			cout << "},"<< endl;
		}

	}

	cout << "max: " << evaluate_solution(trucks) << endl;

	for(int i = 0; i < 500; i++){
		opt_2(docks,dist,trucks,t_load,t_fix_load);
		cout << i << endl;
	}



	cout << "max: " << evaluate_solution(trucks) << endl;

	for(auto i : docks){
		//list< tuple <int, int, int> > jobs;
		for(auto j : i.jobs){
			cout << "{";
			cout << get<0>(j) << " , " << get<1>(j) << " , " << get<2>(j);
			cout << "},"<< endl;
		}

	}


//	for(auto i : solution){
//		cout << i+1 << ",";
//	}
//	cout << endl;
//
//	int result = 0;
//	evaluate_solution(result, solution, dist);
//
//	cout << "Result: " << result << endl;
//
//	//-----split into sublists-----
//
//	int route_size = 0;
//	vector<int> it_routes_size;
//	vector< list<int>::iterator > it_routes;
//	for(auto it = solution.begin(); it != solution.end(); it++, route_size++){
//		if(*it == 0){
//			it_routes.push_back(it);
//			it_routes_size.push_back(route_size);
//			route_size = 0;
//		}
//	}
//
//	it_routes_size.erase(it_routes_size.begin());
//
////	opt_2(it_routes.at(0), it_routes.at(1), it_routes_size.at(0), dist);
//
//	for(int i = 0; i < 1000; i++){
//
//		int select = rand() % (it_routes_size.size() - 1);
//
//		opt_2(it_routes.at(select), it_routes.at(select + 1), it_routes_size.at(select), dist);
//
//	}
//
//	result = 0;
//	evaluate_solution(result, solution, dist);
//	cout << "Result after 2_opt: " << result << endl;
//
//	for(auto i : solution){
//		cout << i+1 << ",";
//	}
//	cout << endl;

//
//	for(auto i : solution){
//		cout << i+1 << ",";
//	}
//	cout << endl;
//
//
//
//	list<int> t = {1,2,3,4,5,6,7,8,9};

//	int a = 2;
//	int b = 7;

//	auto it_start = next(t.begin(),min(a,b));
//	auto it_end = next(t.begin(),max(a,b));

//	cout << a << " " << b << endl;

//
//	for(auto j : t )
//		cout << j  << " ";
//	cout << endl;
//	vector<list<int>::iterator > del;
//	for(auto j = t.begin();j!=t.end();j++) {
//			if(rand()%2) {
//				cout << *j << " ";
//				del.push_back(j);
//			}
//	}
//	cout << endl;
//	for(auto j : del)
//		t.erase(j);
//
//	for(auto j : t )
//		cout << j  << " ";
//	cout << endl;


//	for(int i=0;i<(max(a,b)-min(a,b)+1)/2;i++) {
//
//		auto val1 = *it_start;
//		auto val2 = *it_end;
//
//		*it_end=val1;
//		*it_start=val2;
//
//		it_start++;
//		it_end--;
//	}
//
//	for(auto s: t ) cout << s << " ";
//	cout << endl;
//	for(int i=0;i<(max(a,b)-min(a,b)+1)/2;i++) {
//
//		auto val1 = *it_start;
//		auto val2 = *it_end;
//
//		*it_end=val1;
//		*it_start=val2;
//
//		it_start++;
//		it_end--;
//	}
//	for(auto s: t ) cout << s << " ";
//	cout << endl;
//	cout << dist(0,0) << endl;
//
//	dist_mat test;
//
//	test.distances.resize(3);
//	for(int i = 0; i < 3; i++){
//		test.distances[i].resize(3);
//	}
//
//	for(int i = 0; i < 3; i++){
//		for(int j = 0; j < 3; j++){
//			test(i,j) = i*j;
//		}
//	}
//
//	for(int i = 0; i < 3; i++){
//		for(int j = 0; j < 3; j++){
//			cout << test(i,j) << " ";
//		}
//		cout << endl;
//	}


	return 0;
}
