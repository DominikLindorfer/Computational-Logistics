//============================================================================
// Name        : Simplified_B.cpp
// Author      : lindorfer
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include <bits/stdc++.h>
using namespace std;

void read_csv(string& file, vector<vector <string> >& data){

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

}

int main() {

	string TravTime = "Data/TravelTim1_short.csv";
	vector<vector <string> > data;
	read_csv(TravTime, data);

	for(auto i : data){
		for(auto j : i){
			cout << j << " ";
		}
		cout << endl;
	}


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
