#include "get_observations.h"
#include <iostream>
#include <math.h>

vector<VectorXf> get_observations(VectorXf x, MatrixXf lm, vector<int> &idf, float rmax)
{
	get_visible_landmarks(x,lm,idf,rmax);
	return compute_range_bearing(x,lm);	
}

void get_visible_landmarks(VectorXf x, MatrixXf &lm, vector<int> &idf, float rmax)
{
	//select set of landmarks that are visible within vehicle's 
	//semi-circular field of view
	vector<float> dx;
	vector<float> dy;

	for (int c=0; c<lm.cols(); c++) {
		dx.push_back(lm(0,c) - x(0));
		dy.push_back(lm(1,c) - x(1));
	}

	float phi = x(2);

	//distant points are eliminated
	vector<int> ii = find2(dx,dy,phi,rmax); 

	MatrixXf lm_new (lm.rows(), ii.size());
	unsigned j,k;
	for (j=0; j<lm.rows(); j++){
		for(k=0; k< ii.size(); k++){
			lm_new(j,k) = lm(j,ii[k]);
		}
	}
	lm = MatrixXf(lm_new); 

	vector<int> idf_backup(idf);
	idf.clear();
	for(int i=0; i<ii.size(); i++) {
		idf.push_back(idf_backup[ii[i]]);
	}
}

vector<VectorXf> compute_range_bearing(VectorXf x, MatrixXf lm) 
{
	vector<float> dx; 
	vector<float> dy;

	for (int c=0; c<lm.cols(); c++) {
		dx.push_back(lm(0,c) - x(0));
		dy.push_back(lm(1,c) - x(1));
	}	

	assert(dx.size() == lm.cols());
	assert(dy.size() == lm.cols());

	float phi = x(2);
	vector<VectorXf> z;

	for (int i =0; i<lm.cols(); i++) {
		VectorXf zvec(2);
		zvec<< sqrt(pow(dx[i],2) + pow(dy[i],2)), atan2(dy[i],dx[i]) - phi;	
		z.push_back(zvec);
	}

	return z; 
}

vector<int> find2(vector<float> dx, vector<float> dy, float phi, float rmax)
{
	vector<int> index;
	//incremental tests for bounding semi-circle
	for (int j=0; j<dx.size(); j++) {
		if ((abs(dx[j]) < rmax) && (abs(dy[j]) < rmax)
				&& ((dx[j]* cos(phi) + dy[j]* sin(phi)) > 0.0)
				&& (((float)pow(dx[j],2) + (float)pow(dy[j],2)) < (float)pow(rmax,2))){
			index.push_back(j);			
		}
	}
	return index;				
}
