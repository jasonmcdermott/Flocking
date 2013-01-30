#include "testApp.h"



void testApp::setup(){
    ofSetFrameRate(60);
    tick = 0.0001;
    ofBackground(0);
    snapCounter = 0;
	memset(snapString, 0, 255);		// clear the string by setting all chars to 0
    
    
    personalSpace = 1;
    guiDraw = false;
    worldSize = 800;
    centre.set(ofGetWidth()/2,ofGetHeight()/2,0);
    cam.resetTransform();
    cam.setFov(60);
    cam.clearParent();
    cam.lookAt(centre);
//    cam.setPosition(ofGetWidth()/2, ofGetHeight()/2, worldSize*2);
//    cam.roll(90);
    camDraw = true;
    flock.reset = false;
    setupGUI();
    camPos.set(ofGetWidth()/2, ofGetHeight()/2, worldSize*2);
    

    bodies.push_back(ofxBody( ofVec3f(centre), 90, ATTRACT));
    outer.setDims(centre,worldSize);
    
    flock.addBoids(100, outer, centre,0);
    flock.addBoids(1, outer, centre, 1);
    
    gui.loadFromXML();
	
	gui.show();

}


void testApp::update(){
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    ofEnableAlphaBlending();
    time = time + tick;
    if (time > 1) {
        updateGUI();
        flock.update(outer, bodies);
        time = 0;
    }
    cam.setPosition(camPos.x, camPos.y, camPos.z);
    cam.lookAt(centre);
}


void testApp::draw(){
    if (camDraw) cam.begin(); cam.draw();

    if (drawBodies) {
        for (int i=0;i<bodies.size();i++) {
            bodies[i].draw();
        }
    }

    if (drawBounds) outer.draw();


//    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glEnableClientState      (GL_VERTEX_ARRAY);
    flock.draw();
    glDisableClientState   (GL_VERTEX_ARRAY);
    
    if (camDraw) cam.end();
    if (guiDraw) gui.draw();
    ofSetColor(255,200);
    ofDrawBitmapString("Hit 'g' to toggle GUI", 20,ofGetHeight()-20);
    snapFrame();
}

void testApp::updateGUI() {
    outer.length = worldSize;
    
//    flock.avoidWalls = avoidWalls;
//    flock.separationF = separationF;
//    flock.cohesionF = cohesionF;
//    flock.alignF = alignF;
//    flock.boidPerception = boidPerception;
//    flock.predPerception = predPerception;
//    flock.personalSpace = personalSpace;
//    flock.maxForce = maxForce;
//    flock.maxSpeed = maxSpeed;
//    flock.reset = reset;
//    flock.interactWithBodies = interactWithBodies;
//    flock.drawFlock = drawFlock;
//    flock.drawPreds = drawPreds;
    outer.updateGUI();
    flock.updateGUI();

}


void testApp::setupGUI() {
    gui.addTitle("Boids");

	gui.addSlider("Separation", flock.separationF, 0, 15);
	gui.addSlider("Cohesion", flock.cohesionF, 0, 5);
	gui.addSlider("Alignment", flock.alignF, 0, 5);
    gui.addSlider("Boid Perception", flock.boidPerception, 1, 500);
    gui.addSlider("PersonalSpace", flock.personalSpace, 1, 100);
    gui.addSlider("Boid Force", flock.boidForce, 0.1, 10);
    gui.addSlider("Boid Speed", flock.boidSpeed, 0.1, 10);
    gui.addSlider("Drag", flock.dragF, 0.1, 1);
    gui.addColorPicker("Boid Color", flock.boidColor);
    gui.addSlider("Evade Force", flock.evadeForce, 0.1, 35);
    gui.addToggle("Interact with Bodies", flock.interactWithBodies);
    gui.addToggle("Interact with Predators", flock.interactWithPredators);
    gui.addToggle("Avoid Walls", flock.avoidWalls);
    
    gui.addTitle("Predators").setNewColumn(true);
    gui.addSlider("Predator Speed",flock.predSpeed,0.1,10);
    gui.addSlider("Predator Force",flock.predForce,0.1,10);
    gui.addSlider("Predator Perception", flock.predPerception, 1, 500);
    
    gui.addTitle("World").setNewColumn(true);
    gui.addSlider("TimeSpeed",tick,0,1);
    gui.addSlider("Size",worldSize,100,2000);
    gui.addToggle("Draw Bounds", drawBounds);
    gui.addToggle("Draw Bodies", drawBodies);    
    gui.addToggle("Draw Flock", flock.drawFlock);
    gui.addToggle("Draw Predators", flock.drawPreds);
    gui.addToggle("Reset Boids", flock.reset);
    
    gui.addTitle("Camera").setNewColumn(true);
    gui.addSlider("Camera Position X", camPos.x, -2000, 2000);
    gui.addSlider("Camera Position Y", camPos.y, -2000, 2000);
    gui.addSlider("Camera Position Z", camPos.z, -2000, 2000);
}

void testApp::keyPressed(int key){
    if (key == 'g') {
        guiDraw = !guiDraw;
    }
}

void testApp::snapFrame() {
    if (ofGetFrameNum() % 900 == 0){
		img.grabScreen(0,0,ofGetWidth(),ofGetHeight());
		string fileName = "../../../../images/flocking3D_"+ofGetTimestampString()+".png";
		img.saveImage(fileName);
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

