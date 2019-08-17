/*
 * metaheuristics.hpp
 *
 *  Created on: May 8, 2019
 *      Author: lindorfer
 */

#ifndef METAHEURISTICS_HPP_
#define METAHEURISTICS_HPP_
#include <bits/stdc++.h>
#include "datastructures.hpp"

void initial_solution_routes(vector< vector< list< vector<long> > > >& solution, vector< store >& stores, dist_mat& dist, vector< truck >& truck, vector<vector< tuple <int, int>>>& nn_mat, int t_unload, int t_fix_unload){

	long n_stores = dist.distances.at(0).size();
	long min_dist = 1e9;
	long ind_max = 0;
	long ind_min = 0;
	vector< long > remain_demand_vec(stores[0].tot_demand.size(), 0);
	int days = 0;

	for(auto s : stores){

		days = 0;
		for(auto d : s.tot_demand){
			remain_demand_vec[days] += d;
			days++;
		}
	}

	solution.resize(days);

	cout << remain_demand_vec[0] << " " << remain_demand_vec[1] << endl;

//	solution.emplace_back({0,0});

	for(int day = 0; day < (int)remain_demand_vec.size(); day++){
		int route_ind = -1;
		while(remain_demand_vec[day] > 0){

			route_ind++;
			solution[day].push_back(list< vector<long> > ());

			ind_max = 0;
			ind_min = 0;
			truck.at(0).load = truck.at(0).capacity;
			min_dist = 1e9;

			//-----select store farthest away------

			for(long i = (int)nn_mat[0].size() - 1; i >= 0; i--){

				if(stores.at(get<0>(nn_mat[0][i]) - 1).cur_demand[day] > 0){
					ind_max = get<0>(nn_mat[0][i]) - 1;
					break;
				}
			}


			long unload = min((int)stores.at(ind_max).cur_demand[day], (int)truck.at(0).load);
			solution[day][route_ind].push_back({ind_max, unload});

			stores.at(ind_max).cur_demand[day] -= unload;
			truck.at(0).load -= unload;
			remain_demand_vec[day] -= unload;

			while(truck.at(0).load > 0 && remain_demand_vec[day] > 0){

				ind_min = 0;

				for(long i = 0; i < (int)nn_mat[ind_max].size() - 1; i++){

					if(stores.at(get<0>(nn_mat[ind_max][i]) - 1).cur_demand[day] > 0){

						long unload_tmp = min((int)stores.at(get<0>(nn_mat[ind_max][i]) - 1).cur_demand[day], (int)truck.at(0).load);
						if(stores.at(ind_max).service_time.first + get<1>(nn_mat[ind_max][i]) + 2 * t_fix_unload + t_unload * unload + t_unload * unload_tmp <=	stores.at(get<0>(nn_mat[ind_max][i]) - 1).service_time.second){
							ind_min = get<0>(nn_mat[ind_max][i]) - 1;
							unload = unload_tmp;
							break;
						}
					}
				}

				if(ind_min == 0){
//					solution[day][solution[day].size()-1].push_back({ind_min, 0});
					break;
				}

				stores.at(ind_min).cur_demand[day] -= unload;
				truck.at(0).load -= unload;

				solution[day][route_ind].push_back({ind_min, unload});

				remain_demand_vec[day] -= unload;
				ind_max = ind_min;
			}
		}
	}
}

void build_nn_mat(dist_mat& dist, vector<vector< tuple <int, int>>>& nn_mat, vector< store >& stores){

	nn_mat.resize((int)dist.distances.size() );

	for(int i = 0; i < (int)dist.distances.size(); i++){
		nn_mat[i].resize(dist.distances.size() );

		for(int j = 0; j < (int)dist.distances.size(); j++){

			if(i == j){
				continue;
			}

			nn_mat[i][j] = make_tuple(j+1, dist(i,j));
		}

		sort(nn_mat[i].begin(), nn_mat[i].end(), [](tuple<int,int> t1, tuple<int,int> t2){return get<1>(t1) < get<1>(t2);});
		nn_mat[i].erase(nn_mat[i].begin());
	}

}

//void initial_solution_A(vector< dock >& docks, vector< store >& stores, dist_mat& dist, vector< truck >& trucks, int& t_load, int& t_fix_load){
//
//	int n_stores = dist.distances.at(0).size();
//	int hub_ind = 0;
//	int remain_demand = 0;
//
//	for(auto s : stores){
//		remain_demand += s.tot_demand;
//	}
//
//	//	solution.emplace_back(0);
//
//	while(remain_demand > 0){
//
//		cout << remain_demand << endl;
//
//		for(int i = 0; i < n_stores; i++){
//			cout << remain_demand << "  " << stores.at(i).id << endl;
//			if(stores.at(i).cur_demand > 0){
//
//				int avail = 0;
//				for(int j = 0; j < (int)trucks.size(); j++){
//
//					if(get<1>(trucks[j].jobs.back()) < get<1>(trucks[avail].jobs.back())){
//						avail = j;
//					}
//				}
//
//				int earliest_dock = 0;
//				for(int d = 0; d < (int)docks.size(); d++){
//
//					if( docks[d].is_free(get<1>(trucks[avail].jobs.back())) ){
//
//						int start_time = get<1>(trucks[avail].jobs.back());
//						int end_time = start_time + (min(stores.at(i).cur_demand, trucks[avail].capacity) * t_load + t_fix_load);
//
//						int end_time_truck = start_time + 2 * (min(stores.at(i).cur_demand, trucks[avail].capacity) * t_load + t_fix_load + dist(0, stores.at(i).id ) );
//
//						trucks[avail].jobs.emplace_back(make_tuple(start_time, end_time_truck, i,min(stores.at(i).cur_demand, trucks[avail].capacity)));
//
//						remain_demand -= min(stores.at(i).cur_demand, trucks[avail].capacity);
//						stores.at(i).cur_demand -= min(stores.at(i).cur_demand, trucks[avail].capacity);
//
//						docks[d].jobs.emplace_back(make_tuple(start_time, end_time, avail));
//
//						earliest_dock = -1;
//
//						cout << "d : " << d << endl;
//						break;
//					}
//
//					if( get<1>(docks[d].jobs.back()) < get<1>(docks[earliest_dock].jobs.back()) ){
//						earliest_dock = d;
//					}
//				}
//
//				if(earliest_dock >= 0){
//					int start_time = get<1>(docks[earliest_dock].jobs.back());
//					int end_time = start_time + (min(stores.at(i).cur_demand, trucks[avail].capacity) * t_load + t_fix_load);
//
//					int end_time_truck = start_time + 2 * (min(stores.at(i).cur_demand, trucks[avail].capacity) * t_load + t_fix_load + dist(0, stores.at(i).id) );
//
//					trucks[avail].jobs.emplace_back(make_tuple(start_time, end_time_truck, i,min(stores.at(i).cur_demand, trucks[avail].capacity)));
//
//					remain_demand -= min(stores.at(i).cur_demand, trucks[avail].capacity);
//					stores.at(i).cur_demand -= min(stores.at(i).cur_demand, trucks[avail].capacity);
//
//					docks[earliest_dock].jobs.emplace_back(make_tuple(start_time, end_time, avail));
//				}
//
//				cout << "earliest dock: " << earliest_dock << endl;
//			}
//		}
//	}
//	for(auto &t : trucks) {
//		t.jobs.erase(t.jobs.begin());
//	}
//}

int evaluate_solution(vector< truck >& trucks){

	int max = 0;

	for(auto t : trucks){

		for(auto j : t.jobs){
			if(get<1>(j) > max){
				max = get<1>(j);
			}
		}
	}

	return max;
}

void evaluate_sublist_solution(int& result, list<int>::iterator& start, list<int>::iterator& end, dist_mat& dist){

	result = 0;

	for(auto it = start; it != end; ){

		int id = *it;
		result += dist(id , *(++it) );
	}
}

void delete_jobs_docks(vector< dock >& docks_try, truck& truck, int& truck_id, int& job_id_1 ,int& job_id_2){

	auto it_start = next(truck.jobs.begin(), min(job_id_1, job_id_2));
	for(auto it = it_start; it != truck.jobs.end(); it++) {

		list<tuple<int,int,int>>::iterator del;
		int found = 0;

		for(auto& d : docks_try) {

			for(auto j = d.jobs.begin(); j!= d.jobs.end(); j++) {
				if(truck_id == get<2>(*j) && get<0>(*it) == get<0>(*j)) {
					del = j;
					found = 1;
					break;
				}
			}
			if(found != 0){
				d.jobs.erase(del);
				break;
			}
		}
	}

}

void create_new_jobs(vector< dock >& docks_try, truck& truck, dist_mat& dist, int& truck_id, int& job_id_1 ,int& job_id_2, int& t_fix_load, int& t_load) {

	auto it_start = next(truck.jobs.begin(), min(job_id_1, job_id_2));

	for(auto &job = it_start; job!=truck.jobs.end(); job++) {

		int loading_time = t_fix_load + t_load*get<3>(*job);
		int start_time;
		if(job == truck.jobs.begin())
			start_time = 0;
		else
			start_time = get<1>(*prev(job,1));

		int is_dock_free = 0;
		int earliest_dock = -1;

		for(int d_ind = 0; d_ind < (int)docks_try.size(); d_ind++){

			if( docks_try[d_ind].is_free(start_time, start_time+loading_time) ){
				//				docks_try[d].jobs.insert(next(it_dock_job,1), make_tuple(time_wind_begin,time_wind_begin+loading_time,truck_id));

				docks_try[d_ind].jobs.push_back(make_tuple(start_time, start_time+loading_time, truck_id));
				docks_try[d_ind].jobs.sort([](tuple<int,int,int> a,tuple<int,int,int> b) {return get<0>(a) < get<0>(b);});

				get<0>(*job) = start_time;
				get<1>(*job) = start_time + 2*loading_time + 2*dist(0, get<2>(*job));
				is_dock_free = 1;
				break;
			}
		}

		if(!is_dock_free) {
			for(int d = 0; d < (int)docks_try.size(); d++) {
				for(auto it_dock_job = docks_try[d].jobs.begin(); it_dock_job != prev(docks_try[d].jobs.end(),1); it_dock_job++ ) {

					int time_wind_begin = get<1>(*it_dock_job);
					int time_wind_end = get<0>(*next(it_dock_job,1));

					if(start_time <= time_wind_begin) {
						if(loading_time <= (time_wind_end-time_wind_begin)) {
							//							docks_try[d].jobs.insert(next(it_dock_job,1), make_tuple(time_wind_begin,time_wind_begin+loading_time,truck_id));

							docks_try[d].jobs.emplace_back(make_tuple(time_wind_begin, time_wind_begin+loading_time, truck_id));
							docks_try[d].jobs.sort([](tuple<int,int,int> a,tuple<int,int,int> b) {return get<0>(a) < get<0>(b);});

							get<0>(*job) = time_wind_begin;
							get<1>(*job) = time_wind_begin + 2*loading_time + 2*dist(0,get<2>(*job));
							earliest_dock=d;
							break;
						}
					}
					//					else if((time_wind_end - loading_time) >= start_time && time_wind_begin <= start_time){
					//						docks_try[d].jobs.emplace_back(make_tuple(start_time, start_time+loading_time, truck_id));
					//						docks_try[d].jobs.sort([](tuple<int,int,int> a,tuple<int,int,int> b) {return get<0>(a) < get<0>(b);});
					//
					//						get<0>(*job) = start_time;
					//						get<1>(*job) = start_time + 2*loading_time + 2*dist(0,get<2>(*job));
					//						earliest_dock=d;
					//						break;
					//					}
				}
				if(earliest_dock>=0)
					break;
			}
		}

		if(!is_dock_free && earliest_dock < 0) {
			earliest_dock = 0;

			for(int d = 0; d < (int)docks_try.size(); d++) {
				if( get<1>(docks_try[d].jobs.back()) < get<1>(docks_try[earliest_dock].jobs.back()) ){
					earliest_dock = d;
				}
			}

			start_time = max(get<1>(docks_try[earliest_dock].jobs.back()), start_time);
			//			docks_try[earliest_dock].jobs.insert(docks_try[earliest_dock].jobs.end(), make_tuple(t1,t1+loading_time,truck_id));

			docks_try[earliest_dock].jobs.emplace_back(make_tuple(start_time, start_time+loading_time, truck_id));
			//			docks_try[earliest_dock].jobs.sort([](tuple<int,int,int> a,tuple<int,int,int> b) {return get<0>(a) < get<0>(b);});

			get<0>(*job) = start_time;
			get<1>(*job) = start_time + 2*loading_time + 2*dist(0, get<2>(*job));
		}
	}
}

bool accept_solution(vector< truck >& trucks, vector< truck >& trucks_try){

	int score_old = evaluate_solution(trucks);
	int score_new = evaluate_solution(trucks_try);

	if(score_new <= score_old){
		return true;
	}
	else{
		return false;
	}
}

bool opt_2(vector< dock >& docks, dist_mat& dist, vector< truck >& trucks, int& t_load, int& t_fix_load){

	//-----Version of 2-opt for the Scheduling Problem at the Docks-----

	//-----Select random truck and 2 random jobs of that truck-----
	int truck_id = rand() % trucks.size();
	auto& truck = trucks[truck_id];

	int job_id_1 = rand() % truck.jobs.size();
	int job_id_2 = rand() % truck.jobs.size();

	if (truck.jobs.size() > 1) {
		if (job_id_1 == job_id_2) {
			if(job_id_1 == truck.jobs.size() -1){
				job_id_1--;
			}
			else{
				job_id_2++;
			}
		}

	} else {
		return false;
	}

	//-----reverse stores, demands and delete jobs from (job_id_1 -> job_id_2) in docks-----
	auto it_start = next(truck.jobs.begin(), min(job_id_1, job_id_2));
	auto it_end = next(truck.jobs.begin(), max(job_id_1, job_id_2));

	for(int i = 0 ; i < (max(job_id_1, job_id_2)- min(job_id_1, job_id_2) + 1)/2; i++) {

		auto temp_job = *it_start;
		*it_start = *it_end;
		*it_end = temp_job;

		it_start++;
		it_end--;
	}

	//-----Delete all jobs of truck_id starting from it_start in all docks-----
	delete_jobs_docks(docks, truck, truck_id, job_id_1, job_id_2);

	//-----make new dock-jobs by inserting the reversed list of jobs on the truck_id into the docks-----
	create_new_jobs(docks, truck, dist, truck_id, job_id_1, job_id_2, t_fix_load, t_load);

//	//-----Evaluate the solution and accept everything that does not make the solution worse-----
	return true;
//	auto trucks_temp = trucks;
//	trucks_temp[truck_id] = truck;
//
//	int score_old = evaluate_solution(trucks);
//	int score_new = evaluate_solution(trucks_temp);
//
//	if(score_new <= score_old) {
//		docks = docks;
//		trucks = trucks_temp;
//		//		for(auto& d : docks)
//		//			d.jobs.sort([](tuple<int,int,int> a,tuple<int,int,int> b) {return get<0>(a) < get<0>(b);});
//
//		return true;
//	}
//	else {
////		trucks[truck_id] = truck_temp;
//		return false;
//	}
};

bool swap_2jobs_difftrucks(vector< dock >& docks, dist_mat& dist, vector< truck >& trucks, int& t_load, int& t_fix_load){

	//-----Swap 2 jobs for the Scheduling Problem at the Docks-----

	//-----Select random truck and 2 random jobs of that truck-----
	int truck_id_1 = rand() % trucks.size();
	int truck_id_2 = rand() % trucks.size();

	while(truck_id_1 == truck_id_2){
		truck_id_2=rand() % trucks.size();
	}

	auto& truck_1 = trucks[truck_id_1];
	auto& truck_2 = trucks[truck_id_2];

	int job_id_1 = rand() % truck_1.jobs.size();
	int job_id_2 = rand() % truck_2.jobs.size();

	if(truck_1.jobs.size() == 0 || truck_2.jobs.size() == 0){
		return false;
	}

	//-----delete the 2 jobs + succeeding jobs from docks_try-----

	delete_jobs_docks(docks, truck_1, truck_id_1, job_id_1, job_id_1);
	delete_jobs_docks(docks, truck_2, truck_id_2, job_id_2, job_id_2);

	//-----swap the 2 jobs-----
	auto it_t1 = next(truck_1.jobs.begin(), job_id_1);
	auto it_t2 = next(truck_2.jobs.begin(), job_id_2);

	auto temp_job = *it_t1;
	*it_t1 = *it_t2;
	*it_t2 = temp_job;

	create_new_jobs(docks, truck_1, dist, truck_id_1, job_id_1, job_id_1, t_fix_load, t_load);
	create_new_jobs(docks, truck_2, dist, truck_id_2, job_id_2, job_id_2, t_fix_load, t_load);


	//-----Evaluate the solution and accept everything that does not make the solution worse-----
	return true;

//	auto truck_temp_1 = trucks[truck_id_1];
//	auto truck_temp_2 = trucks[truck_id_2];
//	int score_old = evaluate_solution(trucks);
//
//	trucks[truck_id_1] = truck_1;
//	trucks[truck_id_2] = truck_2;
//	int score_new = evaluate_solution(trucks);
//
//	if(score_new <= score_old) {
//		docks = docks;
//		//		for(auto& d : docks)
//		//			d.jobs.sort([](tuple<int,int,int> a,tuple<int,int,int> b) {return get<0>(a) < get<0>(b);});
//
//		return true;
//	}
//	else {
//		trucks[truck_id_1] = truck_temp_1;
//		trucks[truck_id_2] = truck_temp_2;
//		return false;
//	}
};

bool move_job(vector< dock >& docks, dist_mat& dist, vector< truck >& trucks, int& t_load, int& t_fix_load){

	//-----Move job to a different Truck-----

	//-----Select random truck and 2 random jobs of that truck-----
	int truck_id_1 = rand() % trucks.size();
	int truck_id_2 = rand() % trucks.size();

	while(truck_id_1 == truck_id_2){
		truck_id_2=rand() % trucks.size();
	}

	auto& truck_1 = trucks[truck_id_1];
	auto& truck_2 = trucks[truck_id_2];

	int job_id_1 = rand() % truck_1.jobs.size();

	if(truck_1.jobs.size() == 0 || truck_2.jobs.size() == 0){
		return false;
	}
	else if(truck_1.jobs.size() < 2){
		return false;
	}

//	cout << "truck_id1: " << truck_id_1 << "  truck_id2: " << truck_id_2 << "  job_id1: " << job_id_1 << endl;

	//-----delete the job + succeeding jobs from docks_try-----
	delete_jobs_docks(docks, truck_1, truck_id_1, job_id_1, job_id_1);

	//-----Move the job from truck_1 to the end of truck_2-----
	auto it_t1 = next(truck_1.jobs.begin(), job_id_1);
	truck_2.jobs.emplace_back(*it_t1);

	//-----Delete Job from truck_1 and create new jobs for all other jobs on truck_1-----
	truck_1.jobs.erase(it_t1);
	create_new_jobs(docks, truck_1, dist, truck_id_1, job_id_1, job_id_1, t_fix_load, t_load);

	//-----Create a new job for the moved job on truck_2-----
	int job_id_2 = truck_2.jobs.size() - 1;
	create_new_jobs(docks, truck_2, dist, truck_id_2, job_id_2, job_id_2, t_fix_load, t_load);


	//-----Evaluate the solution and accept everything that does not make the solution worse-----
	return true;

//	auto truck_temp_1 = trucks[truck_id_1];
//	auto truck_temp_2 = trucks[truck_id_2];
//	int score_old = evaluate_solution(trucks);
//
//	trucks[truck_id_1] = truck_1;
//	trucks[truck_id_2] = truck_2;
//	int score_new = evaluate_solution(trucks);
//
//	if(score_new <= score_old) {
//		docks = docks;
//		//		for(auto& d : docks)
//		//			d.jobs.sort([](tuple<int,int,int> a,tuple<int,int,int> b) {return get<0>(a) < get<0>(b);});
//
//		return true;
//	}
//	else {
//		trucks[truck_id_1] = truck_temp_1;
//		trucks[truck_id_2] = truck_temp_2;
//		return false;
//	}
};

bool swap_njobs_difftrucks(vector< dock >& docks, dist_mat& dist, vector< truck >& trucks, int& t_load, int& t_fix_load, int& n){

	//-----Performance Improvement?: Create vector for random-jobs at different trucks and delete / create in one step-----
	//-----is limited to #of total amount of jobs though!-----

	//-----Swap n jobs for the Scheduling Problem at the Docks-----
	int truck_id_1 = rand() % trucks.size();

	for(int i = 0; i < n; i++){
		//-----Select random truck and 2 random jobs of those trucks-----
		int truck_id_2 = rand() % trucks.size();

		while(truck_id_1 == truck_id_2){
			truck_id_2=rand() % trucks.size();
		}

		auto& truck_1 = trucks[truck_id_1];
		auto& truck_2 = trucks[truck_id_2];

		int job_id_1 = rand() % truck_1.jobs.size();
		int job_id_2 = rand() % truck_2.jobs.size();

		if(truck_1.jobs.size() == 0 || truck_2.jobs.size() == 0){
			return false;
		}

//		cout << "truck_id1: " << truck_id_1 << "  truck_id2: " << truck_id_2 << "  job_id1: " << job_id_1 << "  job_id2: " << job_id_2 << endl;

		//-----delete the 2 jobs + succeeding jobs from docks_try-----

		delete_jobs_docks(docks, truck_1, truck_id_1, job_id_1, job_id_1);
		delete_jobs_docks(docks, truck_2, truck_id_2, job_id_2, job_id_2);

		//-----swap the 2 jobs-----
		auto it_t1 = next(truck_1.jobs.begin(), job_id_1);
		auto it_t2 = next(truck_2.jobs.begin(), job_id_2);

		auto temp_job = *it_t1;
		*it_t1 = *it_t2;
		*it_t2 = temp_job;

		create_new_jobs(docks, truck_1, dist, truck_id_1, job_id_1, job_id_1, t_fix_load, t_load);
		create_new_jobs(docks, truck_2, dist, truck_id_2, job_id_2, job_id_2, t_fix_load, t_load);
	}

	//-----Evaluate the solution and accept everything that does not make the solution worse-----
	return true;
};

bool swap_2job_sametruck(vector< dock >& docks, dist_mat& dist, vector< truck >& trucks, int& t_load, int& t_fix_load){

	//-----Swap_2 on the same Truck for the Scheduling Problem at the Docks-----

	//-----Select random truck and 2 random jobs of that truck-----
	int truck_id = rand() % trucks.size();
	auto& truck = trucks[truck_id];

	int job_id_1 = rand() % truck.jobs.size();
	int job_id_2 = rand() % truck.jobs.size();

	if (truck.jobs.size() > 1) {
		if (job_id_1 == job_id_2) {
			if(job_id_1 == truck.jobs.size() -1){
				job_id_1--;
			}
			else{
				job_id_2++;
			}
		}

	} else {
		return false;
	}

	//-----reverse stores, demands and delete jobs from (job_id_1 -> job_id_2) in docks-----
	auto it_start = next(truck.jobs.begin(), min(job_id_1, job_id_2));
	auto it_end = next(truck.jobs.begin(), max(job_id_1, job_id_2));

	auto temp_job = *it_start;
	*it_start = *it_end;
	*it_end = temp_job;

	//-----Delete all jobs of truck_id starting from it_start in all docks-----
	delete_jobs_docks(docks, truck, truck_id, job_id_1, job_id_2);

	//-----make new dock-jobs by inserting the reversed list of jobs on the truck_id into the docks-----
	create_new_jobs(docks, truck, dist, truck_id, job_id_1, job_id_2, t_fix_load, t_load);

//	//-----Evaluate the solution and accept everything that does not make the solution worse-----
	return true;
};

bool latest_returntime(int& time, int& route_time, int& wait_time, list< vector<long> >& subsolution, int& t_unload, int& t_fix_unload, vector< store >& stores, dist_mat& dist){

	//-----Calculate Latest Starting-Time-----

	if((int)subsolution.size() == 0){
		time = 0;
		return true;
	}

	int latest_time = stores[(*next(subsolution.begin(), subsolution.size()-1)).at(0)].service_time.second;
	int unload_time = 0;

	for(int i = subsolution.size() - 1; i > 0; i--){

		unload_time = (*next(subsolution.begin(), i)).at(1) * t_unload + t_fix_unload;
		latest_time = min(latest_time, stores[(*next(subsolution.begin(),i)).at(0)].service_time.second);
		latest_time-= unload_time + dist((*next(subsolution.begin(), i-1)).at(0), (*next(subsolution.begin(), i)).at(0));

		latest_time = min(latest_time, stores[(*next(subsolution.begin(), i-1)).at(0)].service_time.second);

		//-----Check if Solution is still Feasible-----
		if(latest_time < stores[(*next(subsolution.begin(), i-1)).at(0)].service_time.first){
			return false;
		}
	}

	unload_time = (*subsolution.begin()).at(1) * t_unload + t_fix_unload;

	if(latest_time - unload_time < stores[(*next(subsolution.begin(), 0)).at(0)].service_time.first){
		return false;
	}
	latest_time -= unload_time + dist(0, (*next(subsolution.begin(), 0)).at(0));
	time = latest_time;


	//-----Calculate Route Time (Hub -> Store -> Unload -> Next Store -> Unload -> ... -> Hub) starting from Latest Time -----
	route_time = 0;
	wait_time = 0;

	route_time = latest_time + dist(0, (*next(subsolution.begin(), 0)).at(0)) + unload_time;
	for(int i = 1; i < (int)subsolution.size(); i++){

		route_time += dist((*next(subsolution.begin(), i-1)).at(0), (*next(subsolution.begin(), i)).at(0));

		if(route_time < stores[(*next(subsolution.begin(),i)).at(0)].service_time.first){
			wait_time += stores[(*next(subsolution.begin(),i)).at(0)].service_time.first - route_time;
		}

		route_time = max(route_time, stores[(*next(subsolution.begin(),i)).at(0)].service_time.first);
		unload_time = (*next(subsolution.begin(), i)).at(1) * t_unload + t_fix_unload;
		route_time += unload_time;
	}

	route_time += dist((*prev(subsolution.end(), 1)).at(0), 0);
	route_time -= latest_time;

	return true;
}

bool earliest_returntime(int& time, int& route_time, int& wait_time, list< vector<long> >& subsolution, int& t_unload, int& t_fix_unload, vector< store >& stores, dist_mat& dist){

	//-----Calculate Earliest Starting-Time-----

	if((int)subsolution.size() == 0){
		time = 0;
		return false;
	}

	int earliest_time = stores[(*subsolution.begin()).at(0)].service_time.first;
	int unload_time = 0;

	unload_time = (*next(subsolution.begin(), 0)).at(1) * t_unload + t_fix_unload;
	earliest_time += unload_time;

	for(int i = 1; i < (int)subsolution.size(); i++){

		unload_time = (*next(subsolution.begin(), i)).at(1) * t_unload + t_fix_unload;
		earliest_time+= dist((*next(subsolution.begin(), i-1)).at(0), (*next(subsolution.begin(), i)).at(0));
		earliest_time = max(earliest_time, stores[(*next(subsolution.begin(),i)).at(0)].service_time.first);

		earliest_time += unload_time;
	}

	earliest_time -= unload_time;

	//-----Calculate Route Time (Hub -> Store -> Unload -> Next Store -> Unload -> ... -> Hub) starting from Earliest Time -----
	route_time = 0;
	wait_time = 0;

	route_time = earliest_time;
	for(int i = (int)subsolution.size() - 1; i > 0; i--){

		route_time -= dist((*next(subsolution.begin(), i-1)).at(0), (*next(subsolution.begin(), i)).at(0));

		if(route_time > stores[(*next(subsolution.begin(),i-1)).at(0)].service_time.second){
			wait_time += route_time - stores[(*next(subsolution.begin(),i-1)).at(0)].service_time.second;
		}

		route_time = min(route_time, stores[(*next(subsolution.begin(),i-1)).at(0)].service_time.second);
		unload_time = (*next(subsolution.begin(), i-1)).at(1) * t_unload + t_fix_unload;
		route_time -= unload_time;
	}

	route_time -= dist((*prev(subsolution.begin(), 0)).at(0), 0);

	int earliest_time_tmp = earliest_time;
	earliest_time = route_time;

	route_time = earliest_time_tmp - route_time + dist((*prev(subsolution.end(), 1)).at(0), 0) + (*prev(subsolution.end(), 1)).at(1) * t_unload + t_fix_unload;

	time = earliest_time;
	return true;
}

bool evaluate_subroute(int& latest_time, int& route_time, int& wait_time, list< vector<long> >& subsolution, int& t_unload, int& t_fix_unload, vector< store >& stores, dist_mat& dist){

	latest_time = 0;
	route_time = 0;
	wait_time = 0;

	if(subsolution.size() == 0){
		return false;
	}
	//-----Returns False if Route is not Feasible-----
	return latest_returntime(latest_time, route_time, wait_time, subsolution, t_unload, t_fix_unload, stores, dist);
}

long updateDemand(list< long >& solution, vector< store >& stores, vector< truck >& trucks, vector<long> & route_remaining_loads,long &total_demand) {

	total_demand=0;
	route_remaining_loads.clear();
	long i=0;
	for(auto& s: stores) {
		s.cur_demand = s.tot_demand;
		total_demand +=s.tot_demand;
		//		cout << i << ":" << s.cur_demand << " ";
		//		i++;
	}
	i=-1;
	//	cout << endl;

	long reduce =0;

	for(auto r : solution) {
		//		cout << r << ":" << stores[r].cur_demand << " ";
		if(r == 0) {
			i++;
			trucks[0].load = trucks[0].capacity;
			route_remaining_loads.push_back(trucks[0].load);
			//			cout << " " <<  route_remaining_loads[i-1] << endl;
		}
		else {
			reduce = min(trucks[0].load,stores[r].cur_demand);
			stores[r].cur_demand -= reduce;
			trucks[0].load -= reduce;
			total_demand -= reduce;
			route_remaining_loads[route_remaining_loads.size()-1]-= reduce;
		}
	}
	route_remaining_loads.pop_back();
	i=0;
	//	for(auto& s: stores) {
	//		cout << i << ":" << s.cur_demand << " ";
	//		i++;
	//	}
	//	cout << endl;

	return total_demand;
}

void splitRoutes(list<long>& solution, vector<list<long> >& routes) {
	for(auto it = solution.begin(); it != prev(solution.end(),1);it++) {
		if(*it==0) {
			routes.resize(routes.size()+1);
		}
		else {
			routes[routes.size()-1].emplace_back(*it);
		}
	}
}

void updateRoutes(list<long>& solution,vector<long>& it_routes_size, vector< list<long>::iterator >& it_routes) {
	long route_size = 0;
	it_routes_size.clear();
	it_routes.clear();
	for(auto it = solution.begin(); it != solution.end(); it++, route_size++){
		if(*it == 0){
			it_routes.push_back(it);
			it_routes_size.push_back(route_size);
			route_size = 0;
		}
	}
	it_routes_size.erase(it_routes_size.begin());
}

void fixSolutionConstrains(list< long >& solution, vector< store >& stores, vector< truck >& trucks, vector<list<long>> &routes, vector<long>& route_remaining_loads, long &total_demand) {


	//		for(auto r: routes) {
	//			for(auto s: r) {
	//				cout << s << " ";
	//			}
	//			cout << endl;
	//		}
	//	for(auto& s: stores) {
	//		cout << i << ":" << s.cur_demand << " ";
	//		i++;
	//	}
	//	cout << endl;

	//----------- try random stuff

	vector<tuple<long,store*>> sort_stores;
	long s_id=0;
	bool random_select = rand()%2;
	for(auto it = stores.begin();it!=stores.end(); it++,s_id++ ){
		sort_stores.push_back(make_tuple(s_id,&(*it)));
	}
	if(random_select)
		random_shuffle(sort_stores.begin(),sort_stores.end());

	for(unsigned long i=0; i<route_remaining_loads.size();i++) {
		if(route_remaining_loads[i]==0)
			continue;
		for(auto& s: sort_stores) {
			if(get<1>(s)->cur_demand > 0 && ((rand()%2) ? route_remaining_loads[i] >0 : get<1>(s)->cur_demand <= route_remaining_loads[i])) { //
				long reduce = min(route_remaining_loads[i],get<1>(s)->cur_demand);
				get<1>(s)->cur_demand-=reduce;
				route_remaining_loads[i] -= reduce;
				total_demand -= reduce;
				routes[i].emplace_back(get<0>(s));
			}
		}
		if(total_demand==0)
			break;
	}
	//	for(auto r: routes) {
	//		for(auto s: r) {
	//			cout << s << " ";
	//		}
	//		cout << endl;
	//	}
	//		for(auto r: routes) {
	//			for(auto s: r) {
	//				cout << s << " ";
	//			}
	//			cout << endl;
	//		}

	for(auto& s: stores) {
		s.cur_demand = s.tot_demand;
		total_demand+= s.tot_demand;
	}

	for(auto& r: routes) {
		trucks[0].load=trucks[0].capacity;
		list<long>::iterator it;
		for(it= r.begin();it!=r.end();it++) {
			if(trucks[0].load==0)
				break;
			long reduce = min(trucks[0].load,stores[*it].cur_demand);
			stores[*it].cur_demand -= reduce;
			trucks[0].load -= reduce;
			//			total_demand -= reduce;
		}
		r.erase(it,r.end());
	}
	//		for(auto r: routes) {
	//			for(auto s: r) {
	//				cout << s << " ";
	//			}
	//			cout << endl;
	//		}
	//	for(auto& s: stores) {
	//		cout << i << ":" << s.cur_demand << " ";
	//		i++;
	//	}
	//	cout << endl;

	//	solution.clear();
	//	for(auto& r : routes) {
	//		solution.emplace_back(0);
	//		for(auto& s: r) {
	//			solution.emplace_back(s);
	//		}
	//	}
	//	solution.emplace_back(0);

	createSolutionFromRoutes(solution,routes);

	updateDemand(solution,stores,trucks,route_remaining_loads,total_demand);
	bool sort_in=false;
	while(total_demand>0) {
		sort_in=true;
		list<long> new_route;
		trucks[0].load = trucks[0].capacity;
		for(auto &s: sort_stores) {
			if(trucks[0].load==0) {
				continue;
			}
			else {
				if(get<1>(s)->cur_demand > 0 && trucks[0].load >0) { // && s.cur_demand
					long reduce = min(trucks[0].load,get<1>(s)->cur_demand );
					get<1>(s)->cur_demand -=reduce;
					trucks[0].load -= reduce;
					total_demand -= reduce;
					new_route.emplace_back(get<0>(s));
				}
			}
			s_id++;
		}
		if(new_route.size()>0) {
			routes.push_back(new_route);
		}
	}
	//		for(auto s: solution) {
	//			cout << s << " ";
	//		}
	//		cout << endl;
	if(sort_in) {
		createSolutionFromRoutes(solution,routes);
		//		solution.clear();
		//		for(auto& r : routes) {
		//			solution.emplace_back(0);
		//			for(auto& s: r) {
		//				solution.emplace_back(s);
		//			}
		//		}
		//		solution.emplace_back(0);
	}
	//		for(auto s: solution) {
	//			cout << s << " ";
	//		}
	//		cout << endl;

	//	trucks[0].load = trucks[0].capacity;
	//	for(long i=0;i<stores.size();i++) {
	//		if(stores[i].cur_demand>0) {
	//			auto it = solution.end();
	//			do {
	//				it--;
	//			}while(*it != 0);
	//			for(;it!=solution.end();i++) {
	//
	//			}
	//
	//
	//		}
	//
	//	}
}

long checkSolution(list< vector<long> >& solution, vector< store >& stores, dist_mat& dist, vector< truck >& trucks, long &total_demand, bool fix, bool prlong=false) {
	vector<long> route_remaining_loads;

	updateDemand(solution,stores,trucks,route_remaining_loads,total_demand);

	if(fix) {
		while(total_demand > 0) {
			vector<list<long> > routes;
			splitRoutes(solution,routes);
			//		for(auto v: routes) {
			//			for(auto r : v) {
			//				cout << r << " ";
			//			}
			//			cout << endl;
			//		}
			//		cout << endl;

			fixSolutionConstrains(solution,stores,trucks,routes,route_remaining_loads,total_demand);
			if(total_demand)
				cout << "total demand >0" << endl;
			if(prlong)
				cout << "fixed" << " " << total_demand <<endl;
		}
		//		cout << "total demand: " << total_demand << " remaining loads: ";
		//		for(auto r: route_remaining_loads) {
		//			cout << r << " ";
		//		}
		//		cout << endl;
	}

	for(auto it=next(solution.begin(),1);it!=solution.end();it++) {
		if(*it==*prev(it,1)) {
			solution.erase(prev(it,1));
		}
	}



	return 0;
}

long routes_swap_2(list< vector<long> >& sol_1, list< vector<long> >& sol_2, vector< store >& stores, dist_mat& dist, vector< truck >& trucks, long& sw_1, long& sw_2){

	//-----implement evaluate_sublist-----
	auto start_1 = sol_1.begin();
	auto start_2 = sol_2.begin();

	auto end_1 =  next(start_1, sol_1.size());
	auto end_2 =  next(start_2, sol_2.size());

//	long a = 1+rand() % (route_size_1-1);
//	long b;
//
//	do {
//		b = 1+rand() % (route_size_2-1);
//	}
//	while(start_1==start_2 && a==b && route_size_1>2 && route_size_2 >2);


	//	if(a == 0)	a++;
	//	if(a == route_size_1) a--;
	//	if(b == 0) b++;
	//	if(b == route_size_2) b--;
	auto swap_1 = next(start_1, sw_1), swap_2 = next(start_2, sw_2);

	//-----Swap if Route 1 & 2 are the same-----
	if(start_1 == start_2) {
		auto temp = *swap_1;
		*swap_1 = *swap_2;
		*swap_2 = temp;
	}
	else {
		//-----Check if Store from Route 1 is already in Route 2 and vice versa-----
		//-----and swap Demands accordingly-----
		if(end_2 == find(start_2,end_2,*swap_1) && end_1 == find(start_1,end_1,*swap_2)) {
			auto temp = *swap_1;
			*swap_1 = *swap_2;
			*swap_2 = temp;
		}
		else if(end_1!=find(start_1,end_1,*swap_2) && end_2==find(start_2,end_2,*swap_1)) {
			int demand_2 = *swap_2[1];
			(*find(start_1,end_1,*swap_2))[1]+=demand_2;
			*swap_2 = *swap_1;

			sol_1.erase(swap_1);
		}
		else if(end_1==find(start_1,end_1,*swap_2) && end_2!=find(start_2,end_2,*swap_1)) {
			int demand_1 = *swap_1[1];
			(*find(start_2,end_2,*swap_1))[1]+=demand_1;
			*swap_1 = *swap_2;

			sol_2.erase(swap_2);
		}
	}

	long total_demand;

	checkSolution(solution,stores,dist,trucks,total_demand,true);

	return 1;
}


#endif /* METAHEURISTICS_HPP_ */
