//
//  Predator.h
//  flocking3D
//
//  Created by Jason McDermott on 29/01/13.
//
//

#ifndef flocking3D_Predator_h
#define flocking3D_Predator_h





#pragma once
#include "ofVec3f.h"
#include "ofMain.h"
#include <vector>


class Predator{
public:
    
    ofVec3f pos, vel, acc,ali,coh,sep, origin;
    float personalSpace, perception;
    bool avoidWalls = true;
    ofVec3f other, dist;
    float c;
    float maxSpeed = 3;
    float maxForce = .5;
    float h;
    float sc = 10;
    float flap = 0;
    float t = 0;
    int ID;
    string a;
    ofVec3f outerTemp;
    float separationF, alignF, cohesionF, dragF, mass;
    bool reset, isDead;
    int age;
    
    Predator() {
        
    }
    
    Predator(int ID_, Boundary outer, ofVec3f centre_) {
        origin.set(centre_);
        origin.set(origin.x + ofRandom(-outer.halfLength,outer.halfLength),origin.y + ofRandom(-outer.halfLength,outer.halfLength),origin.z+ ofRandom(-outer.halfLength,outer.halfLength));
        ID = ID_;
        initPredator();
    }
    
    void initPredator() {
        isDead = false;
        age = 0;
        mass = 1;
        pos.set(origin);
        vel.set(0,0,0);
        acc.set(0,0,0);
        personalSpace = 15;
        perception = 50;
        c = 255;
        alignF = 1;
        separationF = 1;
        cohesionF = 1;
        dragF = 0.95;
    }
    
    void run(vector <Boid> boids, Boundary outer) {
        if (reset == true) {
            initPredator();
        }
        if (isDead != true) {
            age ++;
            chaseFlock(boids);
            if (avoidWalls) {
                avoidBounds(outer);
                killStrays(outer);
            } else {
                checkBounds(outer);
            }
            move();
        }
    }
    
    void chaseFlock(vector <Boid> boids) {
        coh = cohesion(boids);
        coh *= cohesionF;
        acc += coh;
    }
    
    void move() {
        vel += acc; //add acceleration to velocity
        vel.limit(maxSpeed); //make sure the velocity vector magnitude does not exceed maxSpeed
        vel *= dragF; // add a drag force into the mix.
        pos += vel; //add velocity to position
        acc *= 0; //reset acceleration
    }
    
    void checkBounds(Boundary outer) {
        if(pos.x > outer.right.x) pos.x = outer.left.x;
        if(pos.x < outer.left.x) pos.x = outer.right.x;
        if(pos.y < outer.top.y) pos.y = outer.bottom.y;
        if(pos.y > outer.bottom.y) pos.y = outer.top.y;
        if(pos.z > outer.front.z) pos.z = outer.back.z;
        if(pos.z < outer.back.z) pos.z = outer.front.z;
    }
    
    void avoidBounds(Boundary outer) {
        ofVec3f left(outer.left.x,pos.y,pos.z);
        ofVec3f right(outer.right.x,pos.y,pos.z);
        ofVec3f top(pos.x,outer.top.y,pos.z);
        ofVec3f bottom(pos.x,outer.bottom.y,pos.z);
        ofVec3f front(pos.x,pos.y,outer.front.z);
        ofVec3f back(pos.x,pos.y,outer.back.z);
        
        ofVec3f avLeft(avoid(left,true));
        ofVec3f avRight(avoid(right,true));
        ofVec3f avTop(avoid(top,true));
        ofVec3f avBottom(avoid(bottom,true));
        ofVec3f avFront(avoid(front,true));
        ofVec3f avBack(avoid(back,true));
        
        acc += avLeft;
        acc += avRight;
        acc += avTop;
        acc += avBottom;
        acc += avFront;
        acc += avBack;
    }
    
    ofVec3f avoid(ofVec3f target, bool weight) {
        ofVec3f steer;
        steer.set(pos);
        steer -= target;
        float distance = steer.length();
        if (weight) {
            steer *= (1 / (distance * distance));
        }
        steer *= 15;
        return steer;
    }
    
    ofVec3f cohesion(vector <Boid> boids) {
        ofVec3f posSum;
        ofVec3f steer;
        posSum.set(origin);
        steer.set(origin);
        int count = 0;
        for(int i=0;i<boids.size();i++) {
            if (boids[i].ID != ID && boids[i].isDead != true) {
                other.set(boids[i].pos);
                dist.set(pos);
                dist -= other;
                float d = dist.length();
                if(d > 0 && d <= perception) {
                    posSum += boids[i].pos;
                    count++;
                }
            }
        }
        if(count>0) {
            posSum /= (float)count;
        }
        steer.set(pos);
        steer -= posSum;
        steer.limit(maxForce);
        return steer;
    }
    
    
    void killStrays(Boundary outer) {
        if (pos.x > outer.right.x || pos.x < outer.left.x) {
            isDead = true;
        }
        if (pos.y > outer.bottom.y || pos.y < outer.top.y) {
            isDead = true;
        }
        if (pos.z > outer.front.z || pos.z < outer.back.z) {
            isDead = true;
        }
    }
    
    void render() {
        if (isDead != true) {
            ofPushMatrix();
            ofTranslate(pos.x,pos.y,pos.z);
            
            
            ofSetColor(c);
            ofBox(0,0,0,sc);
            //            ofPoint(0,0,0);
            //            glutSolidSphere(sc, 60, 60);
            
            
            //        ofDrawBitmapString(a,pos.x+5,pos.y+5,pos.z);
            //        ofRotateY(atan2(-vel.z,vel.x));
            //        ofRotateZ(asin(vel.y/vel.length()));
            //        ofSetColor(255);
            //        //draw bird
            //        glBegin(GL_TRIANGLE_FAN);
            //        glVertex3f(3*sc,0,0);
            //        glVertex3f(-3*sc,2*sc,0);
            //        glVertex3f(-3*sc,-2*sc,0);
            //
            //        glVertex3f(3*sc,0,0);
            //        glVertex3f(-3*sc,2*sc,0);
            //        glVertex3f(-3*sc,0,2*sc);
            //
            //        glVertex3f(3*sc,0,0);
            //        glVertex3f(-3*sc,0,2*sc);
            //        glVertex3f(-3*sc,-2*sc,0);
            //
            //        /* wings
            //         vertex(2*sc,0,0);
            //         vertex(-1*sc,0,0);
            //         vertex(-1*sc,-8*sc,flap);
            //
            //         vertex(2*sc,0,0);
            //         vertex(-1*sc,0,0);
            //         vertex(-1*sc,8*sc,flap);
            //         */
            //        
            //        glVertex3f(-3*sc,0,2*sc);
            //        glVertex3f(-3*sc,2*sc,0);
            //        glVertex3f(-3*sc,-2*sc,0);
            //        glEnd();
            ofPopMatrix();
        }
    }
};







#endif
