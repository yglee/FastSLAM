#include "pi_to_pi.h"
#include <iostream>

void pi_to_pi(VectorXf &angle) 
{
    int n;
    for (int i=0; i<angle.size(); i++) {
        if ((angle[i] < (-2*pi)) || (angle[i] > (2*pi))) {
            n=floor(angle[i]/(2*pi));
            angle[i] = angle[i]-n*(2*pi);    
            
            if (angle[i] > pi) {
                angle[i] = angle[i] - (2*pi);
            }
            if (angle[i] < -pi) {
                angle[i] = angle[i] + (2*pi);
            }
        }
    }
}

float pi_to_pi(float ang) 
{
    int n;
    if ((ang < (-2*pi)) || (ang > (2*pi))) {
        n=floor(ang/(2*pi));
        ang = ang-n*(2*pi);    

        if (ang > pi) {
            ang = ang - (2*pi);
        }
        if (ang < -pi) {
            ang = ang + (2*pi);
        }
    }
    return ang;
}

#if 0
vector<int> find1(VectorXf input) 
{
	vector<int> index;
    for (int i =0; i<input.size(); i++) {
        if ((input[i] > 2*pi) || (input[i] < -2*pi)){
            index.push_back(i);
        }    
    }
	return index; 
}
#endif
