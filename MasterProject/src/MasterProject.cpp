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

	long numb_Days = 0;
	read_csv_distance(TravTime, data,distances);
	read_csv_demand_multipleDays(Demand, demands, numb_Days);
	read_csv_servicetimes(ServiceTime, service_times);

	//	vector< tuple<string,string,string>> distances(data.size());
	long t_load = 1 * 60;
	long t_fix_load = 10 * 60;
	long t_unload = 1 * 60;
	long t_fix_unload = 10 * 60;

	//-----initialize distance matrix-----
	long n_stores = sqrt(distances.size());
	dist_mat dist(n_stores);

	for(auto d : distances){

		long i = stoi(get<0>(d)) - 1;
		long j = stoi(get<1>(d)) - 1;

		dist(i, j) = stoi(get<2>(d));
	}

	//-----initialize stores-----
	vector< store > stores(n_stores);

	cout << n_stores << endl;


	//-----push back the HUBs-----
	vector<long> hub_demand(numb_Days, 0);

	stores.at(0).tot_demand = hub_demand;
	stores.at(0).cur_demand = hub_demand;

	//-----set demands for stores-----
	for(auto& s : stores){
		s.cur_demand.resize(numb_Days,0);
		s.tot_demand.resize(numb_Days,0);
	}

	long day = 0;
	for(auto demand : demands){
		for(auto s : demand){

			long id = stoi(get<0>(s));
			long d = stoi( get<1>(get<3>(s)) );

			stores.at(id - 1).tot_demand[day] += d;
			stores.at(id - 1).cur_demand[day] += d;
			stores.at(id - 1).id = id - 1;
		}
		day++;
	}

	//-----Set Service Times-----
	for(auto s : service_times){

		long id = stoi(get<0>(s)) - 1;
		string start = get<2>(s);
		string end = get<3>(s);

		istringstream stream_start(start);
		istringstream stream_end(end);

		string token_s;
		string token_e;

		long start_time = 0;
		long end_time = 0;

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

	//-----Print out Total Demand and Service Times-----
	for(auto s : stores){
		for(auto i : s.tot_demand)
			cout << i << " ";
		cout << endl;
	}

	for(auto s: stores){
		cout << s.id << " " << s.service_time.first << " " <<s.service_time.second << endl;
	}

	//-----initialize trucks-----
	long n_trucks = 3;
	vector< truck > trucks(n_trucks);

	for(auto& i : trucks){
		i.capacity = 30;
	}

	//-----initialize docks-----
	long n_docks = 1;
	vector< dock > docks(n_docks);

	//-----build nearest neighbor matrix-----
	vector<vector< tuple <long, long>>> nn_mat;
	build_nn_mat(dist, nn_mat, stores);

//	for(long i = 0; i < (long)nn_mat.size(); i++){
//		cout << "{";
//		for(long j = 0; j < (long)nn_mat[i].size();j++){
//			cout << "(" << get<0>(nn_mat[i][j]) << "," << get<1>(nn_mat[i][j]) << ")" << ",";
//		}
//		cout << "}" << endl << endl;
//	}

	//-----build initial solution-----

	//-----solution structure: Days (vector) - Routes on that day (vector) - Routes (lists) - Store# and Demands as vector (longs): [0] == store, [1] == demands-----
	vector<vector< list< vector<long> > > > solution;
	initial_solution_routes(solution, stores, dist, trucks, nn_mat, t_unload, t_fix_unload);

	for(long day = 0; day < (long)solution.size(); day++){

		for(long route_ind = 0; route_ind < (long)solution[day].size(); route_ind++){

			for(auto i : solution[day][route_ind]){
				cout << i[0] << " " << i[1] << " - ";
			}
			cout << endl;
		}
		cout << endl << endl;
	}


	//-----Optimize Initial Solution for Routes-----

	//------ Days -- Routes -- Scores -----
	vector< vector < vector< long > > > route_scores;
	route_scores.resize(solution.size());

	for(long day = 0; day < (long)solution.size(); day++){

		route_scores[day].resize(solution[day].size());

		for(long route = 0; route < (long)solution[day].size(); route++){

			long latest_time = 0;
			long earliest_time = 0;
			long route_time = 0;
			long wait_time = 0;

			long feasible = 0;
			feasible = latest_returntime(latest_time, route_time, wait_time, solution[day][route], t_unload, t_fix_unload, stores, dist);

			if(feasible == 0){
				cout << "Initial Solution of Routes is unfeasible!" << endl;
			}

			earliest_returntime(earliest_time, route_time, wait_time, solution[day][route], t_unload, t_fix_unload, stores, dist);

			route_scores[day][route].push_back(earliest_time);
			route_scores[day][route].push_back(latest_time);
			route_scores[day][route].push_back(route_time);
			route_scores[day][route].push_back(wait_time);
		}
	}

	//-----Print Scores of Initial Solution-----
//	for(long day = 0; day < (long)solution.size(); day++){
//			for(long route = 0; route < (long)solution[day].size(); route++){
//				cout << route_scores[day][route].at(0) << " " << route_scores[day][route].at(1) << " " << route_scores[day][route].at(2) << " " << route_scores[day][route].at(3) << endl;
//			}
//			cout << endl << endl;
//	}
//
//	latest_returntime(latest_time, route_time, wait_time, solution[0][0], t_unload, t_fix_unload, stores, dist);
//
//	cout << "Latest Return-Time: " << latest_time << "  " << route_time << "  " << wait_time << endl;
//	cout << "Latest Return-Time: " << latest_time << "  " << stores[10].service_time.first << "  " << stores[10].service_time.second << "  " << dist(0,10) << endl;
//
//	long earliest_time = 0;
//	route_time = 0;
//	wait_time = 0;
//	earliest_returntime(earliest_time, route_time, wait_time, solution[0][0], t_unload, t_fix_unload, stores, dist);
//
//	cout << "Earliest Return-Time: " << earliest_time << "  " << route_time << "  " << wait_time << endl;
//
//	//-----Swap 2 Stores in diff Routes-----
//
//	long sw_1 = 0;
//	long sw_2 = 0;
//	auto& sol_1 = solution[0][0];
//	auto& sol_2 = solution[0][1];
//	auto& sol_3 = solution[0][7];
//
//	long st = 1;
//	long st2 = 1;
//
//
//
////	//-----Test-----
////	(*next(solution[0][9].begin(), 0))[1] = 25;
////	stores[2].tot_demand[0] += 24;
//
////	routes_swap_2(sol_1, sol_2, stores, dist, trucks, sw_1, sw_2);
////	routes_opt_2(sol_3, st, st2);
//
//	routes_move(solution[0][1], solution[0][9], st, st2);
//	cout << "Routes move!" << endl;
//
//	long cur_day = 0;
//
//	checkSolution(solution[0], cur_day, stores,dist,trucks,total_demand,true);
//	bool accept_swap = false;
//
//	for(long route = 0; route < (long)solution[cur_day].size(); route++){
//
//		long latest_time = 0;
//		long earliest_time = 0;
//		long route_time = 0;
//		long wait_time = 0;
//
//		long tmp = 0;
//		tmp = latest_returntime(latest_time, route_time, wait_time, solution[cur_day][route], t_unload, t_fix_unload, stores, dist);
//		if(tmp == 0){
//
//			cout << "Route " << route << " is not feasible!" << endl;
//
//			accept_swap = false;
//			break;
//		}
//		cout << tmp << endl;
//		accept_swap = true;
//		earliest_returntime(earliest_time, route_time, wait_time, solution[cur_day][route], t_unload, t_fix_unload, stores, dist);
//
//		route_scores[cur_day][route] = {earliest_time, latest_time, route_time, wait_time};
////		route_scores[day][route].push_back(latest_time);
////		route_scores[day][route].push_back(route_time);
////		route_scores[day][route].push_back(wait_time);
//	}
//
//	long score_best = evaluate_route_scores(route_scores_best[cur_day]);
//	long score = evaluate_route_scores(route_scores[cur_day]);
//
//	cout << "Scores: " << score << "  " << score_best << endl;
//
//	if(accept_swap == true && score < score_best){
//
//		cout << "New best found!" << endl;
//		score_best = score;
//		route_scores_best[cur_day] = route_scores[cur_day];
//	}
//
//	long feasible = latest_returntime(latest_time, route_time, wait_time, solution[0][0], t_unload, t_fix_unload, stores, dist);
//	cout << "Feasible?: " << feasible << endl;


	//-----VND/S for Route Optimization-----
	long latest_time = 0;
	long route_time = 0;
	long wait_time = 0;

	auto route_scores_best = route_scores;

	auto solution_best = solution;
	long total_demand = 0;

	long level = 0;
	long moves = 0;
	long accept = 0;
	long n_tries = 1000;
	long tries = 0;
	long not_updated = 0;
	long not_updated_max = 50;

	double accept_ratio = 0.3;

	cout << "Doing VND/S for routes now!" << endl;
	//-----Potential parallelization at this loop b/c routes of different days are independently created-----
	for(long cur_day = 0; cur_day < (long)solution.size(); cur_day++){
		long score_best = evaluate_route_scores(route_scores_best[cur_day]);
		level = 0;

		while(level < 3){
			accept = 0;

			for(long i = 0; i < n_tries; i++){

//				cout << i << " " << cur_day << " " << level << endl;

				long route_1 = rand() % solution[cur_day].size();
				long route_2 = rand() % solution[cur_day].size();

				long pos_1 = rand() % solution[cur_day][route_1].size();
				long pos_2 = rand() % solution[cur_day][route_2].size();

				switch(level){
				case 0 :
					//-----swap 2 jobs-----
					routes_swap_2(solution[cur_day][route_1], solution[cur_day][route_2], pos_1, pos_2);
					break;
				case 1 :
					//-----move jobs between trucks/routes-----
					routes_move(solution[cur_day][route_1], solution[cur_day][route_2], pos_1, pos_2);
					break;
				case 2 :
					//-----2opt-----
					pos_2 = rand() % solution[cur_day][route_1].size();
					routes_opt_2(solution[cur_day][route_1], pos_1, pos_2);
					break;
				default:
					level = 0;
				}

//				if(level != 2 && ((double)accept / (double)n_tries) < accept_ratio){
//					level++;
//				}
//				else if(not_updated > not_updated_max){
//					level++;
//				}
//				else{
//					level = 0;
//				}

				//-----Check for new best Solution-----
				checkSolution(solution[cur_day], cur_day, stores, dist, trucks, total_demand, true);
				bool valid = validateSolution(solution[cur_day], cur_day, t_unload, t_fix_unload, stores, dist, route_scores);

				if(valid){
					long score = evaluate_route_scores(route_scores[cur_day]);
					//			cout << "Scores: " << score << "  " << score_best << endl;

					if(score < score_best){

						//				cout << "New best found!" << endl;
						score_best = score;
						route_scores_best[cur_day] = route_scores[cur_day];
						solution_best[cur_day] = solution[cur_day];
						accept = 1;
					}
					else{
						solution[cur_day] = solution_best[cur_day];
					}
				}
				else{
					solution[cur_day] = solution_best[cur_day];
				}
				//		cout << "Current Level: " << level << "  amount of Tries: " << tries << "  AcceptanceRatio: " << ((double)accept / (double)n_tries) << "  moves: " << moves << endl;
			}

			if(accept == 1){
				level = 0;
			}
			else{
				level++;
			}
		}
	}

	//-----Print out the Route-Solution-----
	for(long day = 0; day < (long)solution.size(); day++){

		for(long route_ind = 0; route_ind < (long)solution[day].size(); route_ind++){

			for(auto i : solution_best[day][route_ind]){
				cout << i[0] << " " << i[1] << " - ";
			}
			cout << endl;
		}
		cout << endl << endl;
	}


	//-----Dock Scheduling with Optimized Routes-----

	//-----create dock jobs from the best route-----
	//-----day, route-jobs -----
	vector< vector <job> > jobs;
	jobs.resize(route_scores_best.size());

	for(unsigned long d = 0; d < route_scores_best.size(); d++){
		for(unsigned long r = 0; r < route_scores_best[d].size(); r++){

			long load = 0;
			for(auto l : solution[d][r]){
				load += l[1];
			}

			jobs[d].push_back(job(route_scores_best[d][r][0], route_scores_best[d][r][1], route_scores_best[d][r][2], route_scores_best[d][r][3], -1, -1, r, load));
			jobs[d].back().calc_loading_t(t_load, t_fix_load);
		}
	}
	cout << "Dock-Jobs Created!" << endl;

	long feasibility_tries = 1000;
	auto trucks_best = trucks;
	auto docks_best = docks;
	accept = 0;

	for(long cur_day = 0; cur_day < (long)solution.size(); cur_day++){	//

		long feasible_solution = -1;

		for(long is_feasible = 0; is_feasible < feasibility_tries; is_feasible++){

//			cout << "sort-routine: " << is_feasible << endl;

			feasible_solution = initial_solution_docks(docks, jobs, trucks, cur_day, is_feasible);
			if(feasible_solution == 0){
				break;
			}

			for(auto& d : docks){
				d.jobs[cur_day].resize(0);
			}

			for(auto& t: trucks){
				t.jobs[cur_day].resize(1);
			}

		}

		if(feasible_solution < 0){
			cout << "No Feasible Solution found!" << endl;
		}

		//-----sort jobs by id again, after the initial solution-----
		sort(jobs[cur_day].begin(), jobs[cur_day].end(), [](job a, job b){ return (a.job_id < b.job_id); });

		long cur_dock = 0;
		for(auto i : docks){
			//list< tuple <long, long, long> > jobs;
			cout << "Dock_id: " << cur_dock << endl;
			cur_dock++;
			for(auto j : i.jobs[cur_day]){
				cout << "{";
				cout << get<0>(j) << " , " << get<1>(j) << " , " << get<2>(j);
				cout << "},"<< endl;
			}
		}

		cout << endl << "Trucks:" << endl;
		for(auto i : trucks){
			//list< tuple <long, long, long> > jobs;
			for(auto j : i.jobs[cur_day]){
				cout << "{";
				cout << get<0>(j) << " , " << get<1>(j) << " , " << get<2>(j) << " , " << get<3>(j);
				cout << "},"<< endl;
			}
			cout << endl;
		}

		for(long t = 0; t < (long)trucks.size(); t++){
			trucks_best[t].jobs.resize(cur_day + 1);
			trucks_best[t].jobs[cur_day] = trucks[t].jobs[cur_day];
		}

		for(long d = 0; d < (long)docks.size(); d++){
			docks_best[d].jobs.push_back(docks[d].jobs[cur_day]);
		}

		//-----Evaluate the score of the initial solution-----
		long score_best = evaluate_truck_workingtime(trucks_best, jobs, cur_day);
		level = 0;

		cout << "Doing VND/S for Docks now!" << endl;

		while(level < 3){
			bool valid = 0;
			accept = 0;

			for(long i = 0; i < n_tries; i++){

//				cout << i << " " << cur_day << " " << level << endl;

				long truck_id_1 = rand() % trucks.size();
				long truck_id_2 = rand() % trucks.size();
				long job_id_1 = rand() % trucks[truck_id_1].jobs[cur_day].size();
				long job_id_2 = rand() % trucks[truck_id_2].jobs[cur_day].size();

				switch(level){
				case 0 :
					//-----swap 2 jobs on docks-----
					valid = docks_swap_2jobs_trucks(docks, jobs, trucks, cur_day, truck_id_1, truck_id_2, job_id_1, job_id_2);
					break;
				case 1 :
					//-----move jobs between trucks/docks-----
					valid = docks_move_job(docks, jobs, trucks, cur_day, truck_id_1, truck_id_2, job_id_1, job_id_2);
					break;
				case 2 :
					//-----2opt on trucks-----
					truck_id_2 = truck_id_1;
					job_id_2 = rand() % trucks[truck_id_1].jobs[cur_day].size();
					valid = docks_opt_2(docks, jobs, trucks, cur_day, truck_id_1, job_id_1, job_id_2);
					break;
				default:
					level = 0;
				}

				if(valid){
					long score = evaluate_truck_workingtime(trucks, jobs, cur_day);
					//			cout << "Scores: " << score << "  " << score_best << endl;

					if(score < score_best){

						//				cout << "New best found!" << endl;
						score_best = score;

						for(long t = 0; t < (long)trucks.size(); t++){
							trucks_best[t].jobs[cur_day] = trucks[t].jobs[cur_day];
						}

						for(long d = 0; d < (long)docks.size(); d++){
							docks_best[d].jobs[cur_day] = docks[d].jobs[cur_day];
						}
						accept = 1;
					}
					else{

						//-----Change back job-ids if the new solution does not get accepted-----
						jobs[cur_day][get<2>(*next(trucks[truck_id_1].jobs[cur_day].begin(), job_id_1))].truck_id = truck_id_2;
						if(level == 0){
							jobs[cur_day][get<2>(*next(trucks[truck_id_2].jobs[cur_day].begin(), job_id_2))].truck_id = truck_id_1;
						}

						for(long t = 0; t < (long)trucks.size(); t++){
							trucks[t].jobs[cur_day] = trucks_best[t].jobs[cur_day];
						}

						for(long d = 0; d < (long)docks.size(); d++){
							docks[d].jobs[cur_day] = docks_best[d].jobs[cur_day];
						}
					}
				}
				else{
					for(long t = 0; t < (long)trucks.size(); t++){
						trucks[t].jobs[cur_day] = trucks_best[t].jobs[cur_day];
					}

					for(long d = 0; d < (long)docks.size(); d++){
						docks[d].jobs[cur_day] = docks_best[d].jobs[cur_day];
					}
				}

//				cout << endl << "Trucks:" << endl;
//				for(auto i : trucks_best){
//					//list< tuple <long, long, long> > jobs;
//					for(auto j : i.jobs[cur_day]){
//						cout << "{";
//						cout << get<0>(j) << " , " << get<1>(j) << " , " << get<2>(j) << " , " << get<3>(j);
//						cout << "},"<< endl;
//					}
//					cout << endl;
//				}

				//		cout << "Current Level: " << level << "  amount of Tries: " << tries << "  AcceptanceRatio: " << ((double)accept / (double)n_tries) << "  moves: " << moves << endl;
			}

			if(accept == 1){
				level = 0;
			}
			else{
				level++;
			}

		}

		//-----Print out the Solution-----

		cout << "Current day: " << cur_day << endl;
		cur_dock = 0;
		for(auto i : docks_best){
			//list< tuple <long, long, long> > jobs;
			cout << "Dock_id: " << cur_dock << endl;
			cur_dock++;
			for(auto j : i.jobs[cur_day]){
				cout << "{";
				cout << get<0>(j) << " , " << get<1>(j) << " , " << get<2>(j);
				cout << "},"<< endl;
			}
		}

		cout << endl << "Trucks:" << endl;
		for(auto i : trucks_best){
			//list< tuple <long, long, long> > jobs;
			for(auto j : i.jobs[cur_day]){
				cout << "{";
				cout << get<0>(j) << " , " << get<1>(j) << " , " << get<2>(j) << " , " << get<3>(j);
				cout << "},"<< endl;
			}
			cout << endl;
		}

	}

//	long cur_day = 1;
//	long truck_id_1 = 0;
//	long truck_id_2 = 1;
//	long job_id_1 = 0;
//	long job_id_2 = 0;
//
////	docks_swap_2jobs_trucks(docks, jobs, trucks, cur_day, truck_id_1, truck_id_2, job_id_1, job_id_2);
////
////	//-----swap back job-ids if the new solution does not get accepted-----
////	jobs[day][get<2>(*next(trucks[truck_id_1].jobs[day].begin(), job_id_1))].truck_id = truck_id_1;
////	jobs[day][get<2>(*next(trucks[truck_id_2].jobs[day].begin(), job_id_2))].truck_id = truck_id_2;
////
////	cout << "Swap 2 jobs difftrucks performed!: " << endl;
////
////	docks_opt_2(docks, jobs, trucks, cur_day, truck_id_1, job_id_1, job_id_2);
////	cout << "2opt performed on docks!: " << endl;
//
//	docks_move_job(docks, jobs, trucks, cur_day, truck_id_1, truck_id_2, job_id_1, job_id_2);
//	cout << "move job performed on docks!: " << endl;
//
//	cout << "max: " << evaluate_solution(trucks) << endl;
//	prlong_docks(docks);
//	prlong_docks_mathematicaLabel(docks);
//	prlong_trucks_mathematicaLabel(trucks);
//
//	long numb_j_trucks = 0;
//	long numb_j_docks  = 0;
//

////
////	cout << endl << "Docks:" << endl;
////	for(auto i : docks){
////		cout << i.jobs.size() << endl;
////		//list< tuple <long, long, long> > jobs;
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
//	long n_swaps = 3;

//	for(long i = 0; i < 10000; i++){
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
//		//		prlong_docks(docks);
//	}
//
//	cout << "new max: " << evaluate_solution(trucks) << endl;
//	prlong_docks(docks);
//
//	cout << endl << "Docks:" << endl;
//	for(auto i : docks){
//		//list< tuple <long, long, long> > jobs;
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
//		//list< tuple <long, long, long> > jobs;
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
//	long level = 0;
//	long moves = 0;
//	long accept = 0;
//	long n_tries = 1000;
//	long tries = 0;
//	long not_updated = 0;
//	long not_updated_max = 50;
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
//			for(long i = 0; i < n_tries; i++){
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
//			for(long i = 0; i < n_tries; i++){
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
//			for(long i = 0; i < n_tries; i++){
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
//			for(long i = 0; i < n_tries; i++){
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
//			for(long i = 0; i < n_tries; i++){
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





//	for(long i = 0; i < 1; i++){
//		swap_njobs_difftrucks(docks_new, dist, trucks_new, t_load, t_fix_load, n_swaps);
//
//		docks = docks_new;
//		trucks = trucks_new;
//	}
//
//
//	for(long i = 0; i < 1; i++){
//		move_job(docks_new, dist, trucks_new, t_load, t_fix_load);
//
//		docks = docks_new;
//		trucks = trucks_new;
//	}



//
//	for(long i = 0; i < 50; i++){
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
//	for(long i = 0; i < 500; i++){
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
//	prlong_docks(docks_best);
//	prlong_docks_mathematicaLabel(docks_best);
//	prlong_trucks_mathematicaLabel(trucks_best);
//
//	cout << endl << "Docks_best:" << endl;
//	for(auto i : docks_best){
//		//list< tuple <long, long, long> > jobs;
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
//		//list< tuple <long, long, long> > jobs;
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
	//	long result = 0;
	//	evaluate_solution(result, solution, dist);
	//
	//	cout << "Result: " << result << endl;
	//
	//	//-----split longo sublists-----
	//
	//	long route_size = 0;
	//	vector<long> it_routes_size;
	//	vector< list<long>::iterator > it_routes;
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
	//	for(long i = 0; i < 1000; i++){
	//
	//		long select = rand() % (it_routes_size.size() - 1);
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
	//	list<long> t = {1,2,3,4,5,6,7,8,9};

	//	long a = 2;
	//	long b = 7;

	//	auto it_start = next(t.begin(),min(a,b));
	//	auto it_end = next(t.begin(),max(a,b));

	//	cout << a << " " << b << endl;

	//
	//	for(auto j : t )
	//		cout << j  << " ";
	//	cout << endl;
	//	vector<list<long>::iterator > del;
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


	//	for(long i=0;i<(max(a,b)-min(a,b)+1)/2;i++) {
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
	//	for(long i=0;i<(max(a,b)-min(a,b)+1)/2;i++) {
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
	//	for(long i = 0; i < 3; i++){
	//		test.distances[i].resize(3);
	//	}
	//
	//	for(long i = 0; i < 3; i++){
	//		for(long j = 0; j < 3; j++){
	//			test(i,j) = i*j;
	//		}
	//	}
	//
	//	for(long i = 0; i < 3; i++){
	//		for(long j = 0; j < 3; j++){
	//			cout << test(i,j) << " ";
	//		}
	//		cout << endl;
	//	}


	return 0;
}
