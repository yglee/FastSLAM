#include <boost/numeric/ublas/vector.hpp>  
#include <boost/numeric/ublas/matrix.hpp>

using namespace boost::numeric::ublas;

void printMat(matrix A)
{
	for (int r = 0; r< A.size1(); A++) {
		for (int c = 0; c< A.size2(); A++) {
			printf("%f ", A(r,c));
		}
		printf("\n");
	}
	printf("\n");
}
