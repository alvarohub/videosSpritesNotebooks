#include "ofApp.h"
#define STRINGIFY(A) #A

// High Performance OS X Video Playback Example
//--------------------------------------------------------------
// This example shows how to use the OS X platform specific
// ofQTKitPlayer on its own without the cross platform
// ofVideoPlayer wrapper.  Apps you write in this way won't be
// cross platform, but can be faster for HD videos .
//--------------------------------------------------------------

//#define FROM_XML // uncomment if we want to load from a for loop

void ofApp::setup(){
   
    // ofEnableAlphaBlending();
    ofSetDepthTest(true);
    
    // FIXED CODE TRANSPARENCY (not using the shader). I cannot use it, since the videos DOES NOT HAVE alpha channel.
    // (will only allow pixels with alpha values greater than 0.5 to write to the color or depth buffers):
    //glAlphaFunc(GL_GREATER, 0.5);
    //glEnable(GL_ALPHA_TEST);
    
    scale=100;
    
	ofBackground(0);//255, 255, 255);
    
	frameByframe = true;
    
    TTF.loadFont("mono.ttf", 7);
    
    // Load the config of videos:
    configVideosXML.load("configVideos.xml");
    
    // Load the videos:
#ifdef FROM_XML
    // If we have VIDEOS nodes that we've already created, then we can go ahead and
    // load them into the myMovies vector:
    if(configVideosXML.exists("VIDEOS"))
    {
        // This gets the first video (notice the [0], it's just like an array)
        configVideosXML.setTo("SPRITE[0]");
        
        do {
            // set our "current" PT to the first one
            if(configVideosXML.getName() == "VIDEOS" && configVideosXML.setTo("SPRITE[0]"))
            {
                // get each individual x,y for each point
                do {
                    
                    string namevideo=configVideosXML.getValue<string>("NAME");
                    
                    VideoSprite* oneMovie=new VideoSprite();
                    myMovies.push_back(oneMovie);
                    
                    // #1 Play videos with an alpha channel. ---------------------------
                    // ofQTKitPlayer videos encoded with Alpha channels (e.g. Animation Codec etc).
                    // The pixel format MUST be enabled prior to loading!
                    // If an alpha channels is not used, setting a non-alpha pixel format
                    // (e.g. OF_PIXELS_RGB) will increase performance.
                    myMovies.back()->setPixelFormat(OF_PIXELS_RGBA);
                    myMovies.back()->loadSprite("movies/"+namevideo+".mov");
                    
                    // Initial position and speed (from XML file or pre-calculated):
                    //int x = configVideosXML.getValue<int>("X");
                    //int y = configVideosXML.getValue<int>("Y");
                    myMovies.back()->videoAnchorInitial.setPos(0,0);
                    myMovies.back()->videoAnchorInitial.setSpeed(0,0);
                    
                    // Initial position and speed for the video head:
                    myMovies.back()->videoHeadAnchorInitial.setPos(0,0);
                    myMovies.back()->videoHeadAnchorInitial.setSpeed(0,0);
                    
                }
                while(configVideosXML.setToSibling()); // go the next PT
                
                // go back up
                configVideosXML.setToParent();
            }
            
        }
        while( configVideosXML.setToSibling() ); // go to the next VIDEO
    }
#else
    // Read automatically if we know the name and the number of videos:
    // I will read 42 notebooks and put them in a 6x7 grid:
    myMovies.clear();
    int i=0; int j=0;
    int widthVideo=int(1.0*ofGetWidth()/7);
    int heightVideo=int(1.0*widthVideo*450/800);//ofGetHeight()/6;
    for (int i=0; i<6; i++) {//6; i++) {
        for (int j=0; j<7; j++) {//7; j++) {
            int k=j+1+i*7; // first notebook name is "1.mov"
            
            string namevideo;
            if (k<=8) namevideo=ofToString(k)+".mov"; else namevideo=ofToString(k+2)+".mov";
            
            VideoSprite* oneMovie=new VideoSprite();
            myMovies.push_back(oneMovie);
            // PROBLEM! when clearing the vector, I won't be able to call the destructor for each VideoSprite object!!
            
            if (myMovies.back()->loadSprite("movies/"+namevideo)) {
                cout << "Loaded file: "<<"movies/"+namevideo<< endl;
                cout <<"Duration: "<<myMovies.back()->getDuration() << endl;
            } else {
                cout << "Could not load file: "<<"movies/"+namevideo<< endl;
            }
            
            myMovies.back()->setSizeVideo(widthVideo, heightVideo);
            
            // Initial position and speed for the kinematics:
            myMovies.back()->initPos(widthVideo*j+widthVideo/2,heightVideo+heightVideo/2+heightVideo*i);
            //cout << widthVideo*j << " - " << heightVideo+heightVideo*i << endl;
            myMovies.back()->initVel(20*cos(2*PI/21*k),20*sin(2*PI/21*k));
            myMovies.back()->setPos(myMovies.back()->videoAnchor.pos);
            myMovies.back()->setVel(myMovies.back()->videoAnchor.getSpeed());
            // Activate the spring from current position to video anchor:
            myMovies.back()->setAnchorSpring(true);
            
            // Initial depth position and speed:
            myMovies.back()->initDepth(0,0);
            myMovies.back()->initDepthVel(0,0);
            myMovies.back()->setDepth(myMovies.back()->zCoordinateAnchor.pos);
            myMovies.back()->setDepth(myMovies.back()->zCoordinateAnchor.getSpeed());
            myMovies.back()->setAnchorDepthSpring(true);
            
            // Initial position and speed for the video head:
            myMovies.back()->initHeadPos(0,0);
            myMovies.back()->initHeadVel(0,0);
            myMovies.back()->setHeadPos(myMovies.back()->videoHeadAnchor.pos);
            myMovies.back()->setHeadVel(myMovies.back()->videoHeadAnchor.getSpeed());
            myMovies.back()->videoHead.setWallLimits(myMovies.back()->getDuration(), 360, 0,0); // second value is not relevant (perhaps used for other parameter like rotation)
            
            // Activate the spring from current position to video head anchor:
            myMovies.back()->setAnchorHeadSpring(true);
        }
    }
    
#endif
    
    // You can also load movies from URLs.
    // fingerMovie.loadMovie("http://labs.silentlycrashing.net/fingers.mov", decodeMode);
	
    //# 3 Synchronous seeking. ---------------------------
    // Like the legacy player, by default frames are available immediately for display
    // via setFrame().  In many cases when random access via setFrame() is not required,
    // significant speed increases can be gained by disabling synchronous seeking
    // particularly when playing high bitrate HD video.
    
    // In "asynchronous" seek mode, individual frames can still be randomly accessed,
    // but the pixels may not be available immediately without calling update() first.
    
    // fingerMovie.setSynchronousSeeking(false);
    
    cout << ofToString(myMovies.size())+" movies loaded"<< endl;
    
	for (int k=0; k<myMovies.size(); k++) {
        myMovies[k]->play();
        myMovies[k]->setLoopState(OF_LOOP_NONE);
        myMovies[k]->setSynchronousSeeking(false);
        myMovies[k]->setPaused(frameByframe);
        myMovies[k]->setSpeed(1.0);
        // used when in automode:
        myMovies[k]->spriteMode=backGround;
        cout << "Movie "+ofToString(k+1)+" initialized"<<endl;
    }
    
    //Mode of the whole show:
    showMode=Auto;
    chooseRandomInFocus();

    
// ===================================================================
    //SHADER for extraction of background and borders:
    string shaderProgram = STRINGIFY(
                                     //Note: dimensions of the image being displayed NEED to be uniform variables passed to the shader,
                                     // the reason is that the concept of the "size" of the image does not exist here, only rendered points... (size of the screen can
                                     // however be retrieved with GLSL constants...)
                                     
                                     uniform sampler2DRect tex0;
									 uniform vec2 imageSize;
									 uniform float thresholdGrey; // from 0 to 1
									 uniform vec4 cropBorder; // normalized with respect to the image size
                                     uniform float fakeFog;
                                     
                                     void main (void){

                                         vec2 pos = gl_TexCoord[0].st; // position of the texel (not normalized!)
                                         vec4 src = texture2DRect(tex0, pos);  // texture color at that texel:
                                          vec4 colorOutput = src;
                                         
										 // Convert color to grayscale:
                                         float greyLevel = 0.2989 * src.r + 0.5870 * src.g + 0.1140 * src.b;

                                         // (a) Thresholding grey (pass the threshold as uniform)
                                         if (greyLevel<thresholdGrey) colorOutput.a = 0.0; // 0 alpha
										 else colorOutput.a = 1.0;// vec4(src.rgb,1.0); // solid color

                                         // (b) Clip the borders using (note: pass the edges as uniforms)
                                         vec2 dista=2.0*abs(vec2(pos.x-imageSize.x/2.0, pos.y-imageSize.y/2.0))/imageSize;
                                         vec2 edge0=vec2(cropBorder.x, cropBorder.y);
                                         vec2 edge1=vec2(cropBorder.z, cropBorder.w);
                                         vec2 alphaSpace=smoothstep(edge0, edge1, dista);
                                         float alpha2=1.0-max(alphaSpace.x, alphaSpace.y); // pyramidal mask
                                         colorOutput.a=min(colorOutput.a, alpha2);
                                         
                                         // (c) Order independent TRANSPARENCY,
                                         // by discarding rendering = the classical openGL technique:
                                         //glAlphaFunc(GL_GREATER, 0.5);glEnable(GL_ALPHA_TEST);
                                         if(colorOutput.a < 0.1) discard;
                                         
                                         // (d) Fake FOG? (using depth passed as uniform):
                                         vec4 fogcolor=vec4(0.5, 0.0, 0.0,0.4);
                                        // colorOutput=mix(colorOutput, fogcolor, fakeFog);
                                         
                                         gl_FragColor = colorOutput;
                                     }
                                     );
    
    shader.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderProgram);
    shader.linkProgram();
    
    
}

// float blendFactor=gl_TexCoord[0].depth; // NO!
// float r = gl_FragCoord.x / windowWidth;
// float g = gl_FragCoord.y / windowHeight;
//vec2 mouse_norm = vec2( m.x/screen.width, 1.0 - m.y/screen.height )
// vec2 sizeTexture=textureSize(tex0); // size of the texture

//New color in framebuffer =
//current alpha in framebuffer * current color in framebuffer +
//(1 - current alpha in framebuffer) * shader's output color
//src.rgb*src.a+(1.0-src.a)*

//--------------------------------------------------------------
void ofApp::update(){
    
    for (int k=0; k<myMovies.size(); k++) {
        
        myMovies[k]->updateKinematics();
        
        if (myMovies[k]->spriteMode==backGround) {
            // set the anchor to the original one:
            myMovies[k]->initPos(myMovies[k]->videoAnchorInitial.pos);
            myMovies[k]->initVel(myMovies[k]->videoAnchorInitial.getSpeed());
            // set the z coordinate to initial one:
            myMovies[k]->initDepth(myMovies[k]->zCoordinateAnchorInitial.pos);
            myMovies[k]->initDepth(myMovies[k]->zCoordinateAnchorInitial.getSpeed());
            
            //float dist= (myMovies[k]->videoAnchor.pos-myMovies[k]->videoPosition.pos).length();
            float dist= 1000*(1.0-myMovies[k]->zCoordinate.pos.x);
            myMovies[k]->scaleSizeVideo(80+500*exp(-dist*dist/50000));
        
        } else {

            myMovies[k]->videoAnchor.setPos(ofGetWidth()/2, ofGetHeight()/2+85);
            myMovies[k]->zCoordinateAnchor.setPos(1, 0); // 1 is the "focus depth" (second coordinate irrelevant...)
            
            //float dist= (myMovies[k]->videoAnchor.pos-myMovies[k]->videoPosition.pos).length();
            float dist= 1000*(1.0-myMovies[k]->zCoordinate.pos.x);
            myMovies[k]->scaleSizeVideo(80+500*exp(-dist*dist/50000));
            
            // We don't change the current position so it relax to the anchor:
           // myMovies[k]->setPos(ofGetWidth()/2, ofGetHeight()/2);
            //myMovies[k]->setDepth(1, 0); // 1 is the "focus depth" (second coordinate irrelevant...)
            
//            if (frameByframe) myMovies[k]->updateFrame();
//            else {
//                //the speed is given by the speed of the movie (but we could set the speed of the movie
                // insted of frame by frame seeking directly from the speed of videoHead. To test...
                myMovies[k]->update();
       //     }
          
            
            // Check if we have ended with this movie:
            if (myMovies[k]->getIsMovieDone()) { // seems not working...
                chooseRandomInFocus();
                cout << "End Of Movie" << endl;
            }
            
            
            
        }
    }
    
    //for (int k=0; k<myMovies.size(); k++) myMovies[k]->updateKinematics(mouseX, mouseY);
    
}

void ofApp::chooseRandomInFocus() {
    int randomNumber = (int) ofRandom(0,myMovies.size());
     for (int k=0; k<myMovies.size(); k++) {
         myMovies[k]->spriteMode=backGround;
         myMovies[k]->firstFrame();
         myMovies[k]->setPaused(true);
     }
    myMovies[randomNumber]->spriteMode=inFocus;
    myMovies[randomNumber]->play();
    myMovies[randomNumber]->setSpeed(2.0);
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // For some reason I need to update the movie in the draw and not in the update,
    // otherwise it throws an error (texture not allocated)
    
    for (int k=0; k<myMovies.size(); k++) {
        
        if (myMovies[k]->isLoaded()) {
            
//            if (frameByframe) myMovies[k]->updateFrame();
//            else {//the speed is given by the speed of the movie (but we could set the speed of the movie
//                // insted of frame by frame seeking directly from the speed of videoHead. To test...
//                myMovies[k]->update();
//            };
            
           
            // if ((myMovies[k]->getTexture()->isAllocated())&&(myMovies[k]->getTexture()!=NULL)) { // WHY I have to do this???
            if (myMovies[k]->getTexture()!=NULL) { // WHY I have to do this???
                shader.begin();
                // Pass the video texture with ofTexture * getTexture();
                // if decodeMode == OF_QTKIT_DECODE_PIXELS_ONLY,
                // the returned pointer will be NULL.
                // pass other things:
                shader.setUniformTexture("tex0", *(myMovies[k]->getTexture()), 1 );
                shader.setUniform2f("imageSize", myMovies[k]->getWidth(), myMovies[k]->getHeight() );
                shader.setUniform1f("thresholdGrey", 0.1);
                shader.setUniform4f("cropBorder", 0.9, 0.9, 1.0, 1.0); // normalized with respect to the image size
                shader.setUniform1f("fakeFog", ofClamp(fabs(myMovies[k]->zCoordinateAnchor.pos.x/10), 0, 0.5));
                
                ofPushMatrix();
                //ofRotateX(25);
                //ofRotateY(25);
                
                myMovies[k]->drawSprite();
                
                ofPopMatrix();
                
                shader.end();
            }
    
        }
    }
}

void ofApp::exit() {
    for (int k=0; k<myMovies.size(); k++) {
        delete(myMovies[k]);
    }
    myMovies.clear(); // not really necessary as the vector destructor will be called automatically when exiting the program
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch(key){
            
        case ' ':
            chooseRandomInFocus();
            break;
            
        case 'f':
            frameByframe = !frameByframe;
            for (int k=0; k<myMovies.size(); k++) {
                myMovies[k]->setPaused(frameByframe);
            }
            break;
            
        case 'i':
            showMode=Interactive;
            break;
        case 'I':
            showMode=Auto;
            break;
            
        case OF_KEY_LEFT:
            for (int k=0; k<myMovies.size(); k++) myMovies[k]->previousFrame();
            break;
            
        case OF_KEY_RIGHT:
            for (int k=0; k<myMovies.size(); k++) myMovies[k]->nextFrame();
            break;
            
        case '0':
            for (int k=0; k<myMovies.size(); k++) myMovies[k]->firstFrame();
            break;
            
        case '2':
            scale+=10;
            for (int k=0; k<myMovies.size(); k++)  myMovies[k]->scaleSizeVideo(scale);
            break;
        case '1':
            scale-=10; if (scale<10) scale=10;
            for (int k=0; k<myMovies.size(); k++)  myMovies[k]->scaleSizeVideo(scale);
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    if (showMode==Interactive) {
    ofVec2f mousePos(x, y);
    for (int k=0; k<myMovies.size(); k++) {
        float dist= (mousePos-myMovies[k]->videoPosition.pos).length();
        myMovies[k]->scaleSizeVideo(80+500*exp(-dist*dist/50000));
        myMovies[k]->zCoordinate.pos.x=1-sqrt(dist*dist)/50;
        
        // speed of the video:
        //        if (sqrt(dist*dist)<500) {
        //        float speed =3*exp(-dist*dist/3000);
        //        myMovies[k]->setSpeed(speed);
        //        }
        
        // speed of the video:
        if (sqrt(dist*dist)<1000) {
            float speed =1-1*exp(-dist*dist/10000);
            //   myMovies[k]->setSpeed(speed);
        }
        
    }
    
    //	if(!frameByframe){
    //		int width = ofGetWidth();
    //		float pct = (float)x / (float)width;
    //		float speed = (2 * pct - 1) * 5.0f;
    //		 for (int k=0; k<myMovies.size(); k++) myMovies[k]->setSpeed(speed);
    //	}
        
    }
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    if(!frameByframe) {
    
    int width = ofGetWidth();
	for (int k=0; k<myMovies.size(); k++) {
        float pct = (float)x / (float)width*myMovies[k]->getDuration();
        myMovies[k]->videoHeadAnchor.setPos(pct, 0);
    }
    
    //
    //    if(!frameByframe){
    //		int width = ofGetWidth();
    //		float pct = (float)x / (float)width;
    //		 for (int k=0; k<myMovies.size(); k++) myMovies[k]->setPosition(pct);
    //	}
        
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    //	if(!frameByframe){
    //		 for (int k=0; k<myMovies.size(); k++) myMovies[k]->setPaused(true);
    //	}
    
    //    ofVec2f mousePos(x, y);
    //    for (int k=0; k<myMovies.size(); k++) {
    //        float dist= (mousePos-myMovies[k]->videoPosition.pos).length();
    //        myMovies[k]->scaleSizeVideo(80+500*exp(-dist*dist/50000));
    //        myMovies[k]->zCoordinateAnchor=1-sqrt(dist*dist)/50;
    //
    //        // speed of the video:
    //        //        if (sqrt(dist*dist)<500) {
    //        //        float speed =3*exp(-dist*dist/3000);
    //        //        myMovies[k]->setSpeed(speed);
    //        //        }
    //
    //        // speed of the video:
    //        if (sqrt(dist*dist)<1000) {
    //            float speed =1-1*exp(-dist*dist/10000);
    //            //   myMovies[k]->setSpeed(speed);
    //        }
    //        
    //    }
    
    
}


//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    //	if(!frameByframe){
    //		 for (int k=0; k<myMovies.size(); k++) myMovies[k]->setPaused(false);
    //	}
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
