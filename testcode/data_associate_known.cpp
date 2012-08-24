#include "data_associate_known.h"
#include <iostream>

void data_associate_known(MatrixXf z, vector<int> ftag_visible, VectorXf &da_table, int Nf, \
        MatrixXf &zf, vector<int> &idf, MatrixXf &zn) 
{
    cout<<"z (r,c) = "<<z.rows()<<" "<<z.cols()<<endl;
    zf.resize(z.rows(), ftag_visible.size());
    zf.setZero();
    cout<<"zf (r,c) = "<<zf.rows()<<" "<<zf.cols()<<endl;
    zn.resize(z.rows(), ftag_visible.size());
    cout<<"zn (r,c) = "<<zn.rows()<<" "<<zn.cols()<<endl;
    zn.setZero();
    idf.clear();
    vector<int> idn;

    unsigned i,ii,r;
    
    for (i =0; i< ftag_visible.size(); i++){
        ii = ftag_visible[i];
        if (da_table(ii) ==-1) { //new feature
            for (r=0; r<z.rows();r++) {
                zn(r,i) = z(r,i);
            }
            idn.push_back(ii);				
        }
        else {
            for (r=0; r<z.rows(); r++) {
                zf(r,i) = z(r,i);
            } 
            idf.push_back(da_table(ii));
        }	
    }

    //TODO: not sure if these are necessary...
    if (zf == MatrixXf::Zero(zf.rows(),zf.cols())) {
        zf = MatrixXf();
    }
    
    if (zn == MatrixXf::Zero(zn.rows(),zn.cols())) {
        zn = MatrixXf();
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

    //TODO: look into this later (da_table(ii) == 0) but counter starts from index 0...
    //add new feature IDs to lookup da_table
    vector<int> counter;
    for (unsigned int c=0; c<zn.cols(); c++) {
        counter.push_back(c+Nf);
    }

    for (unsigned int d=0; d<idn.size(); d++) {
        da_table(idn[d]) = counter[d]; 			
    }
    
    #if 0
    cout<<"da_table"<<endl;
    cout<<da_table<<endl;	
    #endif
}

