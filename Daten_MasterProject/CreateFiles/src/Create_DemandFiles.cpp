//============================================================================
// Name        : Create_DemandFiles.cpp
// Author      : lindorfer
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include <bits/stdc++.h>
using namespace std;

void read_csv_demand_template(string& file, vector< tuple<string,string,string,string, string> >& demands, long& numb_days){
	ifstream in;
	in.open(file);
	string line;

	long first_day = 2;
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
			demands.push_back(make_tuple(vec_line.at(0), vec_line.at(1), vec_line.at(2), vec_line.at(3), vec_line.at(4)));
			//----- Month, Day, Store#, Demand-----
		}
	}

//	auto sort_tuple = [] (tuple<string,string,string,tuple<string,string>> a , tuple<string,string,string,tuple<string,string>> b) { return tie(a) < tie(b); };
//	sort(demands.begin(),demands.end(),sort_tuple);
	cout << demands.size() << endl;
	for(auto d : demands) {
		cout << get<0>(d) << " " << get<1>(d) << " " << get<2>(d) << " " <<  get<3>(d) << " " <<  get<4>(d) << endl;
	}

	numb_days = i;
}

void create_csv_demand_multDays(string& file, vector< tuple<string,string,string,string, string> >& demands, long& numb_days, long max_demand){

	ofstream out_file;
	out_file.open(file);

	out_file << "\"Month\";\"Day\";\"SiteName\";\"Temperature\";\"DemandPallets\"" << endl;

	for(int day = 0; day < numb_days; day++){
		for(auto& d : demands){

			out_file << get<0>(d) <<";"<< stoi(get<1>(d))+day <<";" << get<2>(d) <<";"<< get<3>(d) <<";"<< rand() % max_demand << endl;
		}
	}


	out_file.close();
}


int main() {

	//-----Read the Input template-----
	vector< tuple<string,string,string,string, string> > demands;
	long numb_days;
	string file_name = "Demand1_48Stores_template.csv";

	read_csv_demand_template(file_name, demands, numb_days);

	//-----Create an Output with numb_days days and random demand-----
	string out_file = "testdemand.csv";
	numb_days = 5;
	long max_demand = 20;

	create_csv_demand_multDays(out_file, demands, numb_days, max_demand);

	cout << "" << endl; // prints 
	return 0;
}
