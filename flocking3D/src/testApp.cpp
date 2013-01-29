#include "testApp.h"



void testApp::setup(){
    ofSetFrameRate(60);
    tick = 0.01;
    ofBackground(0);

    centre.set(ofGetWidth()/2,ofGetHeight()/2,0);
    outer.setDims(centre,1000);
    
    
    flock.addBoids(200, outer, centre);
}


void testApp::update(){
    time ++;
    if (time > tick) {
        flock.update(outer);
        time = 0;
    }
}


void testApp::draw(){
    outer.draw();
    flock.draw();
}


void testApp::keyPressed(int key){}
void testApp::keyReleased(int key){}
void testApp::mouseMoved(int x, int y){}
void testApp::mouseDragged(int x, int y, int button){}
void testApp::mousePressed(int x, int y, int button){}
void testApp::mouseReleased(int x, int y, int button){}
void testApp::windowResized(int w, int h){}
void testApp::gotMessage(ofMessage msg){}
void testApp::dragEvent(ofDragInfo dragInfo){}