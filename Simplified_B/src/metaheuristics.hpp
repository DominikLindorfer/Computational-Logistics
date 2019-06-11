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


auto removeDuplicates(list<int> solution) {



	return true;
}

void initial_solution(list< int >& solution, vector< store >& stores, dist_mat& dist, vector< truck >& truck){

	int n_stores = dist.distances.at(0).size();
	int max_dist = 0;
	int min_dist = 1e9;
	int ind_max = 0;
	int ind_min = 0;
	int remain_demand = 0;

	for(auto s : stores){
		remain_demand += s.cur_demand;
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

void evaluate_solution(int& result, list< int >& solution, dist_mat& dist){

	result = 0;

	auto it = solution.begin();
	for(int i = 0; i < solution.size() - 1; i++){

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


void createSolutionFromRoutes(list< int >& solution, vector<list<int>> &routes) {
	solution.clear();
	for(auto& r : routes) {
		solution.emplace_back(0);
		for(auto& s: r) {
			solution.emplace_back(s);
		}
	}
	solution.emplace_back(0);
}


int updateDemand(list< int >& solution, vector< store >& stores, vector< truck >& trucks, vector<int> & route_remaining_loads,int &total_demand) {

	total_demand=0;
	route_remaining_loads.clear();
	int i=0;
	for(auto& s: stores) {
		s.cur_demand = s.tot_demand;
		total_demand +=s.tot_demand;
		//		cout << i << ":" << s.cur_demand << " ";
		//		i++;
	}
	i=-1;
	//	cout << endl;

	int reduce =0;

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

void fixSolutionConstrains(list< int >& solution, vector< store >& stores, vector< truck >& trucks, vector<list<int>> &routes, vector<int>& route_remaining_loads, int &total_demand) {


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

	vector<tuple<int,store*>> sort_stores;
	int s_id=0;
	bool random_select = rand()%2;
	for(auto it = stores.begin();it!=stores.end(); it++,s_id++ ){
		sort_stores.push_back(make_tuple(s_id,&(*it)));
	}
	if(random_select)
		random_shuffle(sort_stores.begin(),sort_stores.end());

	for(unsigned int i=0; i<route_remaining_loads.size();i++) {
		if(route_remaining_loads[i]==0)
			continue;
		for(auto& s: sort_stores) {
			if(get<1>(s)->cur_demand > 0 && ((rand()%3) ? route_remaining_loads[i] >0 : get<1>(s)->cur_demand <= route_remaining_loads[i])) { //
				int reduce = min(route_remaining_loads[i],get<1>(s)->cur_demand);
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
		list<int>::iterator it;
		for(it= r.begin();it!=r.end();it++) {
			if(trucks[0].load==0)
				break;
			int reduce = min(trucks[0].load,stores[*it].cur_demand);
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
		list<int> new_route;
		trucks[0].load = trucks[0].capacity;
		for(auto &s: sort_stores) {
			if(trucks[0].load==0) {
				continue;
			}
			else {
				if(get<1>(s)->cur_demand > 0 && trucks[0].load >0) { // && s.cur_demand
					int reduce = min(trucks[0].load,get<1>(s)->cur_demand );
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
	//	for(int i=0;i<stores.size();i++) {
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

void splitRoutes(list<int>& solution, vector<list<int> >& routes) {
	for(auto it = solution.begin(); it != prev(solution.end(),1);it++) {
		if(*it==0) {
			routes.resize(routes.size()+1);
		}
		else {
			routes[routes.size()-1].emplace_back(*it);
		}
	}
}


void updateRoutes(list<int>& solution,vector<int>& it_routes_size, vector< list<int>::iterator >& it_routes) {
	int route_size = 0;
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


int checkSolution(list< int >& solution, vector< store >& stores, dist_mat& dist, vector< truck >& trucks, int &total_demand,bool fix,bool print=false) {
	vector<int> route_remaining_loads;

	updateDemand(solution,stores,trucks,route_remaining_loads,total_demand);

	if(fix) {
		while(total_demand > 0) {
			vector<list<int> > routes;
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
			if(print)
				cout << "fixed" << " " << total_demand <<endl;
		}
		//		cout << "total demand: " << total_demand << " remaining loads: ";
		//		for(auto r: route_remaining_loads) {
		//			cout << r << " ";
		//		}
		//		cout << endl;
	}
	return 0;
}


bool opt_2(list<int>::iterator& start, list<int>::iterator& end, int& route_size, dist_mat& dist){

	bool return_val = true;

	//-----implement evaluate_sublist-----

	int a = 1+rand() % (route_size-1);
	int b = 1+rand() % (route_size-1);

	//	if(a == 0)	a++;
	//	if(a == route_size) a--;
	//	if(b == 0) b++;
	//	if(b == route_size) b--;

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

	//	//-----implement evaluate_sublist-----
	//	int result = 0;
	//	evaluate_sublist_solution(result, start, end, dist);
	//
	//	if(result_old <= result){
	//
	//		it_start = next(start,min(a,b));
	//		it_end = next(start,max(a,b));
	//
	//		for (int i = 0; i < (max(a, b) - min(a, b) + 1) / 2; i++) {
	//
	//			auto val1 = *it_start;
	//			auto val2 = *it_end;
	//
	//			*it_end = val1;
	//			*it_start = val2;
	//
	//			it_start++;
	//			it_end--;
	//		}
	//
	//		return_val = false;
	//	}

	//	cout << "Result old: " << result_old << "  result: " << result << endl;

	return return_val;
}

int swap_n(list< int >& solution, vector< store >& stores, dist_mat& dist, vector< truck >& trucks, int swaps) {

	//-----implement evaluate_sublist-----

	int update=1;
	vector<int> it_routes_size;
	vector< list<int>::iterator > it_routes;
	for(int i=0;i<swaps;i++ ) {
		if(update)
			updateRoutes(solution,it_routes_size,it_routes);

		int select_1 =0;
		int select_2 =0;

		if((it_routes_size.size() - 1))
			select_1=rand() % (it_routes_size.size());
		if((it_routes_size.size() - 1))
			select_2=rand() % (it_routes_size.size());

		auto start_1 = it_routes.at(select_1), start_2 = it_routes.at(select_2);
		auto route_size_1 = it_routes_size.at(select_1),route_size_2 =it_routes_size.at(select_2);


		auto end_1 =  next(start_1,route_size_1);
		auto end_2 =  next(start_2,route_size_2);

		int a = 1+rand() % (route_size_1-1);
		int b;
		do {
			b = 1+rand() % (route_size_2-1);
		}
		while(start_1==start_2 && a==b);

		//		if(a == 0)	a++;
		//		if(a == route_size_1) a--;
		//		if(b == 0) b++;
		//		if(b == route_size_2) b--;

		auto swap_1 = next(start_1,a), swap_2 = next(start_2,b);

		if(start_1==start_2 || (end_2==find(start_2,end_2,*swap_1) && end_1==find(start_1,end_1,*swap_2) )) {
			auto temp = *swap_1;
			*swap_1 = *swap_2;
			*swap_2 = temp;
			update = 0;
		}
		else if(end_1!=find(start_1,end_1,*swap_2) && end_2==find(start_2,end_2,*swap_1)) {
			*swap_2 = *swap_1;
			solution.erase(swap_1);
			update = 1;
		}
		else if(end_1==find(start_1,end_1,*swap_2) && end_2!=find(start_2,end_2,*swap_1)) {
			*swap_1 = *swap_2;
			solution.erase(swap_2);
			update = 1;
		}
	}

	int total_demand;
	checkSolution(solution,stores,dist,trucks,total_demand,true);

	return 1;


}

int swap_2(list<int>::iterator& start_1, list<int>::iterator& start_2, int& route_size_1, int& route_size_2, list< int >& solution, vector< store >& stores, dist_mat& dist, vector< truck >& trucks){
	//-----implement evaluate_sublist-----
	auto end_1 =  next(start_1,route_size_1);
	auto end_2 =  next(start_2,route_size_2);

	int a = 1+rand() % (route_size_1-1);
	int b;

	do {
		b = 1+rand() % (route_size_2-1);
	}
	while(start_1==start_2 && a==b);


	//	if(a == 0)	a++;
	//	if(a == route_size_1) a--;
	//	if(b == 0) b++;
	//	if(b == route_size_2) b--;
	auto swap_1 = next(start_1,a), swap_2 = next(start_2,b);

	if(start_1==start_2) {
		auto temp = *swap_1;
		*swap_1 = *swap_2;
		*swap_2 = temp;
	}
	else {
		if(end_2==find(start_2,end_2,*swap_1) && end_1==find(start_1,end_1,*swap_2)) {
			auto temp = *swap_1;
			*swap_1 = *swap_2;
			*swap_2 = temp;
		}
		else if(end_1!=find(start_1,end_1,*swap_2) && end_2==find(start_2,end_2,*swap_1)) {
			*swap_2 = *swap_1;
			solution.erase(swap_1);
		}
		else if(end_1==find(start_1,end_1,*swap_2) && end_2!=find(start_2,end_2,*swap_1)) {
			*swap_1 = *swap_2;
			solution.erase(swap_2);
		}
	}

	int total_demand;

	checkSolution(solution,stores,dist,trucks,total_demand,true);

	return 1;
}

void move(list<int>::iterator& start_1, list<int>::iterator& start_2, int& route_size_1, int& route_size_2, list< int >& solution, vector< store >& stores, dist_mat& dist, vector< truck >& trucks){

	//-----implement evaluate_sublist-----

	int result_old_1 = 0;
	auto end_2 =  next(start_2,route_size_2);

	//	int a = 1+(rand() % route_size_1-1);
	//	int b = 1+(rand() % route_size_2-1);

	int a = 1+rand() % (route_size_1-1);
	int b;

	do {
		b = 1+rand() % (route_size_2-1);
	}
	while(start_1==start_2 && a==b);



	//	if(a == 0)	a++;
	//	if(a == route_size_1) a--;
	//	if(b == 0) b++;
	//	if(b == route_size_2) b--;

	list<int>::iterator it_insert_to, it_insert_from;

	for(it_insert_to = solution.begin(); it_insert_to!= next(start_2,b);it_insert_to++);
	for(it_insert_from = solution.begin(); it_insert_from!= next(start_1,a);it_insert_from++);

	if(start_1==start_2 || end_2==find(start_2,end_2,*it_insert_from))
		solution.insert(it_insert_to,*it_insert_from);
	solution.erase(it_insert_from);
	int total_demand;
	checkSolution(solution,stores,dist,trucks,total_demand,true);
}

void destroyAndRepairRoutes(list< int >& solution, vector< store >& stores, dist_mat& dist, vector< truck >& trucks, int n_routes) {

	vector<list<int> > routes;
	splitRoutes(solution,routes);
	n_routes=min(n_routes,(const int)routes.size());

	//------ delete n_routes routes ------
	int i=0;
	while(i<n_routes) {
		int rnd = rand() % routes.size();
		if(routes[rnd].size()>0) {
			routes[rnd].clear();
			i++;
		}
	}
	solution.clear();
	for(auto& r : routes) {
		if(r.size()>0) {
			solution.emplace_back(0);
			for(auto& s: r) {
				solution.emplace_back(s);
			}
		}
	}
	solution.emplace_back(0);
	//------ rebuild ------
	int total_demand;
	checkSolution(solution,stores,dist,trucks,total_demand,true);

}

void destroyAndRepairStations(list< int >& solution, vector< store >& stores, dist_mat& dist, vector< truck >& trucks, int n_routes) {

	//------ delete n_routes routes ------
	int i=0;
	while(i<n_routes) {
		int rnd = rand() % solution.size();
		if(*next(solution.begin(),rnd)>0) {
			solution.erase(next(solution.begin(),rnd));
			i++;
		}
	}
	//------ rebuild ------
	int total_demand;
	checkSolution(solution,stores,dist,trucks,total_demand,true);

}

void randomRouteOrder(list< int >& solution, vector< store >& stores, dist_mat& dist, vector< truck >& trucks) {

	vector<list<int> > routes;
	int total_demand;
	splitRoutes(solution,routes);
	random_shuffle(routes.begin(),routes.end());
	createSolutionFromRoutes(solution,routes);
	checkSolution(solution,stores,dist,trucks,total_demand,true);

}

void movePartsOfRoutes(list< int >& solution, vector< store >& stores, dist_mat& dist, vector< truck >& trucks, int a,int b, int n_max) {


	vector<list<int> > routes;
	int total_demand;
	splitRoutes(solution,routes);

	auto n_shortest = solution.size();

	n_shortest = (n_max<n_shortest) ? n_max : n_shortest;
	for(auto& r : routes)
		n_shortest = min(r.size(),n_shortest);



	int start_1=rand()%routes[a].size();
	int start_2=rand()%routes[b].size();

	int temp_ind;

	for(int i=0;start_1+i<routes[a].size() && start_2+i<routes[b].size() && i<n_shortest;i++) {
		temp_ind = *next(routes[a].begin(),start_1+i);
		*next(routes[a].begin(),start_1+i)=*next(routes[b].begin(),start_2+i);
		*next(routes[b].begin(),start_2+i)=temp_ind;
	}
	createSolutionFromRoutes(solution,routes);
	checkSolution(solution,stores,dist,trucks,total_demand,true);



}


#endif /* METAHEURISTICS_HPP_ */
