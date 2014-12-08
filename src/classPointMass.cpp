/*
 *  pointMass.cpp
 *  laserBlob
 *
 *  Created by CASSINELLI ALVARO on 5/19/11.
 *  Copyright 2011 TOKYO UNIVERSITY. All rights reserved.
 *
 */

#include "classPointMass.h"

//------------------------------------------------------------
pointMass::pointMass(){
	setIntegrationStep(.01); // default in case we don't call integration step setting
	// NOTE: it is important to set dt before inital conditions in case of VERLET integration, because we need the integration
	// step for properly setting the initial speed.
	setInitialCondition(0,0,0,0);// default in case we don't call to initial conditions.
	setWallLimits(2000, 2000, 300, 300);
	mass=1.0; 
	dampMotion = 0.07f;
	dampBorder = 0.07f;
	bFixed = false;
	bDidICollide=false;
    myTopology=Bounce;
}

//------------------------------------------------------------
void pointMass::resetForce(){
    totalForce.set(0,0);
}

//------------------------------------------------------------
void pointMass::addForce(float x, float y){
	totalForce.x = totalForce.x + x;
    totalForce.y = totalForce.y + y;
}

//------------------------------------------------------------
void pointMass::addForce(ofVec2f forceToAdd){
	totalForce+=forceToAdd;
}

//------------------------------------------------------------
void pointMass::addInvSquareForce(float x, float y, float radiusMin, float radiusMax, float scale){
    
	ofVec2f posOfForce;
	posOfForce.set(x,y);
		
	ofVec2f diff	= pos - posOfForce; // note: we use the position AT TIME T, so this force is at time t
	float length	= diff.length();
	
	// check close enough and far enough (to avoid singularities for example):
	if ((length>radiusMin)&&(length<radiusMax)) {
        diff.normalize();
		totalForce += diff * scale * 1.0/(length*length+1);
    }
}

void pointMass::addInterInvSquareForce(pointMass &theOtherParticle, float radiusMin, float radiusMax, float scale){
    
	ofVec2f posOfForce;
	posOfForce.set(theOtherParticle.pos);
	
	ofVec2f diff	= pos - posOfForce; // note: we use the position AT TIME T, so this force is at time t
	float length	= diff.length();
	
	// check close enough and far enough (to avoid singularities for example):
	if ((length>radiusMin)&&(length<radiusMax)) {
        diff.normalize();
		totalForce += diff * scale * 1.0/(length*length+1);
		theOtherParticle.totalForce -= diff * scale * 1.0/(length*length+1);
    }
}


//------------------------------------------------------------
void pointMass::addSpringForce(float centerx, float centery, float radiusOfAction, float scale){
	
	// ----------- (1) make a vector of where this particle p is: 
	ofVec2f posOfForce;
	posOfForce.set(centerx, centery);
	
	// ----------- (2) calculate the difference & length 
	
	ofVec2f diff	= pos - posOfForce;
	float length	= diff.length();
	
	// ----------- (3) check close enough
	
	bool bAmCloseEnough = true;
    if (radiusOfAction > 0){
        if (length > radiusOfAction){
            bAmCloseEnough = false;
        }
    }
	
	// ----------- (4) if so, update force
    
	if (bAmCloseEnough == true){
		float pct = 1 - (length / radiusOfAction);  // stronger on the inside
		diff.normalize();
		totalForce += diff * scale * pct;
	}
}

void pointMass::addRepulsiveSpringForce(float centerx, float centery, float radiusOfAction, float scale){
	
	// ----------- (1) make a vector of where this particle p is:
	ofVec2f posOfForce;
	posOfForce.set(centerx, centery);
	
	// ----------- (2) calculate the difference & length
	
	ofVec2f diff	= pos - posOfForce;
	float length	= diff.length();
	
	// ----------- (3) check close enough
	
	bool bAmCloseEnough = true;
    if (radiusOfAction > 0){
        if (length < radiusOfAction){
            bAmCloseEnough = false;
        }
    }
	
	// ----------- (4) if so, update force
    
	if (bAmCloseEnough == true){
		float pct = 1 - (length / radiusOfAction);  // stronger on the inside
		diff.normalize();
		totalForce += diff * scale * pct;
	}
}



void pointMass::addInterSpringForce(pointMass &theOtherParticle, float radius, float scale){
	
	// ----------- (1) make a vector of where this particle p is: 
	ofVec2f posOfForce;
	posOfForce.set(theOtherParticle.pos);
	
	// ----------- (2) calculate the difference & length 
	
	ofVec2f diff	= pos - posOfForce;
	float length	= diff.length();
	
	// ----------- (3) check close enough
	
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
	
	// ----------- (4) if so, update force
    
	if (bAmCloseEnough == true){
		float pct = 1 - (length / radius);  // stronger on the inside
		diff.normalize();
		totalForce += diff * scale * pct;
		theOtherParticle.totalForce -= diff * scale * pct;
		//theOtherParticle.frc.x = p.frc.x - diff.x * scale * pct;
        //theOtherParticle.frc.y = p.frc.y - diff.y * scale * pct;
    }
}



//------------------------------------------------------------
void pointMass::addDampingForce(){ // NOTE: use only in case of EULER intgration!
    totalForce-= speed* dampMotion;
}

//------------------------------------------------------------
void pointMass::setIntegrationStep(float _dt){
	dt=_dt;
}

//------------------------------------------------------------
void pointMass::setInitialCondition(float px, float py, float vx, float vy){
#ifndef VERLET_METHOD
	pos.set(px,py);
	speed.set(vx,vy);
#else
	// In case of Verlet method, setting the speed is a little more complicated. It involves in particular the integration step
	// through the approximation formula: 
	// speed = (posNew-posOld)/(2*dt), or speed=(pos-posOld)/dt. Hence:
	pos.set(px, py);
	setSpeed(vx, vy); // this assumes pos known
#endif	
}

//-------------------------------------------------------
ofVec2f pointMass::getSpeed() {
	// this will give an estimate of the speed (not computed explicitly using the Verlet integration):
	//speed=(posNew-posOld)/(2*dt); // the variable speed is also updated (note: it is private)
	speed=(pos-posOld)/dt; // less approximate than the above, but we avoid having a global posNew variable 
	return(speed); 
}

void pointMass::setSpeed(const ofVec2f& vel) {
	setSpeed(vel.x, vel.y);
}

void pointMass::setSpeed(float vx, float vy) {
	speed.set(vx, vy); // enough for EULER METHOD
#ifdef VERLET_METHOD
    posOld=pos-speed*dt; // NECESSARY for VERLET METHOD (we assume pos known).
    //pos=speed*dt+posOld;
	// no need to compute newPos
#endif
}

void pointMass::setPos(float px, float py) { // assuming the speed is unchanged (must do some tweaking in case of Verlet integration)
	pos.set(px, py);
	posOld=pos-speed*dt;
}

//------------------------------------------------------------
void pointMass::update(){	
	if (bFixed == false){
		acc=totalForce/mass; // this is the acceleration at time t
	
#ifndef VERLET_METHOD
		
		// The following equations (Euler integration) assume acceleration constant during time dt:
		speed = speed + acc*dt;
		pos = pos + speed*dt ;//+acc*dt*dt*0.5;
#else
		// The following equations are for VERLET integration with pseudo-damping:
		ofVec2f posNew=pos*(2-dampMotion)-posOld*(1-dampMotion)+acc*dt*dt;
		// Without damping this is just: posNew=2*posOld-1*pos+acc*dt*dt; // i.e., dampMotion=0;
		
		posOld=pos;
		pos=posNew;
		
		// NOTE: we can also estimate the speed if we want. But this may be unnecessary (call getSpeed() for that). 
		
#endif

        // Until here, the update was unconstrained:
        switch(myTopology) {
            case Toroidal:
                toroidalMotion();
                break;
            case Bounce:
                bounceOffWalls(); // constrain position
                break;
            case NonConstrained:
                // .. do nothing (can go very wrong...)
                break;
            default:
                bounceOffWalls(); // constrain position
                break;
        
        }
        
	}
}

void pointMass::setWallLimits(float maxx, float maxy, float minx, float miny) {
	maxWall.set(maxx, maxy);
	minWall.set(minx, miny);
}

//------------------------------------------------------------
void pointMass::bounceOffWalls(){
	// NOTE: bounce is easy in case of EULER method; in case of VERLET, we need to do some hack on the positions.
	
	//Note: the walls are in (ofVec2f) horizontalLimits and verticalLimits 
	
#ifndef VERLET_METHOD
	
	if (pos.x > maxWall.x){
		pos.x = maxWall.x; 
		speed.x *= -1;
		bDidICollide = true;
	} else if (pos.x < minWall.x){
		pos.x = minWall.x; 
		speed.x *= -1;
		bDidICollide = true;
	}
	
	if (pos.y > maxWall.y){
		pos.y = maxWall.y; 
		speed.y *= -1;
		bDidICollide = true;
	} else if (pos.y < minWall.y){
		pos.y = minWall.y; 
		speed.y *= -1;
		bDidICollide = true;
	}
	
	if (bDidICollide) // damping:
		speed *=(1-dampBorder);
	
#else // THIS IS FOR VERLET METHOD:
	// we need to estimate the inverted, damped vector for bumping::
	ofVec2f bumpVector=getSpeed()*dt*(dampBorder-1); // assuming dampBorder<1 of course
	if (pos.x > maxWall.x){
		//posOld.x=pos.x;
		//pos.x=pos.x+bumpVector.x;
		posOld.x=maxWall.x;
		pos.x=maxWall.x+bumpVector.x;
		bDidICollide = true; // this is just computed here to detect bumps
	} else if (pos.x < minWall.x){
		posOld.x=minWall.x;
		pos.x=minWall.x+bumpVector.x;
		bDidICollide = true;
	}
	
	if (pos.y > maxWall.y){
		posOld.y=maxWall.y;
		pos.y=maxWall.y+bumpVector.y;		
		bDidICollide = true;
	} else if (pos.y < minWall.y){
		posOld.y=minWall.y;
		pos.y=minWall.y+bumpVector.y;
		bDidICollide = true;
	}
#endif
}

void pointMass::toroidalMotion(){

#ifndef VERLET_METHOD
	
	if (pos.x > maxWall.x){
		pos.x = minWall.x;
	} else if (pos.x < minWall.x){
		pos.x = maxWall.x;
	}
	
	if (pos.y > maxWall.y){
		pos.y = minWall.y;
    } else if (pos.y < minWall.y){
		pos.y = maxWall.y;
	}
#else // THIS IS FOR VERLET METHOD:
	// In case of bumping, we needed to estimate the inverted, damped vector for bumping. In case of Toroidal
    // topology, we need to estimate the new old position:
	ofVec2f displacement=getSpeed()*dt;
	if (pos.x > maxWall.x){
		posOld.x=minWall.x-displacement.x;
		pos.x=minWall.x;
	} else if (pos.x < minWall.x){
		posOld.x=maxWall.x-displacement.x;
		pos.x=maxWall.x;
	}
	if (pos.y > maxWall.y){
		posOld.y=minWall.y-displacement.y;
		pos.y=minWall.y;
	} else if (pos.y < minWall.y){
		posOld.y=maxWall.y-displacement.y;
		pos.y=maxWall.y;
	}
#endif
}

