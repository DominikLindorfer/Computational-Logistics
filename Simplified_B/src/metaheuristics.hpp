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


auto removeDuplicates(list<long> solution) {



	return true;
}

void initial_solution(list< long >& solution, vector< store >& stores, dist_mat& dist, vector< truck >& truck){

	long n_stores = dist.distances.at(0).size();
	long max_dist = 0;
	long min_dist = 1e9;
	long ind_max = 0;
	long ind_min = 0;
	long remain_demand = 0;

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

		for(long i = 0; i < n_stores; i++){

			if(stores.at(i).cur_demand > 0){

				if(dist(0, i) > max_dist){
					max_dist = dist(0, i);
					ind_max = i;
				}
			}
		}

		solution.emplace_back(ind_max);

		long unload = min(stores.at(ind_max).cur_demand, truck.at(0).load);

		stores.at(ind_max).cur_demand -= unload;
		truck.at(0).load -= unload;
		remain_demand -= unload;

		while(truck.at(0).load > 0 && remain_demand > 0){

			for(long i = 1; i < n_stores; i++){

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

void evaluate_solution(long& result, list< long >& solution, dist_mat& dist){

	result = 0;

	auto it = solution.begin();
	for(long i = 0; i < solution.size() - 1; i++){

		long id = *it;
		result += dist(id , *(++it) );
	}
}

void evaluate_sublist_solution(long& result, list<long>::iterator& start, list<long>::iterator& end, dist_mat& dist){

	result = 0;

	for(auto it = start; it != end; ){

		long id = *it;
		result += dist(id , *(++it) );
	}
}


void createSolutionFromRoutes(list< long >& solution, vector<list<long>> &routes) {
	solution.clear();
	for(auto& r : routes) {
		solution.emplace_back(0);
		for(auto& s: r) {
			solution.emplace_back(s);
		}
	}
	solution.emplace_back(0);
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


long checkSolution(list< long >& solution, vector< store >& stores, dist_mat& dist, vector< truck >& trucks, long &total_demand,bool fix,bool prlong=false) {
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


bool opt_2(list<long>::iterator& start, list<long>::iterator& end, long& route_size, dist_mat& dist){

	bool return_val = true;

	//-----implement evaluate_sublist-----

	long a = 1+rand() % (route_size-1);
	long b = 1+rand() % (route_size-1);

	//	if(a == 0)	a++;
	//	if(a == route_size) a--;
	//	if(b == 0) b++;
	//	if(b == route_size) b--;

	auto it_start = next(start,min(a,b));
	auto it_end = next(start,max(a,b));
	//	cout << a << " " << b << endl;

	for(long i=0; i<(max(a,b)-min(a,b)+1)/2; i++) {

		auto val1 = *it_start;
		auto val2 = *it_end;

		*it_end=val1;
		*it_start=val2;

		it_start++;
		it_end--;
	}

	//	//-----implement evaluate_sublist-----
	//	long result = 0;
	//	evaluate_sublist_solution(result, start, end, dist);
	//
	//	if(result_old <= result){
	//
	//		it_start = next(start,min(a,b));
	//		it_end = next(start,max(a,b));
	//
	//		for (long i = 0; i < (max(a, b) - min(a, b) + 1) / 2; i++) {
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

long swap_n(list< long >& solution, vector< store >& stores, dist_mat& dist, vector< truck >& trucks, long swaps) {

	//-----implement evaluate_sublist-----

	long update=1;
	vector<long> it_routes_size;
	vector< list<long>::iterator > it_routes;
	for(long i=0;i<swaps;i++ ) {
		if(update)
			updateRoutes(solution,it_routes_size,it_routes);

		long select_1 =0;
		long select_2 =0;

		if((it_routes_size.size() - 1))
			select_1=rand() % (it_routes_size.size());
		if((it_routes_size.size() - 1))
			select_2=rand() % (it_routes_size.size());

		auto start_1 = it_routes.at(select_1), start_2 = it_routes.at(select_2);
		auto route_size_1 = it_routes_size.at(select_1),route_size_2 =it_routes_size.at(select_2);


		auto end_1 =  next(start_1,route_size_1);
		auto end_2 =  next(start_2,route_size_2);

		long a = 1+rand() % (route_size_1-1);
		long b;
		do {
			b = 1+rand() % (route_size_2-1);
		}
		while(start_1==start_2 && a==b && route_size_1>2 && route_size_2 >2);

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

	long total_demand;
	checkSolution(solution,stores,dist,trucks,total_demand,true);

	return 1;


}

long swap_2(list<long>::iterator& start_1, list<long>::iterator& start_2, long& route_size_1, long& route_size_2, list< long >& solution, vector< store >& stores, dist_mat& dist, vector< truck >& trucks){
	//-----implement evaluate_sublist-----
	auto end_1 =  next(start_1,route_size_1);
	auto end_2 =  next(start_2,route_size_2);

	long a = 1+rand() % (route_size_1-1);
	long b;

	do {
		b = 1+rand() % (route_size_2-1);
	}
	while(start_1==start_2 && a==b && route_size_1>2 && route_size_2 >2);


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

	long total_demand;

	checkSolution(solution,stores,dist,trucks,total_demand,true);

	return 1;
}

void move(list<long>::iterator& start_1, list<long>::iterator& start_2, long& route_size_1, long& route_size_2, list< long >& solution, vector< store >& stores, dist_mat& dist, vector< truck >& trucks){

	//-----implement evaluate_sublist-----

	long result_old_1 = 0;
	auto end_2 =  next(start_2,route_size_2);

	//	long a = 1+(rand() % route_size_1-1);
	//	long b = 1+(rand() % route_size_2-1);

	long a = 1+rand() % (route_size_1-1);
	long b;

	do {
		b = 1+rand() % (route_size_2-1);
	}
	while(start_1==start_2 && a==b && route_size_1>2 && route_size_2 >2);


	//	if(a == 0)	a++;
	//	if(a == route_size_1) a--;
	//	if(b == 0) b++;
	//	if(b == route_size_2) b--;

	list<long>::iterator it_insert_to, it_insert_from;

	for(it_insert_to = solution.begin(); it_insert_to!= next(start_2,b);it_insert_to++);
	for(it_insert_from = solution.begin(); it_insert_from!= next(start_1,a);it_insert_from++);

	if(start_1==start_2 || end_2==find(start_2,end_2,*it_insert_from))
		solution.insert(it_insert_to,*it_insert_from);
	solution.erase(it_insert_from);
	long total_demand;
	checkSolution(solution,stores,dist,trucks,total_demand,true);
}

void destroyAndRepairRoutes(list< long >& solution, vector< store >& stores, dist_mat& dist, vector< truck >& trucks, long n_routes) {

	vector<list<long> > routes;
	splitRoutes(solution,routes);
	n_routes=min(n_routes,(const long)routes.size());

	//------ delete n_routes routes ------
	long i=0;
	while(i<n_routes) {
		long rnd = rand() % routes.size();
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
	long total_demand;
	checkSolution(solution,stores,dist,trucks,total_demand,true);

}

void destroyAndRepairStations(list< long >& solution, vector< store >& stores, dist_mat& dist, vector< truck >& trucks, long n_routes) {

	//------ delete n_routes routes ------
	long i=0;
	while(i<n_routes) {
		long rnd = rand() % solution.size();
		if(*next(solution.begin(),rnd)>0) {
			solution.erase(next(solution.begin(),rnd));
			i++;
		}
	}
	//------ rebuild ------
	long total_demand;
	checkSolution(solution,stores,dist,trucks,total_demand,true);

}

void randomRouteOrder(list< long >& solution, vector< store >& stores, dist_mat& dist, vector< truck >& trucks) {

	vector<list<long> > routes;
	long total_demand;
	splitRoutes(solution,routes);
	random_shuffle(routes.begin(),routes.end());
	createSolutionFromRoutes(solution,routes);
	checkSolution(solution,stores,dist,trucks,total_demand,true);

}

void movePartsOfRoutes(list< long >& solution, vector< store >& stores, dist_mat& dist, vector< truck >& trucks, long a,long b, long n_max) {


	vector<list<long> > routes;
	long total_demand;
	splitRoutes(solution,routes);

	auto n_shortest = solution.size();

	//	n_shortest = (n_max<n_shortest) ? n_max : n_shortest;
	//	for(auto& r : routes)
	//		n_shortest = min(r.size(),n_shortest);



	long start_1=rand()%routes[a].size();
	long start_2=rand()%routes[b].size();

	long temp_ind;

	for(long i=0;start_1+i<routes[a].size() && start_2+i<routes[b].size() && i<n_max;i++) {
		temp_ind = *next(routes[a].begin(),start_1+i);
		*next(routes[a].begin(),start_1+i)=*next(routes[b].begin(),start_2+i);
		*next(routes[b].begin(),start_2+i)=temp_ind;
	}
	createSolutionFromRoutes(solution,routes);
	checkSolution(solution,stores,dist,trucks,total_demand,true);

}

void swap_neighbours_nn(list< long >& solution, vector< store >& stores, dist_mat& dist, vector< truck >& trucks) {
	long total_demand;

	long score_before;
	long score_after;

//	cout << "before swap n" << endl;

	for(auto it=next(solution.begin(),1);it!=prev(solution.end(),2);it++) {
		auto next_el = next(it,1);
		auto nnext_el = next(it,2);
		if(*next_el != 0 && *it != 0 && *nnext_el != 0) {
			score_before = dist(*prev(it,1),*it)+dist(*next_el,*nnext_el);
			score_after = dist(*prev(it,1),*next_el)+dist(*it,*nnext_el);
//			cout << "before swap" << endl;
			if(score_after<=score_before)
				iter_swap(it,next_el);
//			cout << "after swap" << endl;
		}
	}
//	cout << "after swap n" << endl;
//	cout << solution.size() << " " << endl;
	for(auto it=next(solution.begin(),1);it!=prev(solution.end(),3);it++) {
		auto next_el = next(it,1);
		auto nnext_el = next(it,2);
		auto nnnext_el = next(it,3);
		if(*nnext_el != 0 && *next_el != 0 && *it != 0 && *nnnext_el != 0) {
			score_before = dist(*prev(it,1),*it)+dist(*it,*next(it,1))+dist(*next(it,1),*next(it,2))+dist(*next(it,2),*next(it,3));
//			cout << "before swap" << endl;
			iter_swap(it,nnext_el);
//			cout << "after swap" << endl;
			score_after = dist(*prev(it,1),*it)+dist(*it,*next(it,1))+dist(*next(it,1),*next(it,2))+dist(*next(it,2),*next(it,3));
//			cout << "before back swap" << endl;
			if(score_before<score_after) {
				iter_swap(it,nnext_el);
			}
//			cout << "after back swap" << endl;
		}
//		cout << i << endl;
	}
//	cout << "after swap nn" << endl;
//	cout << "before check sol nn" << endl;
	checkSolution(solution,stores,dist,trucks,total_demand,true);
//	cout << "after check sol nn" << endl;
}


#endif /* METAHEURISTICS_HPP_ */
