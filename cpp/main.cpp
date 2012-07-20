#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <unistd.h>
#include <errno.h>

#include <boost/numeric/ublas/vector.hpp> 
#include <boost/numeric/ublas/matrix.hpp> 

namespace ublas = boost::numeric::ublas;

void read_input_file(const string s) 
{
	using std::ifstream;
	using std::istringstream;
	if(access(s.c_str(),R_OK) == -1) {
		cerr << "Unable to read input file" << s << endl;
		exit(EXIT_FAILURE);
	}
	ifstream in(s.c_str());

	int lm_rows =0;
	int lm_cols =0;
	int wp_rows =0;
	int wp_cols =0;

	matrix<float> lm;
	matrix<float> wp;

	while(in) {
		lineno++;
		string str;
		getline(in,str);
		istringstream line(str);
	
		vector<string> tokens;
		copy(istream_iterator<string>(line), 
			 istream_iterator<string>(), 
			 back_inserter<vector<string>> (tokens));
		
		if(tokens.size() ==0) {
			continue;
		}	
		else if (tokens[0][0] =='#') {
			continue;
		}	
		else if (tokens[0] == "lm") {
			if(tokens.size() != 3) {
				cerr<<"Wrong args for lm!"<<endl;
				cerr<<"Error occuredon line"<<lineno<<endl;
				cerr<<"line:"<<str<<endl;
				exit(EXIT_FAILURE);
			}		
			lm_rows = strtof(tokens[1].c_str(),NULL);	
			lm_cols = strtof(tokens[2].c_str(),NULL);
			
			for (int c =0; c<lm_cols; c++) {
				lineno++;
				if (!in) {
					cerr<<"EOF after reading" << endl;
					end(EXIT_FAILURE);
				}	
				getline(in,str);
				istringstream line(str);
				vector<string> tokens;
				copy(istream_iterator<string>(line), 
			 		istream_iterator<string>(), 
			 		back_inserter<vector<string>> (tokens));
				if(tokens.size() < lm_rows) {
					cerr<<"invalid line for lm coordinate!"<<endl;
					cerr<<"Error occured on line "<<lineno<<endl;
					cerr<<"line: "<<str<<endl;
					exit(EXIT_FAILURE);
				}
				
				for (int r=0; r< lm_rows; r++) {
					lm.insert_element(r,c,tokens[r].c_str(),NULL);				
				}				
			}
			//TODO: double check this	
			printf("lm rows: %f cols: %f\n", lm_rows, lm_cols);
		}
		else if (tokens[0] == "wp") {
			if(tokens.size() != 3) {
				cerr<<"Wrong args for wp!"<<endl;
				cerr<<"Error occured on line"<<lineno<<endl;
				cerr<<"line:"<<str<<endl;
				exit(EXIT_FAILURE);
			}
			wp_rows = strtof(tokens[1].c_str(),NULL);	
			wp_cols = strtof(tokens[2].c_str(),NULL);
			
			for (int c =0; c<wp_cols; c++) {
				lineno++;
				if (!in) {
					cerr<<"EOF after reading" << endl;
					end(EXIT_FAILURE);
				}	
				getline(in,str);
				istringstream line(str);
				vector<string> tokens;
				copy(istream_iterator<string>(line), 
			 		istream_iterator<string>(), 
			 		back_inserter<vector<string>> (tokens));
				if(tokens.size() < wp_rows) {
					cerr<<"invalid line for wp coordinate!"<<endl;
					cerr<<"Error occured on line "<<lineno<<endl;
					cerr<<"line: "<<str<<endl;
					exit(EXIT_FAILURE);
				}
									
				for (int r=0; r< lm_rows; r++) {
					wp.insert_element(r,c,tokens[r].c_str(),NULL);				
				}				
			}
			printf("wp rows: %f cols: %f\n", wp_rows, wp_cols);	
		}
		else {
			cerr<<"Unkwown command"<<tokens[0] <<endl;
			cerr<<"Error occured on line"<<lineo<<endl;
			cerr<<"line: "<<str<<endl;
			exit(EXIT_FAILURE);
		}	
	}
}


void fastslam2_sim(matrix<float> lm, matrix<float> wp) 

int main (int argc, char *argv[])
{
	ublas::vector<float> v (3);
	v(0) = 1.0;
	v(1) = 2.0;
	v(2) = 3.0;

	ublas::vector<float> u (3);
	u(0) = 6.0;
	u(1) = 7.0;
	u(2) = 8.0;

	ublas::vector<float> k = v-u;

	ublas::matrix<float> a(3,3);
/*	
	for (int i=0; i < 3; i++) {
		for (int j =0; j< 3; j++){
			a.insert_element(i,j,1.0);					
		}
	}
*/
	
	for (int i =0; i<a.size1(); i++) {
		for (int j =0; j<a.size2(); j++) {
			printf("%f ",a(i,j));
		}
		printf("\n");
	}
	printf("\n");

	ublas::matrix<float> lm (2,35);
	ublas::matrix<float> wp (2,17);

	
}
