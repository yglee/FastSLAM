#include "data_associate_known.h"
#include <iostream>

void data_associate_known(MatrixXf z, vector<int> idz, VectorXf &table, int Nf, \
        MatrixXf &zf, vector<int> &idf, MatrixXf &zn) 
{
    zf.setZero();
    zn.setZero();
    idf.clear();

    unsigned i,ii,r;
    vector<int> idn;

    for (i =0; i< idz.size(); i++){
        ii = idz[i];
        if (table(ii) ==-1) { //new feature
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

    #if 0
    //test code
    vector<int>::iterator idniter;
    cout<<"idn: "<<endl;
    for (idniter=idn.begin(); idniter!=idn.end(); idniter++) {
        cout<<(*idniter)<<" ";
    }
    cout<<endl;
    cout<<"idn should be 0 and 21"<<endl;

    vector<int>::iterator idfiter;
    cout<<"idf: "<<endl;
    for (idfiter=idf.begin(); idfiter!=idf.end(); idfiter++) {
        cout<<(*idfiter)<<" ";
    }
    cout<<endl;
    cout<<"idf should be empty"<<endl;

    cout<<"Nf is "<<Nf<<endl;
    cout<<"Nf should be 0"<<endl;
    #endif

    //TODO: look into this later (table(ii) == 0) but counter starts from index 0...
    //add new feature IDs to lookup table
    vector<int> counter;
    for (unsigned int c=0; c<zn.cols(); c++) {
        counter.push_back(c+Nf);
    }

    for (unsigned int d=0; d<idn.size(); d++) {
        table(idn[d]) = counter[d]; 			
    }
    
    #if 0
    cout<<"table"<<endl;
    cout<<table<<endl;	
    #endif
}

