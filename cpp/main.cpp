#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <stdlib.h>

#include <boost/numeric/ublas/vector.hpp> 

int main (int argc, char *argv[])
{
	using namespace boost::numeric::ublas;
	float veh[2][3] ={{1,2,3},{4,5,6}};	
	for (int i =0; i < 2; i++){
		for (int j =0; j< 3; j++) {
			std::cout<<veh[i][j]<<" ";
		}
		std::cout<<"\n";
	}
	
	vector<double> v (3);
	for (unsigned i = 0; i < v.size(); ++i) {
		v(i) = 0;	
	} 
}
