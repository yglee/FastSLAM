#include "sample_proposal.h"
#include <iostream>
#include <Eigen/SVD>
#include <iomanip>
#include "assert.h"

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

    MatrixXf zpi;
    MatrixXf Hvi;
    MatrixXf Hfi;
    MatrixXf Sfi;

    VectorXf vi(z.rows(),1);
#if 0
    cout<<"Pv in sample_proposal"<<endl;
    cout<<Pv<<endl;
    cout<<"should be 1.0e-04 * "<<endl; 
    cout<<"          0.5041 -0.0236 -0.0056"<<endl;
    cout<<"          -0.0236 0.01992 0.0459"<<endl;
    cout<<"          -0.0056 0.0459 0.0106"<<endl;
#endif	

    //process each feature, incrementally refine proposal distribution
    unsigned i,r,c;
    for (i =0; i<idf.size(); i++) {
        vector<int> j;
        j.push_back(idf[i]);

        compute_jacobians(particle,j,R,zpi,&Hv,&Hf,&Sf);
        assert(zpi.cols() == 1);

        Hvi = Hv[i];
        Hfi = Hf[i];
        Sfi = Sf[i].inverse();

#if 0
        cout<<"i in SAMPLE_PROPOSAL"<<endl;
        cout<<i<<endl;
        cout<<"should be 1"<<endl;
        cout<<endl;

        cout<<"vi"<<endl;
        cout<<vi<<endl;
        cout<<endl;
        cout<<"z"<<endl;
        cout<<z<<endl;
        cout<<endl;
        cout<<"zpi"<<endl;
        cout<<zpi<<endl;
#endif
        for (r=0; r<z.rows(); r++) {
            vi[r] = z(r,i) - zpi(r,0); 
        }
#if 0
        cout<<"vi"<<endl;
        cout<<vi<<endl;
        cout<<"should be [0.1355; -0.0333]"<<endl; 
        cout<<endl;
#endif
        vi[1] = pi_to_pi(vi[1]);

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
#if 0
        cout<<"Pv"<<endl;
        cout<<Pv<<endl;
#endif

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
    cout<<"in compute_jacob: particle.w() = "<<particle.w()<<endl;
    cout<<"like = "<<like<<endl;
    cout<<"prior = "<<prior<<endl;
    cout<<"prop = "<<prop<<endl;

    particle.setW(particle.w() * like * prior / prop); 
} 

float likelihood_given_xv(Particle particle, MatrixXf z, vector<int>idf, MatrixXf R) 
{
    float w = 1;
    vector<int> idfi;

    vector<MatrixXf> Hv;
    vector<MatrixXf> Hf;
    vector<MatrixXf> Sf;

    MatrixXf zp;
    MatrixXf Hvi;
    MatrixXf Hfi;
    MatrixXf Sfi;
    VectorXf v(z.rows());    

    unsigned i,k;
    cout<<"idf.size() "<<idf.size()<<endl;
    for (i=0; i<idf.size(); i++){
        idfi.push_back(i);
        compute_jacobians(particle,idfi,R,zp,&Hv,&Hf,&Sf);
        Hvi = Hv[0]; 
        Hfi = Hf[0]; 
        Sfi = Sf[0];
#if 0
        cout<<"i = "<<i<<endl;
        cout<<"z"<<endl;
        cout<<z<<endl;
        cout<<"z should be 25.6475   24.5412"<<endl;
        cout<<"		  -1.4803    0.2031"<<endl;
        cout<<"zp"<<endl;
        cout<<zp<<endl;
        cout<<"zp[0] should be 25.7004"<<endl;
        cout<<"                -1.5063"<<endl;
        cout<<"zp[1] should be 24.6814"<<endl;
        cout<<"                0.1522"<<endl;

#endif
        for (k=0; k<z.rows(); k++) {
            v(k) = z(k,i)-zp(k,0); //TODO: this returns wrong values
        }
        v(1) = pi_to_pi(v(1));

        cout<<"v in likelihood eval"<<endl;
        cout<<v<<endl;
        cout<<"should be 0.1804"<<endl;
        cout<<"          0.0327"<<endl;
        cout<<"SF["<<i<<"]"<<endl;
        cout<<Sf[i]<<endl;
        cout<<"should be 0.0201 -0.0002"<<endl;
        cout<<"         -0.0002  0.0006"<<endl;
        cout<<endl;

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
