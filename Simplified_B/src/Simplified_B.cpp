//============================================================================
// Name        : Simplified_B.cpp
// Author      : lindorfer
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include <bits/stdc++.h>
using namespace std;

void read_csv_demand(string& file,vector< tuple<string,string,string,tuple<string,string>>>& demands){
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
			demands.push_back(make_tuple(vec_line.at(2),vec_line.at(0),vec_line.at(1),make_tuple(vec_line.at(3),vec_line.at(4))));
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



}

void read_csv_distance(string& file, vector<vector <string> >& data, vector< tuple<string,string,string>>& distances	){

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
				distances.push_back(make_tuple(d.at(0),d.at(2),d.at(1)));
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

int main() {


	string TravTime = "Data/TravelTim1_short.csv";
	string Demand = "Data/Demand1.csv";

	vector<vector <string> > data;
	vector< tuple<string,string,string>> distances;
	vector< tuple<string,string,string,tuple<string,string>>> demands;

	read_csv_distance(TravTime, data,distances);
	read_csv_demand(Demand,demands);
	//	vector< tuple<string,string,string>> distances(data.size());

	//Test EGit
	//	std::string input = "abc,def,ghi";
	//	std::istringstream ss(input);
	//	std::string token;
	//
	//	while(std::getline(ss, token, ',')) {
	//	    std::cout << token << '\n';
	//	}
	//
	//
	//	cout << "Hello World!" << endl; // prints
	return 0;
}
