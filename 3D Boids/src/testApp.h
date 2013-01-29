#pragma once

#include "ofMain.h"
#include <math.h>
#include "Boid.h"
#include "Boids.h"
#include "ofVec3f.h"
#include "ofxGrabCam.h"
#include "ofCamera.h"

#define MAX_BOIDS 1000

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
    void drawBox();
    
    Boid boidies[MAX_BOIDS];
    Boids* boids;
    char mode;
    ofxGrabCam      camera;

    ofEasyCam   eCam;
        ofNode lookat;
    ofVec3f target; // target used to seek, flee, arrive
 
    
    
    
    ofLight pointLight;
	ofLight spotLight;
	ofLight directionalLight;
	
	ofMaterial material;
    ofImage ofLogoImage;
	
	float radius;
	ofVec3f center;
	bool bShiny;
    bool bSmoothLighting;
	bool bPointLight, bSpotLight, bDirLight;
    bool bUseTexture;


};
