//
//  videoSprite.cpp
//  osxHighPerformanceVideoPlayerExample
//
//  Created by Alvaro Cassinelli on 8/31/14.
//
//

#include "videoSprite.h"

// Constructor:
VideoSprite::VideoSprite() {
  
    //(1) init of common parameters of dynamics:
    //(a) For the "2d anchor":
    videoAnchor.setPos(ofGetWidth()/2, ofGetHeight()/2); // second coordinate won't be used in principle (we could have a template class for the points...)
    videoAnchor.setSpeed(0,0);
	videoAnchor.setIntegrationStep(.7);
    videoAnchor.dampBorder=.002;
    videoAnchor.dampMotion=.001;
    videoAnchor.mass=0.5;
	videoAnchor.setWallLimits(ofGetWidth(), ofGetHeight(), 0,0);
	videoAnchor.myTopology=Bounce;
    
    //(b) for the actual position:
    videoPosition.setPos(ofGetWidth()/2, ofGetHeight()/2); // second coordinate won't be used in principle (we could have a template class for the points...)
    videoPosition.setSpeed(15,10);
	videoPosition.setIntegrationStep(.05);
    videoPosition.dampBorder=.02;
    videoPosition.dampMotion=.1;
    videoPosition.mass=0.5;
	videoPosition.setWallLimits(ofGetWidth(), ofGetHeight(), 0,0);
	videoPosition.myTopology=Bounce;
    
    //(c) Anchor spring:
    springToAnchor.addEndPoints(&videoAnchor, &videoPosition); // a spring between videoHeadAnchor and videoPosition
    springToAnchor.distance=0;
    springToAnchor.springiness=0.7;	 // this is the k, springiness constant
    setAnchorSpring(true); //active by default
   
    //1.5) For the z coordinate:
      //(a) For the "z anchor":
    zCoordinateAnchor.setPos(0,0); // second coordinate won't be used in principle
    zCoordinateAnchor.setSpeed(0,0);
	zCoordinateAnchor.setIntegrationStep(.7);
    zCoordinateAnchor.dampBorder=.002;
    zCoordinateAnchor.dampMotion=.001;
    zCoordinateAnchor.mass=0.5;
	zCoordinateAnchor.setWallLimits(1.5, 1.5, 0,0);
	zCoordinateAnchor.myTopology=Bounce;
    
    //(b) for the actual position:
    zCoordinate.setPos(0,0); // second coordinate won't be used in principle (we could have a template class for the points...)
    zCoordinate.setSpeed(0,0);
	zCoordinate.setIntegrationStep(.05);
    zCoordinate.dampBorder=.02;
    zCoordinate.dampMotion=.1;
    zCoordinate.mass=0.5;
	zCoordinate.setWallLimits(1.5, 1.5, 0, 0);
	zCoordinate.myTopology=Bounce;
    
    //(c) Depth Anchor spring:
    springToDepthAnchor.addEndPoints(&zCoordinateAnchor, &zCoordinate); // a spring between videoHeadAnchor and videoPosition
    springToDepthAnchor.distance=0;
    springToDepthAnchor.springiness=0.7;	 // this is the k, springiness constant
    setAnchorDepthSpring(true); //active by default
 
    // (2) init the temporal videoHead dynamics:
    //(a) For the temporal anchor:
    videoHeadAnchor.setPos(0,0); // second coordinate won't be used in principle (we could have a template class for the points...)
    videoHeadAnchor.setSpeed(0,0);
	videoHeadAnchor.setIntegrationStep(.03);
    videoHeadAnchor.mass=0.5;
	videoHeadAnchor.setWallLimits(100, 100, 0,0); // from 0 to 100, so we can use a percent value
	videoHeadAnchor.myTopology=Bounce;
    
    //(b) For the actual temporal reading head:
    videoHead.setPos(0,0); // second coordinate won't be used in principle (we could have a template class for the points...)
    videoHead.setSpeed(0,0);
	videoHead.setIntegrationStep(.07);
    videoHead.dampBorder=.02;
    videoHead.dampMotion=.1;
    videoHead.mass=0.5;
	//videoHead.setWallLimits(this->getDuration(), 360, 0,0); // second value is not relevant (perhaps used for other parameter like rotation)
	videoHead.myTopology=Bounce;
    
    //(c) Anchor Head spring:
    springToAnchorHead.addEndPoints(&videoHeadAnchor, &videoHead); // a spring between videoHeadAnchor and videoHead (note: order is important to use asymmetric updating of springs)
    springToAnchorHead.distance=0;
    springToAnchorHead.springiness=1;	 // this is the k, springiness constant
    setAnchorHeadSpring(true); //active by default
}

// Static methods:
// (1) this will be called at any time to analyse the videos metadata and attach them
void VideoSprite::attachSprings( vector<VideoSprite*>& mySpriteVector) {
  //(1) we will attach springs whenever there is a keyword that matches
     string stringA;
    for (int i=0; i<mySpriteVector.size(); i++) {
        for (int j=i; j<mySpriteVector.size(); j++) {
            
            // go through all the keywords in mySpriteVector[i] and see if any of these are in mySpriteVector[j]:
            for (int k=0; k<mySpriteVector[i]->myData.keywords.size(); k++) {
             stringA=mySpriteVector[i]->myData.keywords[k];
             vector<string> vec=mySpriteVector[j]->myData.keywords;
             bool isPresent = (std::find(vec.begin(), vec.end(), stringA) != vec.end());
                if (isPresent) { // add a spring between object i and j, and exit the k loop:
                    spring newSpring;
                    newSpring.addEndPoints(&(mySpriteVector[i]->videoPosition),&(mySpriteVector[j]->videoPosition));
                    springArray.push_back(newSpring);
                    break; // break the k loop
                }
                
        }
    }
    }
}

bool VideoSprite::loadSprite(string name) {
    
    // #1 Play videos with an alpha channel. ---------------------------
    // ofQTKitPlayer videos encoded with Alpha channels (e.g. Animation Codec etc).
    // The pixel format MUST be enabled prior to loading!
    // If an alpha channels is not used, setting a non-alpha pixel format
    // (e.g. OF_PIXELS_RGB) will increase performance.
    this->setPixelFormat(OF_PIXELS_RGBA);
    
    //# 2 Decode Modes and video loading. -------------------------------
	// If you don't need direct access to pixel data, you can use ofQTKitPlayer's
    // highly optimized internal texture management system.  These mode are not
    // available when using the cross-platform ofVideoPlayer.
    
	// Texture only is fastest, but no pixel access allowed.
	// ofQTKitDecodeMode decodeMode = OF_QTKIT_DECODE_TEXTURE_ONLY;
    
	// Pixels and texture together is faster than PIXEL_ONLY and manually uploaded textures.
    ofQTKitDecodeMode decodeMode = OF_QTKIT_DECODE_PIXELS_AND_TEXTURE;
    
    bool canweload=this->loadMovie(name, decodeMode);
    if (canweload) {
    // Size of video (angles?):
    setSizeVideo(this->getWidth(), this->getHeight());
    scaleSizeVideo(30);// make it only 30% from the original size
    cout << currentVideoSize.x << " " << currentVideoSize.y << endl;
    }
    return(canweload);
}

bool VideoSprite::loadMetadata(string nameMetadataFile) {
    // this will load the struct of myData from the metadata file
    
}

void VideoSprite::drawSprite() {// overload drawing method that draws at the position of the anchor
    ofPushMatrix();
    ofSetHexColor(0xFFFFFF);
    ofTranslate(videoPosition.pos.x, videoPosition.pos.y, zCoordinate.pos.x);
    
    ofRotateZ(360*zCoordinate.pos.x);
    // ofRotateY(mouseY);
    
    //drawWithZ(videoPosition.pos.x-currentVideoSize.x/2, videoPosition.pos.y-currentVideoSize.y/2, currentVideoSize.x, currentVideoSize.y, zCoordinateAnchor);
    //drawWithZ(videoPosition.pos.x, videoPosition.pos.y, currentVideoSize.x, currentVideoSize.y, zCoordinateAnchor);
    
    ofTranslate(-currentVideoSize.x/2, -currentVideoSize.y/2, zCoordinate.pos.x);
    draw(0,0, currentVideoSize.x, currentVideoSize.y);
    ofPopMatrix();
}

void VideoSprite::setAnchorSpring(bool set) {
    anchorSpringMode=set;
}

void VideoSprite::setAnchorHeadSpring(bool set) {
    anchorHeadSpringMode=set;
}

void VideoSprite::setAnchorDepthSpring(bool set) {
    depthSpringMode=set;
}


void VideoSprite::updateKinematics() {
    
    // FOR THE 2D position:
    
     videoPosition.resetForce();
     //videoPosition.addRepulsionForce(mouseX, mouseY, 200, 0.7f);
    
    // Motion of the video:
    // (a) NICE crazy motion:
    //videoPosition.addSpringForce(x, y, 100, -100); // third parameter is "radius of action"
    //videoPosition.addRepulsiveSpringForce(x, y, 100, -100); // third parameter is "inverted radius of action"

    // (b) Motion of the video:
 // videoPosition.addSpringForce(x, y, 200, -30); // third parameter is "radius of action"
//  videoPosition.addRepulsiveSpringForce(x, y, 100, -10); // third parameter is "inverted radius of action"

     //Anchor springs:
    if (anchorSpringMode) {
     springToAnchor.updateEnd(); // or symmetric, because we are not going to update the videoAnchorPosition...
     //springToAnchor.updateSymmetric();
    }
    
    // inter-videos springs:
    //springs[i].update() // done in a static method globally
    
     videoPosition.addDampingForce();
     videoPosition.update();
    
    // 2) FOR THE DEPTH:
    zCoordinate.resetForce();
    //Anchor springs:
    if (depthSpringMode) {
        springToDepthAnchor.updateEnd(); // or symmetric, because we are not going to update the videoAnchorPosition...
        //springToAnchor.updateSymmetric();
    }
    zCoordinate.addDampingForce();
    zCoordinate.update();
    
}


// Time control:
void VideoSprite::updateFrame() {
    
    // The update here is different from the kinematic one: the ANCHOR moves, and drags the real head:
    //(1)
    //videoHeadAnchor.resetForce();
    
    // Add whatever forces comes from the interaction to the videoAnchorHead (optical flow, sound, time...)
    //videoHeadAnchor.addRepulsionForce(mouseX, mouseY, 200, 0.7f);
    //... or set the position directly:
    //videoHeadAnchor.setPos(mouseX, 0); // this is done in the mouse callback
    
   // videoHead.addDampingForce();
   // videoHead.update()
    
    // (2) Drag of the video head:
    videoHead.resetForce();
    //Anchor springs will only affect the END point, i.e., the videoHead:
    springToAnchorHead.updateEnd();
    videoHead.addDampingForce();
    videoHead.update();
    
   // cout <<videoHead.pos.x<<" "<<videoHeadAnchor.pos.x<<endl;
    
    // (3) Seek the frame pointed by videoHead:
    this->setPositionInSeconds(videoHead.pos.x); // only one dimension used, but the second could be used to add rotation or change of size!
    this->update();

    //getPosition();
    //float               getPositionInSeconds();
    //float               getSpeed();
    //ofLoopType          getLoopState();
    //float               getDuration(); // in seconds
    
    //setPaused(bool bPaused);
    //void                setPosition(float pct); // from 0 to 1!! not in seconds...
    //void                setVolume(float volume);
    //void                setBalance(float balance);
    //void                setLoopState(ofLoopType state);
    //void                setSpeed(float speed);
    //void                setFrame(int frame); // frame 0 = first frame...
}

