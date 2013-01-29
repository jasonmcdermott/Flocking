#pragma once

#include "ofMain.h"
#include "Flock.h"
#include "Boundary.h"
#include "ofxSimpleGuiToo.h"
#include "ofCamera.h"
#include "ofxBody.h"
#include "Predator.h"

#define ATTRACT 0
#define REPEL 1

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
    void setupGUI();
    void updateGUI();
    
    int initBoidNum;
    ofVec3f centre;
    Flock flock;
    Boundary outer;
    bool smoothEdges;
    float time, tick;
    
    bool guiDraw;

    
    ofCamera cam;

    vector <ofxBody> bodies;
    vector <Predator> predators;
    bool	reset, avoidWalls, interactWithBodies;
    float separationF, cohesionF, alignF, dragF, personalSpace, perception, maxForce, maxSpeed;
    float worldSize;
    bool camDraw;
    
};
