#include "add_observation_noise.h"

//http://moby.ihme.washington.edu/bradbell/mat2cpp/randn.cpp.xml

MatrixXf nRandMat::randn(int m, int n) 
{	
	// use formula 30.3 of Statistical Distributions (3rd ed)
	// Merran Evans, Nicholas Hastings, and Brian Peacock
	int urows = m * n + 1;
	MatrixXf u(urows, 1);

	//u is a random matrix
	for (int r=0; r<urows; r++) {
		for (int c=0; c<1; c++) {
			u(r,c) = std::rand();
		}
	}
	
	MatrixXf x(m,n);

	int i, j, k;
	float pi = 4. * std::atan(1.f);
	float square, amp, angle;
	k = 0;
	for(i = 0; i < m; i++)
	{	for(j = 0; j < n; j++)
		{	if( k % 2 == 0 )
			{	square = - 2. * std::log( u(k, 0) );
				if( square < 0. )
					square = 0.;
				amp = sqrt(square);
				angle = 2. * pi * u(k+1, 0);
				x(i, j) = amp * std::sin( angle );
			}
			else	
				x(i, j) = amp * std::cos( angle );
			k++;
		}
	}
	return x;
}

MatrixXf nRandMat::rand(int m, int n) 
{	
	MatrixXf x(m,n);	
	int i, j;
	float rand_max = float(RAND_MAX);

	for(i = 0; i < m; i++) {	
		for(j = 0; j < n; j++)
			x(i, j) = float(std::rand()) / rand_max;
	} 
	return x;
}

//add random measurement noise. We assume R is diagnoal matrix
void add_observation_noise(vector<VectorXf> &z, MatrixXf R, int addnoise)
{
	if (addnoise == 1){
		int len = z.size();	
		if (len > 0) {
			MatrixXf randM1 = nRandMat::randn(1,len);
			MatrixXf randM2 = nRandMat::randn(1,len);

			for (int c=0; c<len; c++) {
				z[c][0] = z[c][0] + randM1(0,c)*sqrt(R(0,0));
				z[c][1] = z[c][1] + randM2(0,c)*sqrt(R(1,1));
			}
		}
	}	
}


