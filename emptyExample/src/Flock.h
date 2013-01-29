#pragma once
#include "Boid.h"
#include "Boundary.h"

class Flock {
public:

    vector <Boid> boids;
    float col;
    int boidCount;
    
    Flock() {
        
    }
    
    void update(Boundary outer) {
        for(int i=0;i<boids.size();i++) {
            boids[i].run(boids, outer);
        }
    }
    
    void draw() {
        for(int i=0;i<boids.size();i++) {
            boids[i].render();
        }
    }
    
    void addBoids(int n, Boundary outer, ofVec3f centre) {
        for(int i=0;i<n;i++) {
            boids.push_back(Boid(boidCount, outer, centre));
            boidCount ++;
        }
    }
    
};