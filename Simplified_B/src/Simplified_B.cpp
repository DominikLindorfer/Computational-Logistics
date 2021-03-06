//============================================================================
// Name        : Simplified_B.cpp
// Author      : lindorfer
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include <bits/stdc++.h>


#include "io.hpp"
#include "datastructures.hpp"
#include "metaheuristics.hpp"

using hr_clk = chrono::high_resolution_clock;

using namespace std;

int main() {



//	cout << sizeof(long) << " " << sizeof(int) << endl;


	hr_clk::time_point t1_hr = hr_clk::now();


	long n_runs =150-1;
	long n_trys = 100;
	long total_demand;
	long level =0;
	long not_changed=0;
	long not_changed_best=0;
	long temp_result =0;
	long min_level=0;
	long max_level = 8;
	long change_level =0;
	long best_result_run=0;
	long initial_score;
	bool no_reset_lvl = false;
	bool new_best_run_found=false;

	long start_T = 30;
	long T_cycle =10;
	double T;
	double T_min=0;
	double T_max=1;
	double p=.1;
	double d=.01;
	double scalingfactor=-pow(T_max,3)*log(p)/d;
//	double gamma = 1/10000.;
	long iteration=0;


//	vector<long> cap_array={9,18,36,50,100,150,200,300,375,750};
	vector<long> cap_array={36,50,100,150,200,300,375,750};

	for(long cap = 0; cap<cap_array.size(); cap++ ) {
		max_level=8;
		long total_demand_=0;
		srand(time(0));
		string filename;


//		string TravTime = "Data/TravelTim1_test.csv";
//		string Demand = "Data/Demand1_test.csv";
		string TravTime = "Data/TravelTim1_test_2.csv";
		string Demand = "Data/Demand1.csv";

		vector<vector <string> > data;
		vector< tuple<string,string,string>> distances;
		vector< tuple<string,string,string,tuple<string,string>>> demands;

		read_csv_distance(TravTime, data,distances);
		read_csv_demand(Demand,demands);
		//	vector< tuple<string,string,string>> distances(data.size());

		//-----initialize distance matrix-----
		long n_stores = sqrt(distances.size());
		dist_mat dist(n_stores);

		for(auto d : distances){
			long i = stoi(get<0>(d)) - 1;
			long j = stoi(get<1>(d)) - 1;
			dist(i, j) = stoi(get<2>(d));
		}

		//-----initialize stores-----
		vector< store > stores(n_stores);
		stores.at(0).tot_demand = 0;
		stores.at(0).cur_demand = 0;

		//-----set demands for stores-----
		for(auto s : demands){
			long id = stoi(get<0>(s));
			long d = stoi( get<1>(get<3>(s)) );

			stores.at(id - 1).tot_demand += d;
			stores.at(id - 1).cur_demand += d;
			total_demand_+=d;
		}

		int sdf=0;
		for(auto &s : stores){
			sdf++;
			if(sdf==1) continue;
			s.tot_demand =5+rand()%96;
			s.cur_demand = s.tot_demand;
			cout << s.tot_demand << endl;
		}
		//-----initialize trucks-----
		vector< truck > truck(1);
		truck.at(0).capacity = cap_array[cap];
		truck.at(0).load = truck.at(0).capacity;

		//-----build initial solution-----
		list< long > solution;
		initial_solution(solution, stores, dist, truck);

		long store_coord_[stores.size()][2];

		ifstream store_coord("stores.txt");

		for(long i=0;i<stores.size();i++) {
			store_coord >> store_coord_[i][0] >> store_coord_[i][1];
		}
		store_coord.close();

		cout << solution.size() << endl;
		for(auto i : solution){
			cout << i+1 << ",";
		}
		cout << endl;

		long result = 0;
		evaluate_solution(result, solution, dist);
		initial_score=result;
		cout << "Result: " << result << endl;

		vector<list<long>> routes_plt;
		splitRoutes(solution,routes_plt);
		//-----split into sublists-----
		long max_notchange =0;

		auto best_solution = solution ;
		long best_result =result;
		auto temp_solution = solution;
		vector<long> it_routes_size;
		vector< list<long>::iterator > it_routes;
		updateRoutes(solution,it_routes_size,it_routes);
		vector<long> test;

		long accept_ =0;



		for(long runs =0 ;runs <= n_runs ; runs++ ) {
			level =min_level;
			best_result_run = (start_T>runs) ? 1000*1000*1000 : 1.0*best_result_run;
			new_best_run_found=false;
			T = (runs <start_T) ? 0 : pow((T_max-T_min)*(1-1.*((runs-start_T)%T_cycle)/(T_cycle-1)) + T_min,3);
//			T = (runs <start_T) ? 0 : pow((T_max-T_min)*(1-1.*((runs-start_T)%T_cycle)/(T_cycle-1)) + T_min,1);
//			gamma = 1/10000. * (1+1.*((runs-start_T)%T_cycle)/(T_cycle-1));
//			T = (runs <start_T) ? 0 : (T_max-T_min)* pow(sin(M_PI* (runs-start_T)/T_cycle),2);
			if(start_T==runs) {
				not_changed_best=0;
				max_level=7;
				solution=best_solution;
				no_reset_lvl = true;
				evaluate_solution(result,solution,dist);
			}


			while(level<=max_level) {
				//				auto max_steps=(10.*n_trys/(level/3+1));
				auto max_steps=(10*n_trys);
				not_changed=0;
				max_notchange=0;
				accept_=0;
				change_level=1;
				for(long i = 0; i < max_steps; i++) {
					long select_1=0;
					long select_2=0;
					temp_solution=solution;
					updateRoutes(temp_solution,it_routes_size,it_routes);

					if(it_routes_size.size() - 1)
						select_1 = rand() % (it_routes_size.size());
					if(it_routes_size.size() - 1)
						select_2 = rand() % (it_routes_size.size());
					vector<list<long>> routes;
					switch(level) {
					case 0:
						splitRoutes(temp_solution,routes);
						opt_2(it_routes.at(select_1), it_routes.at(select_1 + 1), it_routes_size.at(select_1), dist);
						checkSolution(temp_solution,stores,dist,truck,total_demand,true);
						break;
					case 1:
						move(it_routes.at(select_1), it_routes.at(select_1), it_routes_size.at(select_1),it_routes_size.at(select_1), temp_solution, stores,dist,truck);
						break;
					case 2:
						swap_2(it_routes.at(select_1), it_routes.at(select_1), it_routes_size.at(select_1),it_routes_size.at(select_1), temp_solution, stores,dist,truck);
						break;
					case 3:
						movePartsOfRoutes(temp_solution,stores,dist,truck,select_1,select_2,2+rand()%10);
						break;
					case 4:
						swap_2(it_routes.at(select_1), it_routes.at(select_2), it_routes_size.at(select_1),it_routes_size.at(select_2), temp_solution, stores,dist,truck);
						break;
					case 5:
						//					updateRoutes(temp_solution,it_routes_size,it_routes);
						move(it_routes.at(select_1), it_routes.at(select_2), it_routes_size.at(select_1),it_routes_size.at(select_2), temp_solution, stores,dist,truck);
						break;
					case 6:
						swap_n(temp_solution, stores,dist,truck,rand()%temp_solution.size()/4+1);
						break;
					case 7:
						//					cout << "before entering case 7" << endl;
						swap_neighbours_nn(temp_solution, stores,dist,truck);
						//					cout << "after entering case 7" << endl;
						break;
					case 8:
						temp_solution= (rand()%2) ? best_solution : solution;
						//					temp_solution= best_solution;
						updateRoutes(solution,it_routes_size,it_routes);

						if(rand()%2)
							destroyAndRepairStations(temp_solution,stores,dist,truck,(rand()%stores.size()/3)+1);
						else if(rand()%2) {
							if(it_routes_size.size()>1)
								destroyAndRepairRoutes(temp_solution,stores,dist,truck,min((long)2,(long)(rand()%it_routes.size()/2)+1));
						}
						else {
							randomRouteOrder(temp_solution,stores,dist,truck);
						}

						cout << "solution after destroy" << endl;
						evaluate_solution(result,temp_solution,dist);
						cout << "solution: " << result << endl;
						for(auto i : temp_solution){
							cout << i+1 << ",";
						}
						cout << endl;
						i=max_steps;
						not_changed=max_steps;
						solution = temp_solution;
						level++;
						break;
					default:
						level=max_level+1;
					}

					evaluate_solution(temp_result,temp_solution,dist);
					//				auto accept=min(exp((result-temp_result)/tempscale),1.) >= (1.*rand())/RAND_MAX;
					//				long accept = (-result+temp_result < best_result*.001*pow(scalingfactor,3)) ? !(rand()%3) : false;
					//				long accept = (-result+temp_result < best_result*.05*pow(1*runs/n_runs,5)) ? !(rand()%4) : false;
//					double f_stun_1 = 1-sinh(-gamma* (result-best_result));
//					double f_stun_2 = 1-sinh(-gamma* (temp_result-best_result));
					auto testing_factor = exp( scalingfactor*(result-temp_result)/(T * best_result));
//					auto testing_factor = exp( -(f_stun_2-f_stun_1)/(T));

//					cout << temp_result << " " << result << " " << best_result << " "<< f_stun_2 << " " << f_stun_1 <<" " <<testing_factor << endl;

					auto temp_rnd=1.*rand()/RAND_MAX;
					bool accept = (testing_factor>=temp_rnd) ? true : false;

					//				if(start_T<=runs) {
					//					cout << best_result << " " << best_result_run << " " << result << " " << temp_result << endl;
					//				}

					if(temp_result<best_result_run) {
						best_result_run = temp_result;
						//					cout << "new run best: " << best_result_run << endl;
						new_best_run_found=true;
						//					cout << "LEVEL RESET" << endl;
					}
					if(new_best_run_found) {
						//					if(level==7 && !no_reset_lvl) {
						if(level<=max_level && !no_reset_lvl) {
							change_level=0;
							level=min_level;
							new_best_run_found=false;
						}
					}
					if(accept) {
						//				if(temp_result<=result) {
						solution=temp_solution;
						if(temp_result!=result)
							accept_++;
						result = temp_result;
					}
					if(accept) {
						//				if(temp_result<result) {
						not_changed=0;
					}
					else {
						not_changed++;
					}

					//				if(runs>=start_T)
					//					cout << result << endl;

					if(result<best_result) {
						best_solution = solution;
						best_result = result;
						//					level =0;
						not_changed_best=0;

						vector<list<long>> routes_plt;
						splitRoutes(best_solution,routes_plt);
					}
					else {
						not_changed_best++;
					}
					max_notchange = max(max_notchange,not_changed);
					//				scores << iteration << " " << result << endl;
					iteration++;
					if(level==7 && i>=10)
						break;
				}
				cout << "---------------------------------------------" << endl;
				cout << "current level: " << level << " i_runs: " << runs << "/" << n_runs << endl;
				//			if(max_notchange>=max_steps/ratio_) {
				if(change_level) {
					level++;
				}
				cout <<  "Temp: " << T << " gamma: "<< gamma << " max_notchange: " << max_notchange  << " " << "not_changed_best: " << not_changed_best  << endl;
				//			updateDemand(temp_solution,stores,truck,test,total_demand);
				//			if(total_demand)
				//				cout << "total_demand:" << total_demand << endl;
				cout << "Best solution: " << best_result << endl;

				cout << "acceptance ration: " << accept_ << " "<< max_steps << " " << (1.*accept_/max_steps) << " " << scalingfactor <<  endl;

				//			if((1.*accept_)/max_steps>=0.15)
				//				scalingfactor*=.99;
				//			else
				//				scalingfactor*=1.01;

				//			if(not_changed_best>1000*1000*10)
				//				tempscale*=.9;
			}
			//		tempscale*=.99;

			evaluate_solution(result,solution,dist);
			cout << "solution: " << result << endl;
			for(auto i : solution){
				cout << i+1 << ",";
			}
			cout << endl;
			evaluate_solution(best_result,best_solution,dist);
			cout << "best solution: " << best_result << endl;
			for(auto i : best_solution){
				cout << i+1 << ",";
			}
			cout << endl;


			//		if(not_changed_best>=50*1000*1000)
			//			break;
		}
		//		sol << "best solution: " << endl;
		//		for(auto i : best_solution){
		//			sol << i+1 << ",";
		//		}
		//		sol << " Result: " << best_result << endl;

	}

	//	scores.close();

	//	sol << "---------------------------------------------------";
	hr_clk::time_point t2_hr = hr_clk::now();
	auto time_span_3 = chrono::duration_cast< chrono::duration<double> >(t2_hr - t1_hr);
	cout << endl<<  time_span_3.count() << endl;


	return 0;
}
