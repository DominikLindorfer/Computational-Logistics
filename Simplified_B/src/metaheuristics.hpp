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

void initial_solution(list< int >& solution, vector< store >& stores, dist_mat& dist, vector< truck >& truck){

	int n_stores = dist.distances.at(0).size();
	int max_dist = 0;
	int min_dist = 1e9;
	int ind_max = 0;
	int ind_min = 0;
	int remain_demand = 0;

	for(auto s : stores){
		remain_demand += s.tot_demand;
	}

	solution.emplace_back(0);

	while(remain_demand > 0){
		ind_max = 0;
		ind_min = 0;
		truck.at(0).load = truck.at(0).capacity;
		max_dist = 0;
		min_dist = 1e9;

		for(int i = 0; i < n_stores; i++){

			if(stores.at(i).cur_demand > 0){

				if(dist(0, i) > max_dist){
					max_dist = dist(0, i);
					ind_max = i;
				}
			}
		}

		solution.emplace_back(ind_max);

		int unload = min(stores.at(ind_max).cur_demand, truck.at(0).load);

		stores.at(ind_max).cur_demand -= unload;
		truck.at(0).load -= unload;
		remain_demand -= unload;

		while(truck.at(0).load > 0 && remain_demand > 0){

			for(int i = 1; i < n_stores; i++){

				if(stores.at(i).cur_demand > 0){

					if(dist(ind_max, i) < min_dist){
						min_dist = dist(ind_max, i);
						ind_min = i;
					}
				}
			}

			solution.emplace_back(ind_min);
			unload = min(stores.at(ind_min).cur_demand, truck.at(0).load);

			stores.at(ind_min).cur_demand -= unload;
			truck.at(0).load -= unload;
			remain_demand -= unload;

			ind_max = ind_min;
			min_dist = 1e9;
		}

		solution.emplace_back(0);
	}
}



#endif /* METAHEURISTICS_HPP_ */
