
// NOTEBOOK EXHIBITION (c) ALVARO CASSINELLI 2014


#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){

	ofSetupOpenGL(800*7,450*6, OF_FULLSCREEN);//OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new ofApp());

}
