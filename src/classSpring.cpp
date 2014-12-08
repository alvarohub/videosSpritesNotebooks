#include "classSpring.h"

//---------------------------------------------------------------------
spring::spring(){
	p_massA = NULL;
	p_massB = NULL;
}

void spring::addEndPoints(pointMass* A, pointMass* B) {
	p_massA = A;
	p_massB = B;
}

//---------------------------------------------------------------------
void spring::updateEnd(){
	if ((p_massA == NULL) || (p_massB == NULL)){
		return;
	}
	
	ofVec2f pta = p_massA->pos;
	ofVec2f ptb = p_massB->pos;
	
	float theirDistance = (ptb-pta).length();
	float springForce = springiness * (theirDistance - distance);
	ofVec2f frcToAdd = (ptb-pta).normalize() * springForce;
	
	//p_massA->addForce(frcToAdd);
	p_massB->addForce(-frcToAdd);
	//massA->totalForce.x+=frcToAdd.x;massA->totalForce.y+=frcToAdd.y;
	//massB->totalForce.x-=frcToAdd.x;massB->totalForce.y-=frcToAdd.y;
}

void spring::updateStart(){
	if ((p_massA == NULL) || (p_massB == NULL)){
		return;
	}
	
	ofVec2f pta = p_massA->pos;
	ofVec2f ptb = p_massB->pos;
	
	float theirDistance = (ptb-pta).length();
	float springForce = springiness * (theirDistance - distance);
	ofVec2f frcToAdd = (ptb-pta).normalize() * springForce;
	
	p_massA->addForce(frcToAdd);
	//p_massB->addForce(-frcToAdd);
	//massA->totalForce.x+=frcToAdd.x;massA->totalForce.y+=frcToAdd.y;
	//massB->totalForce.x-=frcToAdd.x;massB->totalForce.y-=frcToAdd.y;
}

void spring::updateSymmetric(){
	if ((p_massA == NULL) || (p_massB == NULL)){
		return;
	}
	
	ofVec2f pta = p_massA->pos;
	ofVec2f ptb = p_massB->pos;
	
	float theirDistance = (ptb - pta).length();
	float springForce = springiness * (theirDistance - distance);
	ofVec2f frcToAdd = (ptb-pta).normalize() * springForce;
	
	p_massA->addForce(frcToAdd);
	p_massB->addForce(-frcToAdd);
	//massA->totalForce.x+=frcToAdd.x;massA->totalForce.y+=frcToAdd.y;
	//massB->totalForce.x-=frcToAdd.x;massB->totalForce.y-=frcToAdd.y;
}