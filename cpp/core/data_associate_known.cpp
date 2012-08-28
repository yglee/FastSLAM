#include "data_associate_known.h"
#include <iostream>

//void data_associate_known(MatrixXf z, vector<int> idz, VectorXf &table, int Nf, \
        vector<VectorXf> &zf, vector<int> &idf, vector<VectorXf> &zn) 

//z is range and bearing of visible landmarks
void data_associate_known(vector<VectorXf> z, vector<int> idz, VectorXf &table, int Nf, \
        vector<VectorXf> &zf, vector<int> &idf, vector<VectorXf> &zn) 
{
    idf.clear();
    vector<int> idn;

    unsigned i,ii,r;
   
	#if 0 
    //this extra loop is required to set the dimension of zn and zf
    int zncols = 0;
    int zfcols = 0;
    for (i =0; i< idz.size(); i++){
        ii = idz[i];
        if (table(ii) ==-1) { //new feature
            zncols++;
        }
        else {
            zfcols++;
        }
    }
    
	//resize
    zn.resize(z.rows(),zncols);
    zn.setZero();
    zf.resize(z.rows(),zfcols);
    zf.setZero();

    int znc,zfc;
    znc =0;
    zfc =0;
    #endif
    
	for (i =0; i< idz.size(); i++){
        ii = idz[i];
		VectorXf z_i;
        if (table(ii) ==-1) { //new feature
			//for (r=0; r<z.size(); r++) {
				z_i = z[i];
				//z_i(r) = z(r,i);
            //}
			zn.push_back(z_i);
            //znc++;
            idn.push_back(ii);				
        }
        else {
            //for (r=0; r<z.rows(); r++) {
               	z_i = z[i];
				// zf(r,zfc) = z(r,i);
            //} 
            //zfc++;
			zf.push_back(z_i);
            idf.push_back(table(ii));
        }	
    }

	assert(idn.size() == zn.size());
    for (int i=0; i<idn.size(); i++) {
        table(idn[i]) = Nf+i;  
    }
}

