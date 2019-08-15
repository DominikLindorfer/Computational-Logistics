//============================================================================
// Name        : MasterProject.cpp
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
	string ServiceTime = "Data/StoreServiceTimes_Test.csv";

	vector<vector <string> > data;
	vector< tuple<string,string,string>> distances;
	vector< vector< tuple<string,string,string,tuple<string,string>>> > demands;
	vector< tuple<string,string,string, string> > service_times;

	int numb_Days = 0;
	read_csv_distance(TravTime, data,distances);
	read_csv_demand_multipleDays(Demand, demands, numb_Days);
	read_csv_servicetimes(ServiceTime, service_times);

	//	vector< tuple<string,string,string>> distances(data.size());
	int t_load = 15 * 60;
	int t_fix_load = 10 * 60;
	int t_unload = 15 * 60;
	int t_fix_unload = 10 * 60;

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

	//
	//	for(int i = 0; i < n_stores - demands.size(); i++){
	//		stores.at(i).tot_demand = 0;
	//		stores.at(i).cur_demand = 0;
	//	}

	//-----push back the HUBs-----
	vector<long> hub_demand(numb_Days, 0);

	stores.at(0).tot_demand = hub_demand;
	stores.at(0).cur_demand = hub_demand;

	//-----set demands for stores-----
	for(auto& s : stores){
		s.cur_demand.resize(numb_Days,0);
		s.tot_demand.resize(numb_Days,0);
	}

	int day = 0;
	for(auto demand : demands){
		for(auto s : demand){

			int id = stoi(get<0>(s));
			int d = stoi( get<1>(get<3>(s)) );

			stores.at(id - 1).tot_demand[day] += d;
			stores.at(id - 1).cur_demand[day] += d;
			stores.at(id - 1).id = id - 1;
		}
		day++;
	}

	//-----Set Service Times-----
	for(auto s : service_times){

		int id = stoi(get<0>(s)) - 1;
		string start = get<2>(s);
		string end = get<3>(s);

		istringstream stream_start(start);
		istringstream stream_end(end);

		string token_s;
		string token_e;

		int start_time = 0;
		int end_time = 0;

		vector<string> vec_line_s;
		vector<string> vec_line_e;

		while(getline(stream_start, token_s, ':')) {
			vec_line_s.push_back(token_s);
		}

		while(getline(stream_end, token_e, ':')) {
			vec_line_e.push_back(token_e);
		}

		start_time += stoi(vec_line_s[0]) * 3600;
		start_time += stoi(vec_line_s[1]) * 60;

		end_time += stoi(vec_line_e[0]) * 3600;
		end_time += stoi(vec_line_e[1]) * 60;

		if(end_time == 0){
			end_time = 24 * 3600;
		}

		for(auto& st : stores){
			if(id == st.id){
				st.service_time.first = start_time;
				st.service_time.second = end_time;
				break;
			}
		}
	}

	for(auto s : stores){
		for(auto i : s.tot_demand)
			cout << i << " ";
		cout << endl;
	}

	for(auto s: stores){
		cout << s.id << " " << s.service_time.first << " " <<s.service_time.second << endl;
	}

	//-----initialize trucks-----
	int n_trucks = 3;
	vector< truck > trucks(n_trucks);

	for(auto& i : trucks){
		i.capacity = 30;
	}
	//	truck.at(0).capacity = 100;

	//-----initialize docks-----
	int n_docks = 2;
	vector< dock > docks(n_docks);

//	//-----sort store-demands-----
//	sort(stores.begin(), stores.end(), [](store s1, store s2){
//		return s1.tot_demand > s2.tot_demand;
//	});

	//-----build nearest neighbor matrix-----
	vector<vector< tuple <int, int>>> nn_mat;
	build_nn_mat(dist, nn_mat, stores);

	for(int i = 0; i < (int)nn_mat.size(); i++){
		cout << "{";
		for(int j = 0; j < (int)nn_mat[i].size();j++){
			cout << "(" << get<0>(nn_mat[i][j]) << "," << get<1>(nn_mat[i][j]) << ")" << ",";
		}
		cout << "}" << endl << endl;
	}

	//-----build initial solution-----
	vector<vector< list< vector<long> > > > solution;
	initial_solution_routes(solution, stores, dist, trucks, nn_mat, t_unload, t_fix_unload);

	for(int day = 0; day < solution.size(); day++){

		for(int route_ind = 0; route_ind < solution[day].size(); route_ind++){

			for(auto i : solution[day][route_ind]){
				cout << i[0] << " " << i[1] << " - ";
			}
			cout << endl;
		}
		cout << endl << endl;
	}

//	initial_solution_A(docks, stores, dist, trucks, t_load, t_fix_load);
//
//
//	for(auto i : docks){
//		//list< tuple <int, int, int> > jobs;
//		for(auto j : i.jobs){
//			cout << "{";
//			cout << get<0>(j) << " , " << get<1>(j) << " , " << get<2>(j);
//			cout << "},"<< endl;
//		}
//	}
//
//	cout << "max: " << evaluate_solution(trucks) << endl;
//	print_docks(docks);
//	print_docks_mathematicaLabel(docks);
//	print_trucks_mathematicaLabel(trucks);
//
//	int numb_j_trucks = 0;
//	int numb_j_docks  = 0;
//
////	cout << endl << "Trucks:" << endl;
////	for(auto i : trucks){
////		//list< tuple <int, int, int> > jobs;
////		for(auto j : i.jobs){
////			cout << "{";
////			cout << get<0>(j) << " , " << get<1>(j) << " , " << get<2>(j);
////			cout << "},"<< endl;
////
////			numb_j_trucks++;
////		}
////		cout << endl;
////	}
////
////	cout << endl << "Docks:" << endl;
////	for(auto i : docks){
////		cout << i.jobs.size() << endl;
////		//list< tuple <int, int, int> > jobs;
////		for(auto j : i.jobs){
////			cout << "{";
////			cout << get<0>(j) << " , " << get<1>(j) << " , " << get<2>(j);
////			cout << "},"<< endl;
////
////			numb_j_docks++;
////		}
////		cout << endl;
////	}
//
//	cout << "Number of Jobs Trucks: " << numb_j_trucks << "  Docks: " << numb_j_docks << endl;
//
//
//	auto docks_new = docks;
//	auto trucks_new = trucks;
//
//	auto docks_best = docks;
//	auto trucks_best = trucks;
//
//	int n_swaps = 3;

//	for(int i = 0; i < 10000; i++){
//
//		opt_2(docks_new, dist, trucks_new, t_load, t_fix_load);
//
//		if(accept_solution(trucks, trucks_new)){
//			docks = docks_new;
//			trucks = trucks_new;
//		}
//		else{
//			docks_new = docks;
//			trucks_new = trucks;
//		}
//		//		cout << i << endl;
//		//		print_docks(docks);
//	}
//
//	cout << "new max: " << evaluate_solution(trucks) << endl;
//	print_docks(docks);
//
//	cout << endl << "Docks:" << endl;
//	for(auto i : docks){
//		//list< tuple <int, int, int> > jobs;
//		for(auto j : i.jobs){
//			cout << "{";
//			cout << get<0>(j) << " , " << get<1>(j) << " , " << get<2>(j);
//			cout << "},"<< endl;
//		}
//		cout << endl;
//	}
//
//	cout << endl << "Trucks_best:" << endl;
//	for(auto i : trucks){
//		//list< tuple <int, int, int> > jobs;
//		for(auto j : i.jobs){
//			cout << "{";
//			cout << get<0>(j) << " , " << get<1>(j) << " , " << get<2>(j);
//			cout << "},"<< endl;
//		}
//		cout << endl;
//	}
//
//
//	exit(1);


//	//-----VND-----
//
//	int level = 0;
//	int moves = 0;
//	int accept = 0;
//	int n_tries = 1000;
//	int tries = 0;
//	int not_updated = 0;
//	int not_updated_max = 50;
//
//	double accept_ratio = 0.3;
//
//	while(moves <= 1000){
//		tries++;
//		accept = 0;
//
//		switch(level){
//		case 0 :
//			//swap 2 jobs at same truck
//			for(int i = 0; i < n_tries; i++){
//
//				swap_2job_sametruck(docks_new, dist, trucks_new, t_load, t_fix_load);
//
//				if(accept_solution(trucks, trucks_new)){
//					docks = docks_new;
//					trucks = trucks_new;
//					accept++;
//				}
//				else{
//					docks_new = docks;
//					trucks_new = trucks;
//				}
//			}
//			break;
//		case 1 :
//			//swap 2 jobs different trucks
//			for(int i = 0; i < n_tries; i++){
//
//				swap_2jobs_difftrucks(docks_new, dist, trucks_new, t_load, t_fix_load);
//
//				if(accept_solution(trucks, trucks_new)){
//					docks = docks_new;
//					trucks = trucks_new;
//					accept++;
//				}
//				else{
//					docks_new = docks;
//					trucks_new = trucks;
//				}
//			}
//			break;
//		case 2 :
//			//2opt
//			for(int i = 0; i < n_tries; i++){
//
//				opt_2(docks_new, dist, trucks_new, t_load, t_fix_load);
//
//				if(accept_solution(trucks, trucks_new)){
//					docks = docks_new;
//					trucks = trucks_new;
//					accept++;
//				}
//				else{
//					docks_new = docks;
//					trucks_new = trucks;
//				}
//			}
//			break;
//		case 3 :
//			//swap njobs
//			for(int i = 0; i < n_tries; i++){
//
//				swap_njobs_difftrucks(docks_new, dist, trucks_new, t_load, t_fix_load, n_swaps);
//
//				if(accept_solution(trucks, trucks_new)){
//					docks = docks_new;
//					trucks = trucks_new;
//					accept++;
//				}
//				else{
//					docks_new = docks;
//					trucks_new = trucks;
//				}
//			}
//			break;
//		case 4 :
//			//move job
//
//			docks_new = docks_best;
//			trucks_new = trucks_best;
//
//			for(int i = 0; i < n_tries; i++){
//
//				move_job(docks_new, dist, trucks_new, t_load, t_fix_load);
//			}
//
//			docks = docks_new;
//			trucks = trucks_new;
//
//			moves = moves + 10;
//			break;
//		default:
//			level = 0;
//		}
//
//		if(level != 4 && ((double)accept / (double)n_tries) < accept_ratio){
//			level++;
//		}
//		else if(not_updated > not_updated_max){
//			level++;
//		}
//		else{
//			level = 0;
//		}
//
//		//-----Set new best Solution-----
//
//		if(evaluate_solution(trucks_best) > evaluate_solution(trucks)){
//			docks_best = docks;
//			trucks_best = trucks;
//
//			not_updated = 0;
//		}
//		else{
//			not_updated++;
//		}
////		cout << "Current Level: " << level << "  amount of Tries: " << tries << "  AcceptanceRatio: " << ((double)accept / (double)n_tries) << "  moves: " << moves << endl;
//	}





//	for(int i = 0; i < 1; i++){
//		swap_njobs_difftrucks(docks_new, dist, trucks_new, t_load, t_fix_load, n_swaps);
//
//		docks = docks_new;
//		trucks = trucks_new;
//	}
//
//
//	for(int i = 0; i < 1; i++){
//		move_job(docks_new, dist, trucks_new, t_load, t_fix_load);
//
//		docks = docks_new;
//		trucks = trucks_new;
//	}



//
//	for(int i = 0; i < 50; i++){
//		swap_2jobs_difftrucks(docks_new, dist, trucks_new, t_load, t_fix_load);
//		if(accept_solution(trucks, trucks_new)){
//			docks = docks_new;
//			trucks = trucks_new;
//		}
//		else{
//			docks_new = docks;
//			trucks_new = trucks;
//		}
//	}
//	for(int i = 0; i < 500; i++){
//		opt_2(docks_new, dist, trucks_new, t_load, t_fix_load);
//		if(accept_solution(trucks, trucks_new)){
//			docks = docks_new;
//			trucks = trucks_new;
//		}
//		else{
//			docks_new = docks;
//			trucks_new = trucks;
//		}
//	}
//
//
//	////	swap_2jobs_difftrucks(docks,dist,trucks,t_load,t_fix_load);
//	//
//
//	//
//	//
//	cout << "new max: " << evaluate_solution(trucks_best) << endl;
//	print_docks(docks_best);
//	print_docks_mathematicaLabel(docks_best);
//	print_trucks_mathematicaLabel(trucks_best);
//
//	cout << endl << "Docks_best:" << endl;
//	for(auto i : docks_best){
//		//list< tuple <int, int, int> > jobs;
//		for(auto j : i.jobs){
//			cout << "{";
//			cout << get<0>(j) << " , " << get<1>(j) << " , " << get<2>(j);
//			cout << "},"<< endl;
//		}
//		cout << endl;
//	}
//
//	cout << endl << "Trucks_best:" << endl;
//	for(auto i : trucks_best){
//		//list< tuple <int, int, int> > jobs;
//		for(auto j : i.jobs){
//			cout << "{";
//			cout << get<0>(j) << " , " << get<1>(j) << " , " << get<2>(j);
//			cout << "},"<< endl;
//		}
//		cout << endl;
//	}




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
