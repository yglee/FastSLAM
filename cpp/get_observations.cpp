#include "get_observations.h"
#include <iostream>
#include <math.h>

MatrixXf get_observations(VectorXf x, MatrixXf lm, vector<int> &idf, float rmax)
{
	get_visible_landmarks(x,lm,idf,rmax);
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

	for (int j=0; j<lm.rows(); j++){
		for(int k=0; k< ii.size(); k++){
			lm_new(j,k) = lm(j,ii[k]);
		}
	}
	lm = MatrixXf(lm_new); 
	idf = vector<int>(ii); 			
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
		z(0,i) = pow(dx(0,i),2) + pow(dy(0,i),2);		
		z(1,i) = atan2(dy(0,i),dx(0,i)) - phi;	
	}

	return z; 
}

vector<int> find2(MatrixXf dx, MatrixXf dy, float phi, float rmax)
{
	vector<int> index;
	//incremental tests for bounding semi-circle
	for (int i =0; i<dx.cols(); i++) {
		if ((abs(dx(0,i)) < rmax) && (abs(dy(0,i)) < rmax)
		&& ((dx(0,i)* cos(phi) + dy(0,i)* sin(phi)) > 0)
		&& ((pow(dx(0,i),2) + pow(dy(0,i),2)) < pow(rmax,2))) {
			index.push_back(i);			
		}
	}	
	return index;				
}
