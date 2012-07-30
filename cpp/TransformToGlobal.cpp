#include "TransformToGlobal.h"
#include "pi_to_pi.h"

void TransformToGlobal(MatrixXf &p, VectorXf b) 
{
	//rotate
	MatrixXf rot(2,2);
	rot<<cos(b(2)), -sin(b(2)), sin(b(2)), cos(b(2));
	
	MatrixXf p_resized;
	p_resized = MatrixXf(p);
	p_resized.conservativeResize(2,p_resized.cols());
	p_resized = rot*p_resized;		
	
	//translate
	int c;
	for (c=0;c<p_resized.cols();c++) {
		p(0,c) = p_resized(0,c)+b(0); 				
		p(1,c) = p_resized(1,c)+b(1); 				
	}

	float input;
	//if p is a pose and not a point
	if (p.rows() ==3){
		for (int k=0; k<p_resized.cols();k++) {
			input = p(2,k) +b(2);
   			pi_to_pi(input);
			p(2,k) = input;
		}		
	}	
}
