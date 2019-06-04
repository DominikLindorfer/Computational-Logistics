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

void initial_solution_A(vector< dock >& docks, vector< store >& stores, dist_mat& dist, vector< truck >& trucks, int& t_load, int& t_fix_load){

	int n_stores = dist.distances.at(0).size();
	int hub_ind = 0;
	int remain_demand = 0;

	for(auto s : stores){
		remain_demand += s.tot_demand;
	}

	//	solution.emplace_back(0);

	while(remain_demand > 0){

		cout << remain_demand << endl;

		for(int i = 0; i < n_stores; i++){
			cout << remain_demand << "  " << stores.at(i).id << endl;
			if(stores.at(i).cur_demand > 0){

				int avail = 0;
				for(int j = 0; j < (int)trucks.size(); j++){

					if(get<1>(trucks[j].jobs.back()) < get<1>(trucks[avail].jobs.back())){
						avail = j;
					}
				}

				int earliest_dock = 0;
				for(int d = 0; d < (int)docks.size(); d++){

					if( docks[d].is_free(get<1>(trucks[avail].jobs.back())) ){

						int start_time = get<1>(trucks[avail].jobs.back());
						int end_time = start_time + (min(stores.at(i).cur_demand, trucks[avail].capacity) * t_load + t_fix_load);

						int end_time_truck = start_time + 2 * (min(stores.at(i).cur_demand, trucks[avail].capacity) * t_load + t_fix_load + dist(0, stores.at(i).id ) );

						trucks[avail].jobs.emplace_back(make_tuple(start_time, end_time_truck, i,min(stores.at(i).cur_demand, trucks[avail].capacity)));

						remain_demand -= min(stores.at(i).cur_demand, trucks[avail].capacity);
						stores.at(i).cur_demand -= min(stores.at(i).cur_demand, trucks[avail].capacity);

						docks[d].jobs.emplace_back(make_tuple(start_time, end_time, avail));

						earliest_dock = -1;
						break;
					}

					if( get<1>(docks[d].jobs.back()) < get<1>(docks[earliest_dock].jobs.back()) ){
						earliest_dock = d;
					}
				}

				if(earliest_dock >= 0){
					int start_time = get<1>(docks[earliest_dock].jobs.back());
					int end_time = start_time + (min(stores.at(i).cur_demand, trucks[avail].capacity) * t_load + t_fix_load);

					int end_time_truck = start_time + 2 * (min(stores.at(i).cur_demand, trucks[avail].capacity) * t_load + t_fix_load + dist(0, stores.at(i).id) );

					trucks[avail].jobs.emplace_back(make_tuple(start_time, end_time_truck, i,min(stores.at(i).cur_demand, trucks[avail].capacity)));

					remain_demand -= min(stores.at(i).cur_demand, trucks[avail].capacity);
					stores.at(i).cur_demand -= min(stores.at(i).cur_demand, trucks[avail].capacity);

					docks[earliest_dock].jobs.emplace_back(make_tuple(start_time, end_time, avail));
				}


			}
		}
	}
	for(auto &t : trucks) {
		t.jobs.erase(t.jobs.begin());
	}
}

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

bool opt_2(vector< dock >& docks, dist_mat& dist, vector< truck >& trucks, int& t_load, int& t_fix_load){

	//-----Version of 2-opt for the Scheduling Problem at the Docks-----

	//-----Select random truck and 2 random jobs of that truck-----
	auto docks_try = docks;
	int truck_id = rand() % trucks.size();
	auto truck = trucks[truck_id];

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
	delete_jobs_docks(docks_try, truck, truck_id, job_id_1, job_id_2);

	//-----make new dock-jobs by inserting the reversed list of jobs on the truck_id into the docks-----
	create_new_jobs(docks_try, truck, dist, truck_id, job_id_1, job_id_2, t_fix_load, t_load);

//	//-----Evaluate the solution and accept everything that does not make the solution worse-----

	auto trucks_temp = trucks;
	trucks_temp[truck_id] = truck;

	int score_old = evaluate_solution(trucks);
	int score_new = evaluate_solution(trucks_temp);

	if(score_new <= score_old) {
		docks = docks_try;
		trucks = trucks_temp;
		//		for(auto& d : docks)
		//			d.jobs.sort([](tuple<int,int,int> a,tuple<int,int,int> b) {return get<0>(a) < get<0>(b);});

		return true;
	}
	else {
//		trucks[truck_id] = truck_temp;
		return false;
	}
};

bool swap_2jobs_difftrucks(vector< dock >& docks, dist_mat& dist, vector< truck >& trucks, int& t_load, int& t_fix_load){

	//-----Swap 2 jobs for the Scheduling Problem at the Docks-----

	//-----Select random truck and 2 random jobs of that truck-----
	auto docks_try = docks;
	int truck_id_1 = rand() % trucks.size();
	int truck_id_2 = rand() % trucks.size();

	while(truck_id_1 == truck_id_2){
		truck_id_2=rand() % trucks.size();
	}

	auto truck_1 = trucks[truck_id_1];
	auto truck_2 = trucks[truck_id_2];

	int job_id_1 = rand() % truck_1.jobs.size();
	int job_id_2 = rand() % truck_2.jobs.size();

	if(truck_1.jobs.size() == 0 || truck_2.jobs.size() == 0){
		return false;
	}

	cout << "truck_id1: " << truck_id_1 << "  truck_id2: " << truck_id_2  << "  job_id1: " << job_id_1 << "  job_id2: " << job_id_2 << endl;

	cout << endl << "Trucks before swap:" << endl;
	for(auto i : {truck_1, truck_2}){
		//list< tuple <int, int, int> > jobs;
		for(auto j : i.jobs){
			cout << "{";
			cout << get<0>(j) << " , " << get<1>(j) << " , " << get<2>(j) << " , " << get<3>(j);
			cout << "},"<< endl;
		}
		cout << endl;
	}

	cout << "Docks before swap: " << endl;
	for(auto i : docks){
		//list< tuple <int, int, int> > jobs;
		for(auto j : i.jobs){
			cout << "{";
			cout << get<0>(j) << " , " << get<1>(j) << " , " << get<2>(j);
			cout << "},"<< endl;
		}
	}



	//-----delete the 2 jobs + succeeding jobs from docks_try-----

	delete_jobs_docks(docks_try, truck_1, truck_id_1, job_id_1, job_id_1);
	delete_jobs_docks(docks_try, truck_2, truck_id_2, job_id_2, job_id_2);

	//-----swap the 2 jobs-----
	auto it_t1 = next(truck_1.jobs.begin(), job_id_1);
	auto it_t2 = next(truck_2.jobs.begin(), job_id_2);

	auto temp_job = *it_t1;
	*it_t1 = *it_t2;
	*it_t2 = temp_job;

	create_new_jobs(docks_try, truck_1, dist, truck_id_1, job_id_1, job_id_1, t_fix_load, t_load);
	create_new_jobs(docks_try, truck_2, dist, truck_id_2, job_id_2, job_id_2, t_fix_load, t_load);


	cout << endl << "Trucks after swap:" << endl;
	for(auto i : {truck_1, truck_2}){
		//list< tuple <int, int, int> > jobs;
		for(auto j : i.jobs){
			cout << "{";
			cout << get<0>(j) << " , " << get<1>(j) << " , " << get<2>(j) << " , " << get<3>(j);
			cout << "},"<< endl;
		}
		cout << endl;
	}

	cout << "Docks after swap: " << endl;
	for(auto i : docks_try){
		//list< tuple <int, int, int> > jobs;
		for(auto j : i.jobs){
			cout << "{";
			cout << get<0>(j) << " , " << get<1>(j) << " , " << get<2>(j);
			cout << "},"<< endl;
		}
	}


//	//-----reverse stores, demands and delete jobs from (job_id_1 -> job_id_2) in docks-----
//
//	auto it_t1 = next(truck_1.jobs.begin(), job_id_1);
//	auto it_t2 = next(truck_2.jobs.begin(), job_id_2);
//
//	for(auto& d : docks_try) {
//		vector<list<tuple<int,int,int>>::iterator> del;
//
//		for(auto j = d.jobs.begin(); j!= d.jobs.end();j++) {
//			if(truck_id_1 == get<2>(*j) && get<0>(*it_t1)==get<0>(*j)) {
//				del.emplace_back(j);
//			}
//			if(truck_id_2 == get<2>(*j) && get<0>(*it_t2)==get<0>(*j)) {
//				del.emplace_back(j);
//			}
//		}
//		for(auto j : del) {
//			//				cout <<d.id << " " << get<0>(*j) << " " << get<1>(*j) << " " << get<2>(*j) << endl;
//			d.jobs.erase(j);
//		}
//	}
//
//	auto temp_job = *it_t1;
//	*it_t1 = *it_t2;
//	*it_t2 = temp_job;
//
//	vector< list< tuple<int, int ,int ,int > >::iterator > it_list;
//	it_list.push_back(it_t1);
//	it_list.push_back(it_t2);
//
//	vector<int> truck_id_list = {truck_id_1, truck_id_2};
//	truck *truck_list[] = {&truck_1, &truck_2};
//
//	//-----Delete all jobs of truck_id after it_start (it_end?) in all docks-----
//
//	for(auto i = 0; i < 2; i++){
//
//		for(auto it= next(it_list[i], 1); it!=truck_list[i]->jobs.end(); it++) {
//			for(auto& d : docks_try) {
//				vector<list<tuple<int,int,int>>::iterator> del;
//
//				for(auto j = d.jobs.begin(); j!= d.jobs.end(); j++) {
//					if(truck_id_list[i] == get<2>(*j) && get<0>(*it)==get<0>(*j)) {
//						del.emplace_back(j);
//						//					d.jobs.erase(j);
//					}
//				}
//				for(auto j : del)
//					d.jobs.erase(j);
//			}
//		}
//	}
//	//-----make new dock-jobs by inserting the list of jobs on the truck_id_list into the docks-----
//	for(auto i = 0; i < 2; i++){
//
//		for(auto &job = it_list[i]; job!=truck_list[i]->jobs.end(); job++) {
//
//			int loading_time = t_fix_load + t_load*get<3>(*job);
//			int start_time;
//			if(job == truck_list[i]->jobs.begin())
//				start_time = 0;
//			else
//				start_time = get<1>(*prev(job,1));
//
//			int is_dock_free = 0;
//			int earliest_dock = -1;
//
//			for(int d_ind = 0; d_ind < (int)docks_try.size(); d_ind++){
//
//				//			assert(docks_try[2].jobs.end() != it_test);
//
//				if( docks_try[d_ind].is_free(start_time, start_time+loading_time) ){
//					//				docks_try[d].jobs.insert(next(it_dock_job,1), make_tuple(time_wind_begin,time_wind_begin+loading_time,truck_id));
//
//					docks_try[d_ind].jobs.push_back(make_tuple(start_time, start_time+loading_time,truck_id_list[i]));
//
//					docks_try[d_ind].jobs.sort([](tuple<int,int,int> a,tuple<int,int,int> b) {return get<0>(a) < get<0>(b);});
//
//					get<0>(*job) = start_time;
//					get<1>(*job) = start_time + 2*loading_time + 2*dist(0, get<2>(*job));
//					is_dock_free = 1;
//					break;
//				}
//			}
//
//			if(!is_dock_free) {
//				for(int d = 0; d < (int)docks_try.size(); d++) {
//					for(auto it_dock_job =docks_try[d].jobs.begin();it_dock_job != prev(docks_try[d].jobs.end(),1);it_dock_job++ ) {
//
//						int time_wind_begin=get<1>(*it_dock_job);
//						int time_wind_end=get<0>(*next(it_dock_job,1));
//
//						if(start_time<=time_wind_begin) {
//							if(loading_time<=time_wind_end-time_wind_begin) {
//								//							docks_try[d].jobs.insert(next(it_dock_job,1), make_tuple(time_wind_begin,time_wind_begin+loading_time,truck_id));
//
//								docks_try[d].jobs.emplace_back(make_tuple(time_wind_begin,time_wind_begin+loading_time,truck_id_list[i]));
//								docks_try[d].jobs.sort([](tuple<int,int,int> a,tuple<int,int,int> b) {return get<0>(a) < get<0>(b);});
//
//								get<0>(*job) = time_wind_begin;
//								get<1>(*job) = time_wind_begin + 2*loading_time + 2*dist(0,get<2>(*job));
//								earliest_dock=d;
//								break;
//							}
//						}
//						else if((time_wind_end - loading_time) >= start_time && time_wind_begin >= start_time){
//							docks_try[d].jobs.emplace_back(make_tuple(start_time, start_time+loading_time, truck_id_list[i]));
//							docks_try[d].jobs.sort([](tuple<int,int,int> a,tuple<int,int,int> b) {return get<0>(a) < get<0>(b);});
//
//							get<0>(*job) = start_time;
//							get<1>(*job) = start_time + 2*loading_time + 2*dist(0,get<2>(*job));
//							earliest_dock=d;
//							break;
//						}
//					}
//					if(earliest_dock>=0)
//						break;
//				}
//			}
//
//			if(!is_dock_free&&earliest_dock<0) {
//				earliest_dock = 0;
//				//			for(auto& d : docks_try)
//				//				d.jobs.sort([](tuple<int,int,int> a,tuple<int,int,int> b) {return get<0>(a) < get<0>(b);});
//
//				for(int d = 0; d < (int)docks_try.size(); d++) {
//					if( get<1>(docks_try[d].jobs.back()) < get<1>(docks_try[earliest_dock].jobs.back()) ){
//						earliest_dock = d;
//					}
//				}
//				int t1 = get<1>(docks_try[earliest_dock].jobs.back());
//				//			docks_try[earliest_dock].jobs.insert(docks_try[earliest_dock].jobs.end(), make_tuple(t1,t1+loading_time,truck_id));
//
//				docks_try[earliest_dock].jobs.emplace_back(make_tuple(t1, t1+loading_time, truck_id_list[i]));
//				//			docks_try[earliest_dock].jobs.sort([](tuple<int,int,int> a,tuple<int,int,int> b) {return get<0>(a) < get<0>(b);});
//
//				get<0>(*job) = t1;
//				get<1>(*job) = t1 + 2*loading_time + 2*dist(0, get<2>(*job));
//			}
//		}
//	}

	//-----Evaluate the solution and accept everything that does not make the solution worse-----
	auto truck_temp_1 = trucks[truck_id_1];
	auto truck_temp_2 = trucks[truck_id_2];
	int score_old = evaluate_solution(trucks);

	trucks[truck_id_1] = truck_1;
	trucks[truck_id_2] = truck_2;
	int score_new = evaluate_solution(trucks);

	if(true||score_new <= score_old) {
		docks = docks_try;
		//		for(auto& d : docks)
		//			d.jobs.sort([](tuple<int,int,int> a,tuple<int,int,int> b) {return get<0>(a) < get<0>(b);});

		return true;
	}
	else {
		trucks[truck_id_1] = truck_temp_1;
		trucks[truck_id_2] = truck_temp_2;
		return false;
	}
};

/*bool return_val = true;

	//-----implement evaluate_sublist-----
	int result_old = 0;
	evaluate_sublist_solution(result_old, start, end, dist);

	int a = rand() % route_size;
	int b = rand() % route_size;

	if(a == 0)	a++;
	if(a == route_size) a--;
	if(b == 0) b++;
	if(b == route_size) b--;

	auto it_start = next(start,min(a,b));
	auto it_end = next(start,max(a,b));
//	cout << a << " " << b << endl;

	for(int i=0; i<(max(a,b)-min(a,b)+1)/2; i++) {

		auto val1 = *it_start;
		auto val2 = *it_end;

 *it_end=val1;
 *it_start=val2;

		it_start++;
		it_end--;
	}

	//-----implement evaluate_sublist-----
	int result = 0;
	evaluate_sublist_solution(result, start, end, dist);

	if(result_old <= result){

		it_start = next(start,min(a,b));
		it_end = next(start,max(a,b));

		for (int i = 0; i < (max(a, b) - min(a, b) + 1) / 2; i++) {

			auto val1 = *it_start;
			auto val2 = *it_end;

 *it_end = val1;
 *it_start = val2;

			it_start++;
			it_end--;
		}

		return_val = false;
	}

//	cout << "Result old: " << result_old << "  result: " << result << endl;

	return return_val;
}
 */


#endif /* METAHEURISTICS_HPP_ */
