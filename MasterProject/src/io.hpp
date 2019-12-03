/*
 * io.hpp
 *
 *  Created on: May 8, 2019
 *      Author: lindorfer
 */

#ifndef IO_HPP_
#define IO_HPP_
#include <bits/stdc++.h>
#include "datastructures.hpp"
using namespace std;

void read_csv_demand(string& file,vector< tuple<string,string,string,tuple<string,string> > >& demands){
	ifstream in;
	in.open(file);
	string line;

	getline(in,line);

	while(in.good()){

		getline(in, line);
		istringstream ss(line);
		string token;

		vector<string> vec_line;

		while(getline(ss, token, ';')) {
			vec_line.push_back(token);
		}
		if(vec_line.size()>0)
			demands.push_back(make_tuple(vec_line.at(2).substr(5, vec_line.at(2).length()- 5 - 1), vec_line.at(0), vec_line.at(1),make_tuple(vec_line.at(3),vec_line.at(4))));
			//----- Store#, Month, Day, <Type, Amount> -----
	}

	auto sort_tuple = [] (tuple<string,string,string,tuple<string,string>> a , tuple<string,string,string,tuple<string,string>> b) { return tie(a) < tie(b); };
	//		try {
	//			unsigned long i=0;
	//	//		cout << distances.size() << endl;
	//			for(auto d : demands) {
	//	//			cout << i << " " << d.size() << endl;
	//				//			distances.at(i) = make_tuple(d.at(0),d.at(2),d.at(1));
	//				if(d.size()==3)
	//					distances.push_back(make_tuple(d.at(0),d.at(2),d.at(1)));
	//				i++;
	//			}
	//		}
	//		catch (exception &e) {
	//			cout << e.what() << endl;
	//		}
	sort(demands.begin(),demands.end(),sort_tuple);
	cout << demands.size() << endl;
	for(auto d : demands) {
		cout << get<0>(d) << " " << get<1>(d) << " " << get<2>(d) <<" " <<  get<0>(get<3>(d)) << " "  <<get<1>(get<3>(d)) << " "  << endl;
	}
	cout << "test" << endl;


}

void read_csv_demand_multipleDays(string& file,vector < vector< tuple<string,string,string,tuple<string,string> > > >& demands, long& numb_days){

	//-----Read in first_day from demand-file-----
	ifstream in;
	in.open(file);
	string line;

	long first_day = -99;

	getline(in, line);
	getline(in, line);
	istringstream ss_(line);
	string token;

	vector<string> vec_line_;

	while(getline(ss_, token, ';')) {
		vec_line_.push_back(token);
	}
	if(vec_line_.size()>0){
		first_day = stoi(vec_line_.at(1));
	}
	in.close();


	//-----Re-Open and Read in demand-file-----
	in.open(file);
	long i = 1;
	getline(in,line);

	while(in.good()){

		getline(in, line);
		istringstream ss(line);
		string token;

		vector<string> vec_line;

		while(getline(ss, token, ';')) {
			vec_line.push_back(token);
		}
		if(vec_line.size()>0){

			if(stoi(vec_line.at(1)) != first_day){
				i++;
				first_day = stoi(vec_line.at(1));
			}
			demands.resize(i);
			demands[i-1].push_back(make_tuple(vec_line.at(2).substr(5, vec_line.at(2).length()- 5 - 1), vec_line.at(0), vec_line.at(1),make_tuple(vec_line.at(3),vec_line.at(4))));
			//----- Store#, Month, Day, <Type, Amount> -----
		}
	}

//	auto sort_tuple = [] (tuple<string,string,string,tuple<string,string>> a , tuple<string,string,string,tuple<string,string>> b) { return tie(a) < tie(b); };
//	sort(demands.begin(),demands.end(),sort_tuple);
	cout << demands.size() << endl;
	for(auto dd : demands) {
		for(auto d : dd)
		cout << get<0>(d) << " " << get<1>(d) << " " << get<2>(d) <<" " <<  get<0>(get<3>(d)) << " "  <<get<1>(get<3>(d)) << " "  << endl;
	}

	numb_days = i;
}

void read_csv_distance(string& file, vector<vector <string> >& data, vector< tuple<string,string,string> >& distances	){

	ifstream in;
	in.open(file);
	string line;

	//-----Reads in the Header-----
	getline(in,line);

	//-----Reads in the actual Data-----
	while(in.good()){

		getline(in, line);
		istringstream ss(line);
		string token;

		vector<string> vec_line;

		while(getline(ss, token, ';')) {
			vec_line.push_back(token);
		}

		data.push_back(vec_line);
	}
	//sort distances
	auto sort_tuple = [] (tuple<string,string,string> a , tuple<string,string,string> b) { return tie(a) < tie(b); };
	try {
		unsigned long i=0;
		//		cout << distances.size() << endl;
		for(auto d : data) {
			//			cout << i << " " << d.size() << endl;
			//			distances.at(i) = make_tuple(d.at(0),d.at(2),d.at(1));
			if(d.size()==3)
				distances.push_back(make_tuple(d.at(0).substr(4, d.at(0).length() - 4), d.at(2).substr(4, d.at(2).length() - 4), d.at(1)));
			i++;
		}
	}
	catch (exception &e) {
		cout << e.what() << endl;
	}
	sort(distances.begin(),distances.end(),sort_tuple);
	//	cout << distances.size() << endl;
	//	for(auto d : distances) {
	//		cout << get<0>(d) << " " << get<1>(d) << " "<< get<2>(d) << endl;
	//	}
}

void read_csv_servicetimes(string& file, vector<tuple<string, string, string, string>>& service_times){

	ifstream in;
	in.open(file);
	string line;

	getline(in,line);

	while(in.good()){

		getline(in, line);
		istringstream ss(line);
		string token;

		vector<string> vec_line;

		while(getline(ss, token, ';')) {
			vec_line.push_back(token);
		}
		if(vec_line.size()>0)
			service_times.push_back(make_tuple(vec_line.at(0).substr(5, vec_line.at(0).length()- 5 - 1), vec_line.at(1), vec_line.at(2).substr(1, vec_line.at(2).length()- 2), vec_line.at(3).substr(1, vec_line.at(3).length()- 2)));
		//----- Store#, Timeframe, Start Time, End Time -----
	}

	for(auto d : service_times)
		cout << get<0>(d) << " " << get<1>(d) << " " << get<2>(d) << " " << get<3>(d) << " "  << endl;
}

//void prlong_docks(vector< dock >& docks){
//
//	cout << endl << endl << "Mathematica Output: " << endl;
//
//	cout << "{";
//
//	for(auto i = docks.begin(); i != docks.end(); i++){
//
//		cout << "{";
//
//		if( get<0>(*(*i).jobs.begin()) != 0){
//			cout << "0," << get<0>(*(*i).jobs.begin()) << ",";
//		}
//
//		for(auto it = (*i).jobs.begin(); it != prev((*i).jobs.end(), 1); it++){
//
//
//			cout << get<1>(*it) - get<0>(*it) << "," << get<0>( *next(it, 1) ) - get<1>(*it) << ",";
//
////			if(it != prev((*i).jobs.end(), 2)){
////
////			}
////			else{
////				cout << get<1>(*it) - get<0>(*it) << "," << get<0>( *next(it, 1) ) - get<1>(*it);
////			}
//		}
//
//		cout << get<1>(*prev((*i).jobs.end(), 1)) - get<0>(*prev((*i).jobs.end(), 1));
//
//		if(i != prev(docks.end(),1))
//			cout << "},";
//		else
//			cout << "}";
//	}
//	cout << "}" << endl;
//}
//
//void prlong_docks_mathematicaLabel(vector< dock >& docks){
//
//	cout << endl << endl << "Mathematica Output for Docks with Labels: " << endl;
//
//	cout << "{";
//
//	for(auto i = docks.begin(); i != docks.end(); i++){
//
//		cout << "{";
//
//		if( get<0>(*(*i).jobs.begin()) != 0){
//			cout << "0," << get<0>(*(*i).jobs.begin()) << ",";
//		}
//
//		for(auto it = (*i).jobs.begin(); it != prev((*i).jobs.end(), 1); it++){
//
//			long d_first = get<1>(*it) - get<0>(*it);
//			long d_break = get<0>( *next(it, 1) ) - get<1>(*it);
//
//			cout << "Style[Labeled[" << d_first << ", Style[\"Truck " << get<2>(*it)+1 << "\", Black, 12], Center], ColorData[\"BrightBands\"," << (double)get<2>(*it)/10.0 << "]]";
//			cout << "," ;
//			cout << d_break;
//			cout << ",";
//
//		}
//
//		long d_last = get<1>(*prev((*i).jobs.end(), 1)) - get<0>(*prev((*i).jobs.end(), 1));
//
//		cout << "Style[Labeled[" << d_last << ", Style[\"Truck " << get<2>(*prev((*i).jobs.end(), 1))+1 << "\", Black, 12], Center], ColorData[\"BrightBands\"," << get<2>(*prev((*i).jobs.end(), 1))/10.0 << "]]";
//
//		if(i != prev(docks.end(),1))
//			cout << "},";
//		else
//			cout << "}";
//	}
//	cout << "}" << endl;
//}
//
//void prlong_trucks_mathematicaLabel(vector< truck >& trucks){
//
//	cout << endl << endl << "Mathematica Output for Trucks with Labels: " << endl;
//
//	cout << "{";
//
//
//	for(auto i = trucks.begin(); i != trucks.end(); i++){
//
//		cout << "{";
//
//		if( get<0>(*(*i).jobs.begin()) != 0){
//			cout << "0," << get<0>(*(*i).jobs.begin()) << ",";
//		}
//
//		for(auto it = (*i).jobs.begin(); it != prev((*i).jobs.end(), 1); it++){
//
//			long d_first = get<1>(*it) - get<0>(*it);
//			long d_break = get<0>( *next(it, 1) ) - get<1>(*it);
//
//			cout << "Style[Labeled[" << d_first << ", Style[\"Job " << get<2>(*it) << "\", Black, 12], Center], Lighter[ColorData[\"Rainbow\"," << (double)get<2>(*it)/10.0 << "]]]";
//			cout << "," ;
//			cout << d_break;
//			cout << ",";
//		}
//		long d_last = get<1>(*prev((*i).jobs.end(), 1)) - get<0>(*prev((*i).jobs.end(), 1));
//
//		cout << "Style[Labeled[" << d_last << ", Style[\"Job " << get<2>(*prev((*i).jobs.end(), 1)) << "\", Black, 12], Center], Lighter[ColorData[\"Rainbow\"," << get<2>(*prev((*i).jobs.end(), 1))/10.0 << "]]]";
//
//		if(i != prev(trucks.end(),1))
//			cout << "},";
//		else
//			cout << "}";
//	}
//	cout << "}" << endl;
//}

void print_docks_mathematicaLabel(vector< dock >& docks, int cur_day){

	//-----Print out the Solution-----

//	cout << "Current day: " << cur_day << endl;
//	cur_dock = 0;
//	for(auto i : docks_best){
//		//list< tuple <long, long, long> > jobs;
//		cout << "Dock_id: " << cur_dock << endl;
//		cur_dock++;
//		for(auto j : i.jobs[cur_day]){
//			cout << "{";
//			cout << get<0>(j) << " , " << get<1>(j) << " , " << get<2>(j);
//			cout << "},"<< endl;
//		}
//	}

	cout << endl << endl << "Mathematica Output for Docks with Labels: " << endl;

	cout << "{";

	for(auto i = docks.begin(); i != docks.end(); i++){

		cout << "{";

		if( get<0>(*(*i).jobs[cur_day].begin()) != 0){
			cout << "0," << get<0>(*(*i).jobs[cur_day].begin()) << ",";
		}

		for(auto it = (*i).jobs[cur_day].begin(); it != prev((*i).jobs[cur_day].end(), 1); it++){

			int d_first = get<1>(*it) - get<0>(*it);
			int d_break = get<0>( *next(it, 1) ) - get<1>(*it);

			cout << "Style[Labeled[" << d_first << ", Style[\"Truck " << get<2>(*it)+1 << "\", Black, 12], Center], ColorData[\"BrightBands\"," << (double)get<2>(*it)/10.0 << "]]";
			cout << "," ;
			cout << d_break;
			cout << ",";

		}

		int d_last = get<1>(*prev((*i).jobs[cur_day].end(), 1)) - get<0>(*prev((*i).jobs[cur_day].end(), 1));

		cout << "Style[Labeled[" << d_last << ", Style[\"Truck " << get<2>(*prev((*i).jobs[cur_day].end(), 1))+1 << "\", Black, 12], Center], ColorData[\"BrightBands\"," << get<2>(*prev((*i).jobs[cur_day].end(), 1))/10.0 << "]]";

		if(i != prev(docks.end(),1))
			cout << "},";
		else
			cout << "}";
	}
	cout << "}" << endl;
}


void print_trucks_mathematicaLabel(vector< truck >& trucks, int cur_day){

	cout << endl << endl << "Mathematica Output for Trucks with Labels: " << endl;

	cout << "{";

	for(auto i = trucks.begin(); i != trucks.end(); i++){

		cout << "{";

		if( get<0>(*(*i).jobs[cur_day].begin()) != 0){
			cout << "0," << get<0>(*(*i).jobs[cur_day].begin()) << ",";
		}

		for(auto it = (*i).jobs[cur_day].begin(); it != prev((*i).jobs[cur_day].end(), 1); it++){

			int d_first = get<1>(*it) - get<0>(*it);
			int d_break = get<0>( *next(it, 1) ) - get<1>(*it);

			cout << "Style[Labeled[" << d_first << ", Style[\"Job " << get<2>(*it) << "\", Black, 12], Center], Lighter[ColorData[\"Rainbow\"," << (double)get<2>(*it)/10.0 << "]]]";
			cout << "," ;
			cout << d_break;
			cout << ",";
		}
		int d_last = get<1>(*prev((*i).jobs[cur_day].end(), 1)) - get<0>(*prev((*i).jobs[cur_day].end(), 1));

		cout << "Style[Labeled[" << d_last << ", Style[\"Job " << get<2>(*prev((*i).jobs[cur_day].end(), 1)) << "\", Black, 12], Center], Lighter[ColorData[\"Rainbow\"," << get<2>(*prev((*i).jobs[cur_day].end(), 1))/10.0 << "]]]";

		if(i != prev(trucks.end(),1))
			cout << "},";
		else
			cout << "}";
	}
	cout << "}" << endl;
}

#endif /* IO_HPP_ */
