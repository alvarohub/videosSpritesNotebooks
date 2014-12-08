#ifndef SPRING_H
#define SPRING_H

// Springs between particles of the active contour:

#include "classPointMass.h"

class spring {

	public:

		spring();
    
        void addEndPoints(pointMass* A, pointMass* B);
		
		pointMass * p_massA;
		pointMass * p_massB;
		
		float distance;
		float springiness;	 // this is the k, springiness constant
	
		void updateSymmetric(); // affect both masses
        void updateEnd();
        void updateStart();
};


#endif
