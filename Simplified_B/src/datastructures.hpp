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
	vector<vector <int> > distances;

	dist_mat(){

	};

	dist_mat(int s){

		distances.resize(s);
		for(int i = 0; i < s; i++){
			distances[i].resize(s);
		}
	}

	inline int& operator()(int i, int j) {
		return distances[i][j];
	};

};

class store{
public:
	int tot_demand;
	int cur_demand;

	store(){
		tot_demand = 0;
		cur_demand = 0;
	};

	store(int demand_tot_, int demand_cur_){
		tot_demand = demand_tot_;
		cur_demand = demand_cur_;
	}
};

class truck{
public:
	int capacity;
	int load;

	truck(){
		capacity = 0;
		load = 0;
	};

	truck(int capacity_, int load_){
		capacity = capacity_;
		load = load_;
	}
};

#endif /* DATASTRUCTURES_HPP_ */
