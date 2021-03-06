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

void print_docks(vector< dock >& docks){

	cout << endl << endl << "Mathematica Output: " << endl;

	cout << "{";

	for(auto i = docks.begin(); i != docks.end(); i++){

		cout << "{";

		if( get<0>(*(*i).jobs.begin()) != 0){
			cout << "0," << get<0>(*(*i).jobs.begin()) << ",";
		}

		for(auto it = (*i).jobs.begin(); it != prev((*i).jobs.end(), 1); it++){


			cout << get<1>(*it) - get<0>(*it) << "," << get<0>( *next(it, 1) ) - get<1>(*it) << ",";

//			if(it != prev((*i).jobs.end(), 2)){
//
//			}
//			else{
//				cout << get<1>(*it) - get<0>(*it) << "," << get<0>( *next(it, 1) ) - get<1>(*it);
//			}
		}

		cout << get<1>(*prev((*i).jobs.end(), 1)) - get<0>(*prev((*i).jobs.end(), 1));

		if(i != prev(docks.end(),1))
			cout << "},";
		else
			cout << "}";
	}
	cout << "}" << endl;
}

#endif /* IO_HPP_ */
