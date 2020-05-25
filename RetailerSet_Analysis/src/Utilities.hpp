//============================================================================
// Name        : Utilities.hpp
// Author      : lindorfer
// Version     :
// Copyright   : Your copyright notice
// Description : Simplistic C++ Utilities to make life easier
//============================================================================

#include <bits/stdc++.h>
using namespace std;

//-----Range-----
//-----The data-type of the step is used for the data-type of the vector-----
template <typename start_type, typename stop_type, typename step_type>
vector<step_type> range(start_type start, stop_type stop, step_type step){
  if (step == step_type(0)){
    throw std::invalid_argument("step for range must be non-zero");
  }

  if(typeid(step) != typeid(start)){

  }

  vector<step_type> result;
  step_type i = start;

  while ((step > 0) ? (i < stop) : (i > stop)){
    result.push_back(i);
    i += step;
  }

  return result;
}

template <typename T>
vector<T> range(T start, T stop){
  return range(start, stop, T(1));
}

template <typename T>
vector<T> range(T stop){
  return range(T(0), stop, T(1));
}

//-----std::vector cout-----
template<typename T>
ostream& operator<< (ostream& out, const vector<T>& v) {
    out << "{";
    size_t last = v.size() - 1;
    for(size_t i = 0; i < v.size(); ++i) {
        out << v[i];
        if (i != last)
            out << ", ";
    }
    out << "}";
    return out;
}

//-----std::tuple cout-----
template<std::size_t I = 0, typename... Tp>
inline typename std::enable_if<I == sizeof...(Tp), string>::type
stringval(const std::tuple<Tp...> & t)
{
  std::stringstream buffer;
  buffer << "}";
  return buffer.str();
}

template<std::size_t I = 0, typename... Tp>
inline typename std::enable_if<I < sizeof...(Tp), string>::type
stringval(const std::tuple<Tp...> & t)
{
  std::stringstream buffer;
  size_t len = sizeof...(Tp);
  if(I==0)
      buffer << "{";
  buffer << std::get<I>(t);
  if(I < len - 1)
    buffer << ", ";
  buffer << stringval<I + 1, Tp...>(t);
  return buffer.str();
}

template<typename... Tp> ostream& operator <<(ostream& out, const std::tuple<Tp...> & t)
{
  out << stringval(t);
  return out;
}

//-----Unordered_Map-----
//-----Output unordered_map to ofstream-----
template<typename K, typename T>
ostream& operator<< (ostream& out, unordered_map<K, T>& umap) {
	out << "{";

	for(auto it = umap.begin(); it != umap.end(); it++){

		if(next(it) == umap.end()){
			out << "{" << it -> first << ", " << it -> second << "}";
		}
		else{
			out << "{" << it -> first << ", " << it -> second << "}, ";
		}
	}

    out << "}";
    return out;
}

