#include <stdio.h>
#include <algorithm>
#include <iterator>
#include <unistd.h>
#include <errno.h>
#include <string>
#include <vector>
#include <Eigen/Dense>

#include "fastslam1_sim.h"
#include "core/particle.h"

using namespace Eigen;
using namespace std;

void read_input_file(const string s, MatrixXf *lm, MatrixXf *wp) 
{
	using std::ifstream;
	using std::istringstream;
	
	if(access(s.c_str(),R_OK) == -1) {
		std::cerr << "Unable to read input file" << s << std::endl;
		exit(EXIT_FAILURE);
	}
	ifstream in(s.c_str());

	int lineno = 0;
	int lm_rows =0;
	int lm_cols =0;
	int wp_rows =0;
	int wp_cols =0;

	while(in) {
		lineno++;
		string str;
		getline(in,str);
		istringstream line(str);
	
		vector<string> tokens;
		copy(istream_iterator<string>(line), 
			 istream_iterator<string>(), 
			 back_inserter<vector<string> > (tokens));
		
		if(tokens.size() ==0) {
			continue;
		}	
		else if (tokens[0][0] =='#') {
			continue;
		}	
		else if (tokens[0] == "lm") {
			if(tokens.size() != 3) {
				std::cerr<<"Wrong args for lm!"<<std::endl;
				std::cerr<<"Error occuredon line"<<lineno<<std::endl;
				std::cerr<<"line:"<<str<<std::endl;
				exit(EXIT_FAILURE);
			}		
			lm_rows = strtof(tokens[1].c_str(),NULL);	
			lm_cols = strtof(tokens[2].c_str(),NULL);
		
			lm->resize(lm_rows,lm_cols);	
			for (int c =0; c<lm_cols; c++) {
				lineno++;
				if (!in) {
					std::cerr<<"EOF after reading" << std::endl;
					exit(EXIT_FAILURE);
				}	
				getline(in,str);
				istringstream line(str);
				vector<string> tokens;
				copy(istream_iterator<string>(line), 
			 		istream_iterator<string>(), 
			 		back_inserter<vector<string> > (tokens));
				if(tokens.size() < lm_rows) {
					std::cerr<<"invalid line for lm coordinate!"<<std::endl;
					std::cerr<<"Error occured on line "<<lineno<<std::endl;
					std::cerr<<"line: "<<str<<std::endl;
					exit(EXIT_FAILURE);
				}
				
				for (unsigned r=0; r< lm_rows; r++) {
					(*lm)(r,c) = strtof(tokens[r].c_str(),NULL);				
				}				
			}
		}
		else if (tokens[0] == "wp") {
			if(tokens.size() != 3) {
				std::cerr<<"Wrong args for wp!"<<std::endl;
				std::cerr<<"Error occured on line"<<lineno<<std::endl;
				std::cerr<<"line:"<<str<<std::endl;
				exit(EXIT_FAILURE);
			}
			wp_rows = strtof(tokens[1].c_str(),NULL);	
			wp_cols = strtof(tokens[2].c_str(),NULL);
			wp->resize(wp_rows, wp_cols);	
			for (int c =0; c<wp_cols; c++) {
				lineno++;
				if (!in) {
					std::cerr<<"EOF after reading" << std::endl;
					exit(EXIT_FAILURE);
				}	
				getline(in,str);
				istringstream line(str);
				std::vector<string> tokens;
				copy(istream_iterator<string>(line), 
			 		istream_iterator<string>(), 
			 		back_inserter<std::vector<string> > (tokens));
				if(tokens.size() < wp_rows) {
					std::cerr<<"invalid line for wp coordinate!"<<std::endl;
					std::cerr<<"Error occured on line "<<lineno<<std::endl;
					std::cerr<<"line: "<<str<<std::endl;
					exit(EXIT_FAILURE);
				}
									
				for (int r=0; r< lm_rows; r++) {
					(*wp)(r,c) = strtof(tokens[r].c_str(),NULL);				
				}				
			}
		}
		else {
			std::cerr<<"Unkwown command"<<tokens[0] <<std::endl;
			std::cerr<<"Error occured on line"<<lineno<<std::endl;
			std::cerr<<"line: "<<str<<std::endl;
			exit(EXIT_FAILURE);
		}	
	}
}


int main (int argc, char *argv[])
{
	MatrixXf lm; //landmark positions
	MatrixXf wp; //way points

	read_input_file("example_webmap.mat", &lm, &wp);
	vector<Particle> data = fastslam1_sim(lm,wp);
}
