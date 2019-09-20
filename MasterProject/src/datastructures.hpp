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
			distances.at(i).resize(s);
		}
	}

	inline long& operator()(long i, long j) {
		return distances.at(i).at(j);
	};

};

class store{
public:
	vector<long> tot_demand;
	vector<long> cur_demand;
	long id;

	pair<long, long> service_time;

	store(){
		tot_demand = {0};
		cur_demand = {0};
		id = -1;
	};

};

class truck{
public:
	long capacity;
	long load;
	long id;
	//----- start, end, store-id -----
	list< tuple <long, long, long, long> > jobs;

	truck(){
		capacity = 0;
		load = 0;
		jobs.emplace_back( make_tuple(0,0,0,0) );  //tuple<long, long, long> (0,0,0)
		id = -1;
	};

	truck(const truck& t){
		capacity = t.capacity;
		load = t.load;
		jobs = t.jobs;
		id = t.id;
	}

	truck(long capacity_, long load_){
		capacity = capacity_;
		load = load_;
	}
};

class dock{
public:
	long id;
	//----- start, end, truck-id -----
	list< tuple <long, long, long> > jobs;

	dock():jobs(0){
		id = -1;
//		jobs.emplace_back ( make_tuple(-1,-1,-1) );
	}

	dock(long id_, list< tuple <long, long, long> > jobs_){
		id = id_;
		jobs = jobs_;
	}

	bool is_free(long start_time, long end_time){
		if((long)jobs.size() > 1){

			if(end_time <= get<0>(*jobs.begin())){
				return true;
			}

			for(auto it = jobs.begin(); it != prev(jobs.end(), 1); it++){
//				start_time >= get<1>(*it) && end_time <= get<0>(*next(it, 1

				if(start_time >= get<1>(*(it)) && end_time <= get<0>(*(next(it, 1)))){
					return true;
				}
			}
			return false;
		}
		else if((long)jobs.size() == 1) {
			auto it = jobs.begin();
			if(end_time <= get<0>(*it) || start_time >= get<1>(*it))
				return true;
			else
				return false;
		}
		else
			return true;
	}

	bool is_free(long time){
		if((long)jobs.size() > 0){
//			if(time > get<1>( jobs.back() )){
//				return true;
//			}
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

class job{
public:
	long earliest_time, latest_time, route_time, wait_time, dock_id, truck_id, job_id, load, loading_time, job_dura;

	job(){
		earliest_time = latest_time = route_time = wait_time = dock_id = truck_id = job_id = load = loading_time = job_dura = -1;
	}

	job(long earliest_time_, long latest_time_, long route_time_, long wait_time_, long dock_id_, long truck_id_, long job_id_, long load_){
		earliest_time = earliest_time_;
		latest_time = latest_time_;
		route_time = route_time_;
		wait_time = wait_time_;
		dock_id = dock_id_;
		truck_id = truck_id_;
		job_id = job_id_;
		load = load_;
		loading_time = -1;
		job_dura = -1;
	}

	void calc_loading_t(long t_load, long t_fix_load){
		loading_time = load * t_load + t_fix_load;
		job_dura = loading_time + route_time;
	}
};

#endif /* DATASTRUCTURES_HPP_ */
