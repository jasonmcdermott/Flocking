#include "testApp.h"



void testApp::setup(){
    ofSetFrameRate(60);
    tick = 0.01;
    ofBackground(0);
    
    
    personalSpace = 1;
    guiDraw = false;
    worldSize = 400;
    
    cam.resetTransform();
    cam.setFov(60);
    cam.clearParent();
    cam.setPosition(ofGetWidth()/2, ofGetHeight()/2, worldSize*2);
    cam.roll(90);
    camDraw = true;

    


    setupGUI();
    
    
    centre.set(ofGetWidth()/2,ofGetHeight()/2,0);
    bodies.push_back(ofxBody( ofVec3f(centre), 90, ATTRACT));
    outer.setDims(centre,worldSize);
    
    
    flock.addBoids(200, outer, centre);
    
    gui.loadFromXML();
	
	gui.show();

}


void testApp::update(){
    glEnable(GL_DEPTH_TEST);
    time ++;
    if (time > tick) {
        updateGUI();
        flock.update(outer, bodies);
        if (reset == true) {
            reset = false;
        }
        time = 0;
        
    }
}


void testApp::draw(){
    // activate camera
    if (camDraw) {
        cam.begin();
        cam.draw();
    }

	// draw world axis
	ofDrawAxis(100);
    for (int i=0;i<bodies.size();i++) {
//        bodies[i].draw();
    }
    outer.draw();
    flock.draw();
    if (camDraw) {
        cam.end();
    }
    if (guiDraw) gui.draw();

}

void testApp::updateGUI() {
    outer.length = worldSize;
    
    flock.avoidWalls = avoidWalls;
    flock.separationF = separationF;
    flock.cohesionF = cohesionF;
    flock.alignF = alignF;
    flock.perception = perception;
    flock.personalSpace = personalSpace;
    flock.maxForce = maxForce;
    flock.maxSpeed = maxSpeed;
    flock.reset = reset;

    outer.updateGUI();
    flock.updateGUI();

}


void testApp::setupGUI() {
    gui.addTitle("Boids");
	gui.addToggle("Avoid Walls", avoidWalls);
	gui.addSlider("Separation", separationF, 0, 15);
	gui.addSlider("Cohesion", cohesionF, 0, 5);
	gui.addSlider("Alignment", alignF, 0, 5);
    gui.addSlider("Perception", perception, 1, 500);
    gui.addSlider("PersonalSpace", personalSpace, 1, 100);
    gui.addSlider("Turning Force", maxForce, 0.1, 5);
    gui.addSlider("Speed", maxSpeed, 0.1, 5);
    gui.addSlider("Drag", dragF, 0.1, 1);
    gui.addToggle("Reset Boids", reset);
    
    gui.addTitle("World").setNewColumn(true);
    gui.addSlider("Size",worldSize,100,1000);
    
    
    //	gui.addSlider("myInt1", myInt1, 100, 200);
    //	gui.addComboBox("box1", box1, 12, NULL);
    //	gui.addButton("Randomize Background", randomizeButton);
    //	gui.addColorPicker("BG Color", aColor);
    

}

void testApp::keyPressed(int key){
    if (key == 'g') {
        guiDraw = !guiDraw;
    }

}
void testApp::keyReleased(int key){}
void testApp::mouseMoved(int x, int y){}
void testApp::mouseDragged(int x, int y, int button){}
void testApp::mousePressed(int x, int y, int button){}
void testApp::mouseReleased(int x, int y, int button){}
void testApp::windowResized(int w, int h){}
void testApp::gotMessage(ofMessage msg){}
void testApp::dragEvent(ofDragInfo dragInfo){}