#include <stdio.h>
#include <algorithm>
#include <iterator>
#include <unistd.h>
#include <errno.h>
#include <string>
#include <vector>
#include <Eigen/Dense>
#include <iostream>
#include <math.h>
#include <vector>

#include "fastslam2_sim.h"
#include "particle.h"
#include "add_control_noise.h"
#include "predict.h"
#include "observe_heading.h"
#include "get_observations.h"
#include "add_observation_noise.h"
#include "TransformToGlobal.h"
#include "line_plot_conversion.h"
#include "data_associate_known.h"
#include "sample_proposal.h"
#include "feature_update.h"
#include "resample_particles.h"
#include "add_feature.h"
#include "stratified_resample.h"

using namespace Eigen;
using namespace std;
using namespace config;

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
	MatrixXf lm;
	MatrixXf wp;

	read_input_file("example_webmap.mat", &lm, &wp);

        MatrixXf z(2,2);
        z<< 25.7745,25.2762,-1.4734,0.1384;
        vector<int> idz;
        idz.push_back(0);
        idz.push_back(21);

        VectorXf da_table(35);
        for (int i=0; i<35; i++) {
            da_table[i] = -1;
        } 
        da_table[0] = 0;
        da_table[21] = 1;
       
        int Nf = 0; 
       
        MatrixXf zf;//(z.rows(), idz.size());
        vector<int> idf;
        MatrixXf zn;//(z.rows(), idz.size());
        data_associate_known(z,idz,da_table,Nf,zf,idf,zn);

        cout<<"da_table"<<endl;
        cout<<da_table<<endl;
        cout<<"zf"<<endl;
        cout<<zf<<endl;

        MatrixXf test;
         
        cout<<"idf"<<endl;
        for (unsigned j=0; j<idf.size(); j++) {
            cout<<idf[j]<<" ";
        } 
        cout<<endl;

        cout<<"zn"<<endl;
        cout<<zn<<endl;
#if 0
        Particle p = Particle();
        p.setW(0.01);
        
        VectorXf xv(3);
        xv <<1.3237,-0.1459,-0.0326;
        p.setXv(xv);

        MatrixXf Pv(3,3);
        Pv.setZero();
        p.setPv(Pv);

        MatrixXf xf(2,2);
        xf<<2.9036,25.7074,-25.7157,3.2072;
        p.setXf(xf);

        vector<MatrixXf> Pf;
        MatrixXf Pf1(2,2);
        Pf1<<0.2009,0.0168,0.0168,0.0115;
        MatrixXf Pf2(2,2);
        Pf2<<0.0130,-0.0235,-0.0235,0.1916;
        Pf.push_back(Pf1);
        Pf.push_back(Pf2);
        p.setPf(Pf);       
 
        MatrixXf z(2,2);
        z<<25.6306,24.6263,-1.4785,0.1662;

        vector<int>idf;
        idf.push_back(0);
        idf.push_back(1);

        MatrixXf R(2,2);
        R<<0.01,0,0,0.0003;        
        feature_update(p,z,idf,R);

        cout<<"xv"<<endl;
        cout<<p.xv()<<endl;
        cout<<"Pv"<<endl;
        cout<<p.Pv()<<endl;
        cout<<"xf"<<endl;
        cout<<p.xf()<<endl;
        cout<<"Pf"<<endl;
        cout<<p.Pf()[0]<<endl;    
        cout<<p.Pf()[1]<<endl;    
#endif
}
