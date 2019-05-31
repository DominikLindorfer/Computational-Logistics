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
			distances.at(i).resize(s);
		}
	}

	inline int& operator()(int i, int j) {
		return distances.at(i).at(j);
	};

};

class store{
public:
	int tot_demand;
	int cur_demand;
	int id;

	store(){
		tot_demand = 0;
		cur_demand = 0;
		id = -1;
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
	//----- start, end, store-id -----
	list< tuple <int, int, int,int> > jobs;

	truck(){
		capacity = 0;
		load = 0;
		jobs.emplace_back( make_tuple(0,0,0,0) );  //tuple<int, int, int> (0,0,0)
	};

	truck(int capacity_, int load_){
		capacity = capacity_;
		load = load_;
	}
};

class dock{
public:
	int id;
	//----- start, end, truck-id -----
	list< tuple <int, int, int> > jobs;

	dock(){
		id = -1;
//		jobs.emplace_back ( make_tuple(-1,-1,-1) );
	}

	dock(int id_, list< tuple <int, int, int> > jobs_){
		id = id_;
		jobs = jobs_;
	}

	bool is_free(int time){
		if((int)jobs.size() > 0){
			if(time > get<1>( jobs.back() )){
				return true;
			}
			auto it = jobs.begin();
			for(auto j=0;j<jobs.size();j++,it++) {
				if( (time >= get<0>(*it)) && (time <= get<1>(*it)) ){
					return false;
				}
			}
			return true;
		}
		else
			return true;
	}
};
#endif /* DATASTRUCTURES_HPP_ */
