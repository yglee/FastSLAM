#ifndef DATA_ASSOCIATE_KNOWN_H
#define DATA_ASSOCIATE_KNOWN_H

#include <Eigen/Dense>
#include <vector>

using namespace std;
using namespace Eigen;

void data_associate_known(vector<VectorXf> z, vector<int> idz, VectorXf &table, int Nf, \
						  vector<VectorXf> &zf, vector<int> &idf, vector<VectorXf> &zn); 

//void data_associate_known(MatrixXf z, vector<int> idz, VectorXf &table, int Nf, \
						  vector<VectorXf> &zf, vector<int> &idf, vector<VectorXf> &zn); 

#endif //DATA_ASSOCIATE_KNOWN_H
