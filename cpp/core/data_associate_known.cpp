#include "data_associate_known.h"
#include <iostream>

void data_associate_known(MatrixXf z, vector<int> idz, VectorXf &table, int Nf, \
        MatrixXf &zf, vector<int> &idf, MatrixXf &zn) 
{
    idf.clear();
    vector<int> idn;

    unsigned i,ii,r;
    
    //this extra loopis required to set the dimension of zn and zf
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
    for (i =0; i< idz.size(); i++){
        ii = idz[i];
        if (table(ii) ==-1) { //new feature
            for (r=0; r<z.rows();r++) {
                zn(r,znc) = z(r,i);
            }
            znc++;
            idn.push_back(ii);				
        }
        else {
            for (r=0; r<z.rows(); r++) {
                zf(r,zfc) = z(r,i);
            } 
            zfc++;
            idf.push_back(table(ii));
        }	
    }

    for (int i=0; i<idn.size(); i++) {
        table(idn[i]) = Nf+i;  
    }
}

