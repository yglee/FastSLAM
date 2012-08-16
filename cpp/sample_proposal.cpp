#include "sample_proposal.h"
#include <iostream>
#include <Eigen/SVD>
#include <iomanip>

//compute proposal distribution, then sample from it, and compute new particle weight
void sample_proposal(Particle &particle, MatrixXf z, vector<int> idf, MatrixXf R)
{
    VectorXf xv(particle.xv());
    MatrixXf Pv(particle.Pv());

    VectorXf xv0(xv);
    MatrixXf Pv0(Pv);	

    vector<MatrixXf> Hv;
    vector<MatrixXf> Hf;
    vector<MatrixXf> Sf;

    MatrixXf zpi(2,1);
    MatrixXf Hvi;
    MatrixXf Hfi;
    MatrixXf Sfi;

    MatrixXf vi(z.rows(),1);
#if 0
    cout<<"Pv in sample_proposal"<<endl;
    cout<<Pv<<endl;
    cout<<"should be 1.0e-04 * "<<endl; 
    cout<<"          0.5041 -0.0236 -0.0056"<<endl;
    cout<<"          -0.0236 0.01992 0.0459"<<endl;
    cout<<"          -0.0056 0.0459 0.0106"<<endl;
#endif	

    //process each feature, incrementally refine proposal distribution
    unsigned i;
    for (i =0; i<idf.size(); i++) {
        vector<int> j;
        j.push_back(idf[i]);

        compute_jacobians(particle,j,R,zpi,&Hv,&Hf,&Sf);

        Hvi = Hv[i];
        Hfi = Hf[i];
        Sfi = Sf[i].inverse();

        vi<<1,2;
        //vi = z.conservativeResize(z.rows(),1) - zpi;
        vi(1,0) = pi_to_pi(vi(1,0)); 		

        //TODO: fix: Pv.inverse is Nan - use a pseudo inverse..
#if 0
        cout<<"Hfi"<<endl;
        cout<<Hfi<<endl;
        cout<<"Sfi"<<endl;
        cout<<Sfi<<endl;
        cout<<"Pv"<<endl;
        cout<<Pv<<endl;
        cout<<"xv"<<endl;
        cout<<xv<<endl;
        cout<<"vi"<<endl;
        cout<<vi<<endl;
#endif
        //add a little bias so I won't get NaNs when I do an inverse
        cout<<"Pv"<<endl;
        cout<<Pv<<endl;

        int r,c;
        for (r=0; r<Pv.rows(); r++) {
            for (c=0; c<Pv.cols(); c++) {
                if (r==c) {
                    Pv(r,c) += 1.0e-11;
                } 
            }   
        }

        #if 0
        cout<<"Pv.inverse()"<<endl;
        cout<<Pv.inverse()<<endl;
        #endif        

        //proposal covariance
        Pv = Hvi.transpose() * Sfi * Hvi + Pv.inverse();
        Pv = Pv.inverse();
    
        #if 0
        cout<<"xv"<<endl;
        cout<<xv<<endl;
        cout<<"Pv"<<endl;
        cout<<Pv<<endl;
        cout<<"Hvi"<<endl;
        cout<<Hvi<<endl;
        cout<<"Sfi"<<endl;
        cout<<Sfi<<endl;
        cout<<"vi"<<endl;
        cout<<vi<<endl;
        #endif
        //proposal mean
        xv = xv + Pv * Hvi.transpose() * Sfi *vi;

        particle.setXv(xv);
        particle.setPv(Pv); 
    }

    //sample from proposal distribution
    VectorXf xvs = multivariate_gauss(xv,Pv,1); 
    particle.setXv(xvs);
    particle.setPv(VectorXf(3));

    //compute sample weight: w = w* p(z|xk) p(xk|xk-1) / proposal
    float like = likelihood_given_xv(particle, z, idf, R);
    float prior = gauss_evaluate(delta_xv(xv0,xvs), Pv0,0); 
    float prop = gauss_evaluate(delta_xv(xv,xvs),Pv,0);
    particle.setW(particle.w() * like * prior / prop); 
} 

float likelihood_given_xv(Particle &particle, MatrixXf z, vector<int>idf, MatrixXf R) 
{
    float w = 1;
    vector<int> temp;

    vector<MatrixXf> Hv;
    vector<MatrixXf> Hf;
    vector<MatrixXf> Sf;

    MatrixXf zp;
    MatrixXf Hvi;
    MatrixXf Hfi;
    MatrixXf Sfi;
    MatrixXf zi(z.rows(),1);
    MatrixXf v;    

    unsigned i,k;
    for (i=0; i<idf.size(); i++){
        temp.push_back(i);
        compute_jacobians(particle,temp,R,zp,&Hv,&Hf,&Sf);
        Hvi = Hv[0]; 
        Hfi = Hf[0]; 
        Sfi = Sf[0];
        for (k=0; k<z.rows(); k++) {
            zi(k,i);
        }
        v = zi-zp;
        v(1,0) = pi_to_pi(v(1,0));
        w = w*gauss_evaluate(v,Sf[i],0);
    } 
    return w;
}

VectorXf delta_xv(VectorXf xv1, VectorXf xv2)
{
    //compute innovation between two xv estimates, normalising the heading component
    VectorXf dx = xv1-xv2; 
    dx(2) = pi_to_pi(dx(2));
    return dx;
}

#if 0
template<typename _Matrix_Type_>
bool pseudoInverse(const _Matrix_Type_ &a, _Matrix_Type_ &result, double
        epsilon = numeric_limits<typename _Matrix_Type_::Scalar>::epsilon())
{
    if(a.rows()<a.cols())
        return false;

    JacobiSVD< _Matrix_Type_ > svd = a.jacobiSvd();

    typename _Matrix_Type_::Scalar tolerance = epsilon * max(a.cols(),
            a.rows()) * svd.singularValues().array().abs().maxCoeff();

    result = svd.matrixV() * _Matrix_Type_(_Matrix_Type_(
                (svd.singularValues().array().abs() > tolerance).select(svd.singularValues().
                    array().inverse(), 0) ).diagonal()) * svd.matrixU().adjoint();
}
#endif
