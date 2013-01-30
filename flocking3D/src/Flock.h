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
    
    Flock() {
        
    }
    
    void update(Boundary outer, vector <ofxBody> bodies) {
        for(int i=0;i<boids.size();i++) {
            boids[i].run(boids, outer, bodies);
        }
        if (reset == true) {
            reset = false;
        }
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
        }
    }
    
};