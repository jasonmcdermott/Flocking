#pragma once

#include "ofMain.h"
#include "Flock.h"
#include "Boundary.h"
#include "ofxSimpleGuiToo.h"

class testApp : public ofBaseApp{
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
    void updateGUI();
    
    int initBoidNum;
    ofVec3f centre;
    Flock flock;
    Boundary outer;
    bool smoothEdges;
    float time, tick;
    
    bool guiDraw;
    bool	avoidWalls;
//    bool	myBool2;
//    bool	myBool3;
//    bool	myBool4;
//    bool	myBool5;
//    bool	myBool6;
//    bool	myBool7;
//    bool	myBool8;
//    bool	myBool9;
    
    float separationF, cohesionF, alignF, dragF, personalSpace, perception, maxForce, maxSpeed;
    bool reset;
    
};
