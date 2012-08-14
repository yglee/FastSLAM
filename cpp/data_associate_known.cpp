#include "data_associate_known.h"
#include <iostream>

void data_associate_known(MatrixXf z, vector<int> idz, VectorXf &table, int Nf, \
						  MatrixXf &zf, vector<int> &idf, MatrixXf &zn) 
{
	unsigned i,ii,r;
	vector<int> idn;
  
	for (i =0; i< idz.size(); i++){
		ii = idz[i];
		if (table(ii) ==0) {
			for (r=0; r<z.rows();r++) {
				zn(r,i) = z(r,i);
			}
			idn.push_back(ii);				
		}
		else {
			for (r=0; r<z.rows(); r++) {
				zf(r,i) = z(r,i);
			} 
			idf.push_back(table(ii));
		}	
	}

	//add new feature IDs to lookup table
	vector<int> counter;
	for (unsigned int c=0; c<zn.cols(); c++) {
		counter.push_back(c+Nf);
	}	 

	for (unsigned int d=0; d<idn.size(); d++) {
		table(idn[d]) = counter[d]; 			
	}	
}

