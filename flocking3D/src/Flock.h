#pragma once
#include "Boid.h"
#include "Boundary.h"
#include "ofxBody.h"


class Flock {
public:

    vector <Boid> boids;
    float col;
    int boidCount;
    bool reset = false;
    bool avoidWalls, interactWithBodies, drawBodies, drawFlock, drawPreds, interactWithPredators;
    float separationF, cohesionF, alignF, dragF, personalSpace, boidPerception, predPerception, predSpeed, predForce, boidForce, boidSpeed, evadeForce;
    ofFloatColor	boidColor;
    bool renderVA = true, flock, drawBounds;
    ofVec3f circleLocation;
    float circleRadius;
    
    Flock() {
        circleLocation.set(0,0,0);
    }
    
    void update(Boundary outer, vector <ofxBody> bodies) {
        for(int i=0;i<boids.size();i++) {
            boids[i].run(boids, outer, bodies);
        }
        if (reset == true) {
            reset = false;
        }

        if (interactWithPredators) {
//            for (int i=0;i<boids.size();i++) {
//                if (boids[i].type == 1) {
//                    ofVec3f mid;
//                    ofVec3f steer;
//                    mid.set(findBoidsMidst());
//                    steer.set(boids[i].pos);
//                    steer -= mid;
//                    steer.limit(predForce);
//                    boids[i].acc += -steer;
//                }
//            }
        }
    }
    
    int countLiveBoids() {
        int liveBoidCount = 0;
        for (int i=0;i<boids.size();i++) {
            if (boids[i].isDead == false) {
                liveBoidCount ++;
            }
        }
        return liveBoidCount;
    }
    
    ofVec3f findBoidsMidst() {
        ofVec3f steer;
        steer.set(0,0,0);
        int count = 0;
        for(int i=0;i<boids.size();i++) {
            if (boids[i].type == 0 && boids[i].isDead != true) {
                steer += boids[i].pos;
            }
        }
        int c = countLiveBoids();
        steer /= c;
        return steer;
    }
    
    void draw() {

        for(int i=0;i<boids.size();i++) {
            if (drawFlock == true && boids[i].type == 0) {
                boids[i].render();
            }
            if (drawPreds == true && boids[i].type == 1) {
                boids[i].render();
            }
        }
        
        if (drawBounds) {
            ofNoFill();
            ofSetColor(255);
            ofSphere(circleLocation,circleRadius*0.55);
        }

    }
    
    void addBoids(int n, Boundary outer, ofVec3f centre, int type_) {
        for(int i=0;i<n;i++) {
            boids.push_back(Boid(boidCount, outer, centre, type_));
            boidCount ++;
        }
    }
    
    void updateGUI() {
        for(int i=0;i<boids.size();i++) {
            boids[i].avoidWalls = avoidWalls;
            boids[i].separationF = separationF;
            boids[i].cohesionF = cohesionF;
            boids[i].alignF = alignF;
            boids[i].boidPerception = boidPerception;
            boids[i].predPerception = predPerception;
            boids[i].personalSpace = personalSpace;
            boids[i].boidForce = boidForce;
            boids[i].boidSpeed = boidSpeed;
            boids[i].predSpeed = predSpeed;
            boids[i].boidForce = boidForce;
            boids[i].evadeForce = evadeForce;
            boids[i].reset = reset;
            boids[i].interactWithBodies = interactWithBodies;
            boids[i].interactWithPredators = interactWithPredators;
            boids[i].boidColor = boidColor;
            boids[i].renderVA = renderVA;
            boids[i].circleRadius = circleRadius;
            boids[i].flock = flock;
            boids[i].circleLocation.set(circleLocation);
        }
    }
    
};