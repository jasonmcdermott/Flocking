#include "testApp.h"



void testApp::setup(){
    ofSetFrameRate(60);
    tick = 0.01;
    ofBackground(0);
    
    personalSpace = 1;
    guiDraw = false;
//    gui.addTitle("A group");
	gui.addToggle("Avoid Walls", avoidWalls);
	gui.addSlider("Separation", separationF, 0, 5);
	gui.addSlider("Cohesion", cohesionF, 0, 5);
	gui.addSlider("Alignment", alignF, 0, 5);
    gui.addSlider("Perception", perception, 1, 500);
    gui.addSlider("PersonalSpace", personalSpace, 1, 100);
    gui.addSlider("Turning Force", maxForce, 0.1, 5);
    gui.addSlider("Speed", maxSpeed, 0.1, 5);
    gui.addSlider("Drag", dragF, 0.1, 1);
    gui.addToggle("Reset Boids", reset);

    
    //	gui.addSlider("myInt1", myInt1, 100, 200);
//	gui.addComboBox("box1", box1, 12, NULL);
//	gui.addButton("Randomize Background", randomizeButton);
//	gui.addColorPicker("BG Color", aColor);

    
    
    centre.set(ofGetWidth()/2,ofGetHeight()/2,0);
    outer.setDims(centre,200);
    
    
    flock.addBoids(200, outer, centre);
    
    gui.loadFromXML();
	
	gui.show();

}


void testApp::update(){
    time ++;
    if (time > tick) {
        updateGUI();
        flock.update(outer);
        if (reset == true) {
            reset = false;
        }
        time = 0;
        
    }
}


void testApp::draw(){
    
    outer.draw();
    flock.draw();
    if (guiDraw) gui.draw();

}

void testApp::updateGUI() {
    flock.avoidWalls = avoidWalls;
    flock.separationF = separationF;
    flock.cohesionF = cohesionF;
    flock.alignF = alignF;
    flock.perception = perception;
    flock.personalSpace = personalSpace;
    flock.maxForce = maxForce;
    flock.maxSpeed = maxSpeed;
    flock.reset = reset;
    flock.updateGUI();
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