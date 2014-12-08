#pragma once

#include "ofMain.h"
#include "ofQTKitPlayer.h"

#include "videoSprite.h"
#include "classSpring.h"


enum ModeShow {Auto, Interactive};


class ofApp : public ofBaseApp {

	public:

		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    void chooseRandomInFocus();
    
        //Add this function to your ofApp to have it called at the moment before the app is terminated. This is useful for
        //doing cleanup stuff or making sure files are saved before the app terminates:
        void exit(); // this will be necessary to properly clear the heap on the vector of video sprites. 

        ofXml configVideosXML;
        ofTrueTypeFont TTF;
    
    ModeShow showMode;
    
		//instead of using ofVideoPlayer we use the ofQTKitPlayer directly
		//ofQTKitPlayer fingerMovie;
        vector <VideoSprite*> myMovies;
    
        //vector <spring> springs;
        //vector<bezier> connections;
		
    int scale;
    
    ofShader    shader;
    
        bool frameByframe;
};

