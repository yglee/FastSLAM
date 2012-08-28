#include "line_plot_conversion.h"
#include <iostream>

using namespace std;

MatrixXf line_plot_conversion(MatrixXf lnes)
{
	int len = lnes.cols()*3 -1;			
	MatrixXf p(2,len);

	for (int j=0; j<len; j+=3) {
		int k = floor((j+1)/3); //reverse of len calculation
		p(0,j) = lnes(0,k);
		p(1,j) = lnes(1,k);
		p(0,j+1) = lnes(2,k);
		p(1,j+1) = lnes(3,k);
		if (j+2 <len) {
			p(0,j+2) = NULL;
			p(1,j+2) = NULL;
		}
	}
	return p;
}
