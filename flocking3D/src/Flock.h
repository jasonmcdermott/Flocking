#pragma once
#include "Boid.h"
#include "Boundary.h"
#include "ofxBody.h"

class Flock {
public:

    vector <Boid> boids;
    float col;
    int boidCount;
    bool    avoidWalls, interactWithBodies, reset;
    float separationF, cohesionF, alignF, dragF, personalSpace, perception, maxForce, maxSpeed;
    
    Flock() {
        
    }
    
    void update(Boundary outer, vector <ofxBody> bodies) {
        for(int i=0;i<boids.size();i++) {
            boids[i].run(boids, outer, bodies);
        }
    }
    
    void draw() {
        glBegin(GL_TRIANGLES);
        for(int i=0;i<boids.size();i++) {
            boids[i].render();
        }
        glEnd();
    }
    
    void addBoids(int n, Boundary outer, ofVec3f centre) {
        for(int i=0;i<n;i++) {
            boids.push_back(Boid(boidCount, outer, centre));
            boidCount ++;
        }
    }
    
    void updateGUI() {
        for(int i=0;i<boids.size();i++) {
            boids[i].avoidWalls = avoidWalls;
            boids[i].separationF = separationF;
            boids[i].cohesionF = cohesionF;
            boids[i].alignF = alignF;
            boids[i].perception = perception;
            boids[i].personalSpace = personalSpace;
            boids[i].maxForce = maxForce;
            boids[i].maxSpeed = maxSpeed;
            boids[i].reset = reset;
            boids[i].interactWithBodies = interactWithBodies;
        }
    }
    
};