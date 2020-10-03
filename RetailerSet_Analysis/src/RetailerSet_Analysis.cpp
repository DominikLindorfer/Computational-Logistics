//============================================================================
// Name        : RetailerSet_Analysis.cpp
// Author      : lindorfer
// Version     :
// Copyright   : Your copyright notice
// Description : Analyze Datasets with ease!
//============================================================================

#include <bits/stdc++.h>
#include "io.hpp"
#include "datastructures.hpp"
#include "metaheuristics.hpp"
#include "Utilities.hpp"
using namespace std;

int main() {
	//	string TravTime = "Data/TravelTim1_test.csv";
	//	string Demand = "Data/Demand1_test.csv";
	//	string ServiceTime = "Data/StoreServiceTimes_Test.csv";

	string TravTime = "Data/TravelTim1.csv";
	string Demand = "Data/demands_agg_singleday.csv";
	string ServiceTime = "Data/ServiceTimes_all.csv";

	vector<vector <string> > data;
	vector< tuple<string,string,string>> distances;
	vector< vector< tuple<string,string,string,tuple<string,string>>> > demands;
	vector< tuple<string,string,string, string> > service_times;

	long numb_Days = 0;
	read_csv_distance(TravTime, data, distances);
	read_csv_demand_multipleDays(Demand, demands, numb_Days);
	read_csv_servicetimes(ServiceTime, service_times);

	//	vector< tuple<string,string,string>> distances(data.size());
	long t_load = 1 * 60;
	long t_fix_load = 10 * 60;
	long t_unload = 1 * 60;
	long t_fix_unload = 10 * 60;

	//-----initialize distance matrix-----
	long n_stores = service_times.size() + 1;
	dist_mat dist(n_stores);

	for(auto d : distances){

		long i = stoi(get<0>(d)) - 1;
		long j = stoi(get<1>(d)) - 1;

		dist(i, j) = stoi(get<2>(d));
	}


	//	//-----Symmetry Operations in Distances-----
	//	dist_mat dist_sym = dist;
	//
	//	for(int i = 0; i < n_stores; i++){
	//		for(int j = 0; j < n_stores; j++){
	//
	//			dist_sym(i, j) = dist(i, j) / 2 + dist(j, i) / 2;
	//		}
	//	}
	//	dist = dist_sym;

	//-----initialize stores-----
	vector< store > stores(n_stores);

	cout << n_stores << endl;


	//-----push back the HUBs-----
	vector<long> hub_demand(numb_Days, 0);

	stores.at(0).tot_demand = hub_demand;
	stores.at(0).cur_demand = hub_demand;

	//-----set demands for stores-----
	for(auto& s : stores){
		s.cur_demand.resize(numb_Days,0);
		s.tot_demand.resize(numb_Days,0);
	}

	long day = 0;
	for(auto demand : demands){
		for(auto s : demand){

			long id = stoi(get<0>(s));
			long d = stoi( get<1>(get<3>(s)) );

			stores.at(id - 1).tot_demand[day] += d;
			stores.at(id - 1).cur_demand[day] += d;
			stores.at(id - 1).id = id - 1;
		}
		day++;
	}

	//-----Set Service Times-----
	for(auto s : service_times){

		long id = stoi(get<0>(s)) - 1;
		string start = get<2>(s);
		string end = get<3>(s);

		istringstream stream_start(start);
		istringstream stream_end(end);

		string token_s;
		string token_e;

		long start_time = 0;
		long end_time = 0;

		vector<string> vec_line_s;
		vector<string> vec_line_e;

		while(getline(stream_start, token_s, ':')) {
			vec_line_s.push_back(token_s);
		}

		while(getline(stream_end, token_e, ':')) {
			vec_line_e.push_back(token_e);
		}

		start_time += stoi(vec_line_s[0]) * 3600;
		start_time += stoi(vec_line_s[1]) * 60;

		end_time += stoi(vec_line_e[0]) * 3600;
		end_time += stoi(vec_line_e[1]) * 60;

		if(end_time == 0){
			end_time = 24 * 3600;
		}

		for(auto& st : stores){
			if(id == st.id){
				st.service_time.first = start_time;
				st.service_time.second = end_time;
				break;
			}
		}
	}

	//-----Print out Total Demand and Service Times-----
	for(auto s : stores){
		for(auto i : s.tot_demand)
			cout << i << " ";
		cout << endl;
	}

	for(auto s: stores){
		cout << s.id << " " << s.service_time.first << " " <<s.service_time.second << endl;
	}


	//-----Check # of stores && Stores with Opening Hours-----
	vector< store > stores_ophours;

	for(auto s : stores){

		if(s.service_time.first == 3600 && s.service_time.second == 86400){
			continue;
		}
		stores_ophours.push_back(s);
	}

	cout << stores.size() << endl;
	cout << stores_ophours.size() << endl;

	//-----Check longest & shortest Opening Hours-----
	sort(stores_ophours.begin(), stores_ophours.end(), [](store a, store b){
		return (a.service_time.second - a.service_time.first) < (b.service_time.second - b.service_time.first);
	});

	for(long i = 0; i < 5; i++){
		cout << stores_ophours[i].service_time.first << " " << stores_ophours[i].service_time.second << endl;
	}
	auto size_oa = stores_ophours.size();
	cout << stores_ophours[size_oa-1].service_time.first << " " << stores_ophours[size_oa-1].service_time.second << endl;

	//-----count stores with under 6h & 12h opening time-----
	long count_6h = 0;
	for(auto s : stores_ophours){

		if((s.service_time.second - s.service_time.first) <= 6 * 3600){
			count_6h++;
		}
	}

	long count_12h = 0;
	for(auto s : stores_ophours){

		if((s.service_time.second - s.service_time.first) <= 12 * 3600){
			count_12h++;
		}
	}

	long count_18h = 0;
	for(auto s : stores_ophours){

		if((s.service_time.second - s.service_time.first) <= 18 * 3600){
			count_18h++;
		}
	}

	cout << "6h: " << count_6h << "  12h: " << count_12h << "  18h: " << count_18h << endl;

	//-----Analyze Distances-----
	cout << dist.distances[0] << endl;

	ofstream outdist("distout.dat");

	for(auto d : dist.distances){
		outdist << d << "," << endl;
//		cout << d << endl;
	}
	outdist.close();

//	exit(1);

	//-----initialize trucks-----
	long n_trucks = 1; //3
	vector< truck > trucks_(n_trucks);

	for(auto& i : trucks_){
		i.capacity = 33;	//60
	}

	//-----initialize docks-----
	long n_docks = 30; //2

	//-----build nearest neighbor matrix-----
	vector<vector< tuple <long, long>>> nn_mat;
	build_nn_mat(dist, nn_mat, stores);
//
//	for(long i = 0; i < (long)nn_mat.size(); i++){
//		cout << "{";
//		for(long j = 0; j < (long)nn_mat[i].size();j++){
//			cout << "(" << get<0>(nn_mat[i][j]) << "," << get<1>(nn_mat[i][j]) << ")" << ",";
//		}
//		cout << "}" << endl << endl;
//	}

	//-----Analyze the Demand-----
	double mean_demand = 0;
	for(auto s : stores){

		mean_demand += s.tot_demand[0];

	}
	mean_demand = (double)mean_demand/stores.size();

	double mean_demand_ophours = 0;
	for(auto s : stores_ophours){

		mean_demand_ophours  += s.tot_demand[0];

	}
	mean_demand_ophours  = (double)mean_demand_ophours /stores_ophours.size();

	cout << "Mean Demand: " << mean_demand << "  Mean Demand OA: " << mean_demand_ophours << endl;

	ofstream demand_out("demand_all.dat");
	demand_out << "{";
	for(auto s : stores){
		demand_out << s.tot_demand << ",";
	}
	demand_out << "}";
	demand_out.close();

	demand_out.open("demand_all_ophours.dat");
	demand_out << "{";
	for(auto s : stores_ophours){
		demand_out << s.tot_demand << ",";
	}
	demand_out << "}";
	demand_out.close();

	return 0;
}
