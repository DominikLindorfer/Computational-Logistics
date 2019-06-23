/*
 * datastructures.hpp
 *
 *  Created on: May 8, 2019
 *      Author: lindorfer
 */

#ifndef DATASTRUCTURES_HPP_
#define DATASTRUCTURES_HPP_
#include <bits/stdc++.h>
using namespace std;

class dist_mat{
public:
	vector<vector <long> > distances;

	dist_mat(){

	};

	dist_mat(long s){

		distances.resize(s);
		for(long i = 0; i < s; i++){
			distances[i].resize(s);
		}
	}

	inline long& operator()(long i, long j) {
		return distances[i][j];
	};

};

class store{
public:
	long tot_demand;
	long cur_demand;

	store(){
		tot_demand = 0;
		cur_demand = 0;
	};

	store(long demand_tot_, long demand_cur_){
		tot_demand = demand_tot_;
		cur_demand = demand_cur_;
	}
};

class truck{
public:
	long capacity;
	long load;

	truck(){
		capacity = 0;
		load = 0;
	};

	truck(long capacity_, long load_){
		capacity = capacity_;
		load = load_;
	}
};

#endif /* DATASTRUCTURES_HPP_ */
