//
//  videoSprite.h
//  osxHighPerformanceVideoPlayerExample
//
//  Created by Alvaro Cassinelli on 8/31/14.
//
// NOTE: this class adds only dynamic motion to the videos, (ofQTKitPlayer or ofVideoPlayer), and also
// metadata to the videos (to create visual links/forces)
//
//

#ifndef osxHighPerformanceVideoPlayerExample_videoSprite_h
#define osxHighPerformanceVideoPlayerExample_videoSprite_h

#include "classPointMass.h"
#include "classSpring.h"

enum ModeSprite {inFocus, backGround}; // this are modes for auto show

typedef struct  {
	string name;
	int ID;
    vector<string> keywords; // to create the links, can be any annotation I make
} VideoMetadata;

class VideoSprite : public ofQTKitPlayer {
public:
    // Constructor:
    VideoSprite();
    
    
    // Note: the following methods apply to the collection of instances of the class;
    // I could have made them globals, or belonging to a parent class, but this makes more sense as it ressembles a namespace.
    // Now, I also could have a vector of pointers to the instantiated objects (adding them in the constructor), but then
    // I may not be able to have SEPARATE collection of videoSprites. So, finally I opted by this solution: passing a reference to a global vector of videosprites...
    static void attachSprings( vector<VideoSprite*>&); // this will be called at any time to analyse videos metadata and attach springs between them
    static void drawConnections( vector<VideoSprite*>&); // same for drawing connections
    static void drawSprites( vector<VideoSprite*>&); // same for drawing the actual videos
    //static void updateKinematics( vector<VideoSprite>&);
    
    
    bool loadSprite(string name);
    bool loadMetadata(string path);
    
    // 1) Kinematics 2d:
    // (a) Anchor:
    void initPos(ofVec2f initPos) {videoAnchor.setPos(initPos); videoAnchorInitial.setPos(initPos);};
    void initPos(float x, float y){videoAnchor.setPos(x, y); videoAnchorInitial.setPos(x, y);};
    void initVel(ofVec2f initSpeed) {videoAnchor.setSpeed(initSpeed);videoAnchorInitial.setSpeed(initSpeed);};
    void initVel(float vx, float vy) {videoAnchor.setSpeed(vx, vy);videoAnchorInitial.setSpeed(vx, vy);};
    // (b) current position:
    void setPos(ofVec2f pos) {videoPosition.setPos(pos);};
    void setPos(float x, float y) {videoPosition.setPos(x, y);};
    void setVel(ofVec2f speed) {videoPosition.setSpeed(speed);};
    void setVel(float vx, float vy) {videoPosition.setSpeed(vx, vy);};
    
    void setAnchorSpring(bool set); // this activate the springToAnchor
    
    // 1.5) Kinematics depth:
    void initDepth(ofVec2f initPos) {zCoordinateAnchor.setPos(initPos); zCoordinateAnchorInitial.setPos(initPos);};
    void initDepth(float x, float y){zCoordinateAnchor.setPos(x, y); zCoordinateAnchorInitial.setPos(x, y);};
    void initDepthVel(ofVec2f initSpeed) {zCoordinateAnchor.setSpeed(initSpeed);zCoordinateAnchorInitial.setSpeed(initSpeed);};
    void initDepthVel(float vx, float vy) {zCoordinateAnchor.setSpeed(vx, vy);zCoordinateAnchorInitial.setSpeed(vx, vy);};
    // (b) current position:
    void setDepth(ofVec2f pos) {zCoordinate.setPos(pos);};
    void setDepth(float x, float y) {zCoordinate.setPos(x, y);};
    void setDepthVel(ofVec2f speed) {zCoordinate.setSpeed(speed);};
    void setDepthVel(float vx, float vy) {zCoordinate.setSpeed(vx, vy);};
    
    void setAnchorDepthSpring(bool set);
    
    // Update function:
    void updateKinematics();
    
    
    //2) Reading head:
    void initHeadPos(ofVec2f initPos) {videoHeadAnchor.setPos(initPos); videoHeadAnchorInitial.setPos(initPos);};
    void initHeadPos(float x, float y){videoHeadAnchor.setPos(x, y); videoHeadAnchorInitial.setPos(x, y);};
    void initHeadVel(ofVec2f initSpeed) {videoHeadAnchor.setSpeed(initSpeed);videoHeadAnchorInitial.setSpeed(initSpeed);};
    void initHeadVel(float vx, float vy) {videoHeadAnchor.setSpeed(vx, vy);videoHeadAnchorInitial.setSpeed(vx, vy);};
    // (b) current position:
    void setHeadPos(ofVec2f pos) {videoHead.setPos(pos);};
    void setHeadPos(float x, float y) {videoHead.setPos(x, y);};
    void setHeadVel(ofVec2f speed) {videoHead.setSpeed(speed);};
    void setHeadVel(float vx, float vy) {videoHead.setSpeed(vx, vy);};
    
    void setAnchorHeadSpring(bool set); // this activate the springToAnchorHead
    
    // play control (most other controls directly from ofQTKitPlayer...)
    void updateFrame();
    
    // drawing:
    void drawSprite();
    void setSizeVideo(const ofVec2f& siz) {videoSizeOriginal=siz;currentVideoSize=siz;}; // note: on ofVec2f, "=" is overload as set, so we can use const
    void setSizeVideo(float w, float h) {videoSizeOriginal.x=w; videoSizeOriginal.y=h;
    currentVideoSize.x=w; currentVideoSize.y=h;};
    
    void scaleSizeVideo(float percentage) {currentVideoSize=videoSizeOriginal*percentage/100.0;}; //
    ofVec2f videoSizeOriginal, currentVideoSize;
    
    // make this private:
    pointMass videoPosition; // current position
    pointMass videoAnchor, videoAnchorInitial; // this will be a "point" of attachment
    // Z coordinate treated independently (pointMass should be 3d... easier - use templete classes!)
    pointMass zCoordinate;
    pointMass zCoordinateAnchor, zCoordinateAnchorInitial;

    // current video head position (this will be a particle moving in a single dimension):
    pointMass videoHead;
    pointMass videoHeadAnchor, videoHeadAnchorInitial;

    // I needed to add this because it does not exist in the original ofQTKitPlayer object:
    void setPositionInSeconds(float sec) {setPosition(1.0*sec/getDuration());};
    void drawWithZ(float x, float y, float w, float h, float z) {
        updateTexture();
        tex.draw(x,y,z, w,h);
        //draw(float x, float y, float z, float w, float h)
    }
    
    
    
    ModeSprite spriteMode;
    
private:
    VideoMetadata myData;
   // ofXml metadataXML;
   
    bool anchorSpringMode;
    spring springToAnchor; // a spring between videoAnchor and videoPosition
    
    bool anchorHeadSpringMode;
    spring springToAnchorHead; // a spring between videoHeadAnchor and videoHead
    
    bool depthSpringMode;
    spring springToDepthAnchor; // a spring between videoHeadAnchor and videoHead
    
    
    static vector<spring> springArray; // springs (and bezier curves?) are static objects that will attach the instantiated videos.

    
};


#endif
