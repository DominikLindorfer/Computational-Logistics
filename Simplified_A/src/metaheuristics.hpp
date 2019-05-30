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
						int end_time = start_time + (min(stores.at(i).cur_demand, trucks[avail].capacity) * 60 + t_load + t_fix_load);

						int end_time_truck = start_time + 2 * (min(stores.at(i).cur_demand, trucks[avail].capacity) * 60 + t_load + t_fix_load + dist(0, stores.at(i).id ) );

						trucks[avail].jobs.emplace_back(make_tuple(start_time, end_time_truck, i));

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
					int end_time = start_time + (min(stores.at(i).cur_demand, trucks[avail].capacity) * 60 + t_load + t_fix_load);

					int end_time_truck = start_time + 2 * (min(stores.at(i).cur_demand, trucks[avail].capacity) * 60 + t_load + t_fix_load + dist(0, stores.at(i).id) );

					trucks[avail].jobs.emplace_back(make_tuple(start_time, end_time_truck, i));

					remain_demand -= min(stores.at(i).cur_demand, trucks[avail].capacity);
					stores.at(i).cur_demand -= min(stores.at(i).cur_demand, trucks[avail].capacity);

					docks[earliest_dock].jobs.emplace_back(make_tuple(start_time, end_time, avail));
				}


			}
		}
	}

}

void evaluate_solution(int& result, list< int >& solution, dist_mat& dist){

	result = 0;

    auto it = solution.begin();
    for(int i = 0; i < (int)solution.size() - 1; i++){

    	int id = *it;
    	result += dist(id , *(++it) );
    }
}

void evaluate_sublist_solution(int& result, list<int>::iterator& start, list<int>::iterator& end, dist_mat& dist){

	result = 0;

    for(auto it = start; it != end; ){

    	int id = *it;
    	result += dist(id , *(++it) );
    }
}

bool opt_2(list<int>::iterator& start, list<int>::iterator& end, int& route_size, dist_mat& dist){

	bool return_val = true;

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


#endif /* METAHEURISTICS_HPP_ */
