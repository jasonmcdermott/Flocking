#pragma once
#include "ofVec3f.h"
#include "ofMain.h"
#include <vector>


class ofxBody{
public:
    
    ofVec3f pos;
    float mass, radius;
    int charge;
    
    ofxBody(ofVec3f loc_, float mass_, int charge_) {
        pos.set(loc_);
        mass = mass_;
        charge = charge_;
        radius = mass;
    }
    
    void update() {
        radius = mass;
    }
    
    void addMass(float m) {
        mass = mass + m;
    }
    
    void draw() {
        ofPushMatrix();
        ofTranslate(pos.x,pos.y,pos.z);
        ofSetColor(255);
        ofSphere(0,0,0,radius);
        ofPopMatrix();
    }
};




