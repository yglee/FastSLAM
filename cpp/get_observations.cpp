#include "get_observations.h"
#include <iostream>
#include <math.h>

MatrixXf get_observations(VectorXf x, MatrixXf lm, vector<int> &idf, float rmax)
{
	get_visible_landmarks(x,lm,idf,rmax);
	#if 0
    cout<<"lm: "<<lm<<endl;
	cout<<"lm should be [2.9922,25.7336;-25.7009, 3.27100]"<<endl;
	cout<<"x: "<<x<<endl;
	cout<<"x should be [0.6741;-0.0309;-0.0074]"<<endl;
	cout<<"compute_range_bearing returns: "<<endl;
	MatrixXf temp = compute_range_bearing(x,lm);
	cout<<temp<<endl;
	cout<<"compute range beearing should be [25.7745,25.2762;-1.4734,0.1384]"<<endl;
	#endif
	return compute_range_bearing(x,lm);	
}

void get_visible_landmarks(VectorXf x, MatrixXf &lm, vector<int> &idf, float rmax)
{
	//select set of landmarks that are visible within vehicle's 
	//semi-circular field of view

	MatrixXf dx(1,lm.cols());
	MatrixXf dy(1,lm.cols());
	for (int c=0; c<lm.cols(); c++) {
		dx(0,c) = lm(0,c) - x(0);
		dy(0,c) = lm(1,c) - x(1);
	}
	
	float phi = x(2);
	vector<int> ii = find2(dx,dy,phi,rmax); 

	//lm(:[2 3]) return matrix of column 2 and colum 3 of lm
	
	MatrixXf lm_new (lm.rows(), ii.size());
	vector<int>::iterator iter;

	unsigned j,k;
	for (j=0; j<lm.rows(); j++){
		for(k=0; k< ii.size(); k++){
			lm_new(j,k) = lm(j,ii[k]);
		}
	}
	lm = MatrixXf(lm_new); 
	//idf = vector<int>(ii); 			
        vector<int>idf_backup(idf);
        idf.clear();
        for(int i=0; i<ii.size(); i++) {
            idf.push_back(idf_backup[ii[i]]);
        }
}

MatrixXf compute_range_bearing(VectorXf x, MatrixXf lm) 
{
	MatrixXf dx(1,lm.cols());
	MatrixXf dy(1,lm.cols());
	for (int c=0; c<lm.cols(); c++) {
		dx(0,c) = lm(0,c) - x(0);
		dy(0,c) = lm(1,c) - x(1);
	}	
	
	float phi = x(2);
	MatrixXf z(2,lm.cols());
	for (int i =0; i<lm.cols(); i++) {
		z(0,i) = sqrt(pow(dx(0,i),2) + pow(dy(0,i),2));		
		z(1,i) = atan2(dy(0,i),dx(0,i)) - phi;	
	}

	return z; 
}

vector<int> find2(MatrixXf dx, MatrixXf dy, float phi, float rmax)
{
	vector<int> index;
	//incremental tests for bounding semi-circle
	for (int j=0; j<dx.rows(); j++) {
            for (int i =0; i<dx.cols(); i++) {
		if ((abs(dx(j,i)) < rmax) && (abs(dy(j,i)) < rmax)
		&& ((dx(j,i)* cos(phi) + dy(j,i)* sin(phi)) > 0)
		&& ((pow(dx(j,i),2) + pow(dy(j,i),2)) < pow(rmax,2))) {
			index.push_back(i);			
		}
	    }	
        }
	return index;				
}
