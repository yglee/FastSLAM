#include "line_plot_conversion.h"
#include <iostream>

using namespace std;

MatrixXf line_plot_conversion(MatrixXf lnes)
{
	int len = lnes.cols()*3 -1;			
	MatrixXf p(2,len);

#if 0
	int i;
	p(0,0) = lne(0,0);
	p(1,0) = lne(1,0);
	p(0,1) = lne(2,0);
	p(1,1) = lne(3,0);

	p(0,3) = lne(0,1);
	p(1,3) = lne(1,1);
	p(0,4) = lne(2,1);
	p(1,4) = lne(3,1);
#endif 

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
