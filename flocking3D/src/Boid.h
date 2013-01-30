#pragma once
#include "Boundary.h"
#include "ofxBody.h"
#include <math.h>

class Boid {
public:
    
    ofVec3f pos, vel, acc,ali,coh,sep, origin, bias;
    float personalSpace, boidPerception, predPerception;
    bool avoidWalls = true;
    ofVec3f other, dist;
    float c;
    float boidSpeed = 3;
    float predSpeed = 6;
    float boidForce = .5;
    float predForce = 0.9;
    float evadeForce = 1;
    float h;
    float sc = 3;
    float flap = 0;
    float t = 0;
    int ID;
    int randomPrey;
    string a;
    ofVec3f outerTemp;
    float separationF, alignF, cohesionF, dragF, mass;
    bool isDead, interactWithBodies, interactWithPredators;
    bool reset = false, updatePrey;
    int age, type, prey;
    ofFloatColor	boidColor;
    
    Boid() {
        
    }
    
    Boid(int ID_, Boundary outer, ofVec3f centre_, int type_) {
        origin.set(centre_);
        ID = ID_;
        type = type_;
        initBoid(outer);
    }

    void initBoid(Boundary outer) {
        origin.set(origin.x + ofRandom(-outer.halfLength,outer.halfLength),origin.y + ofRandom(-outer.halfLength,outer.halfLength),origin.z+ ofRandom(-outer.halfLength,outer.halfLength));
        isDead = false;
        age = 0;
        mass = 1;
        pos.set(origin);
        vel.set(0,0,0);
        acc.set(0,0,0);
        bias.set(0,0,0);
        personalSpace = 15;
        c = 255;
        separationF = 1;
        cohesionF = 1;
        dragF = 0.95;
        a = "";

        if (type == 0) {
            alignF = 1;
            interactWithBodies = true;
            boidPerception = 50;
            sc = 3;
        } else {
            alignF = 0;
            prey = ofRandom(0,ID);
            interactWithBodies = false;
            predPerception = 500;
            sc = 10;
        }
    }
    
    void run(vector <Boid> boids, Boundary outer, vector <ofxBody> bodies) {
        if (reset == true) {
            initBoid(outer);
        }
        if (isDead != true) {
            age ++;
            if (type == 0) {
                if (interactWithBodies == true) {
                    interactingBodies(bodies);
                }
            }
            if (interactWithPredators) {
                if (type == 1) {
    //                if (updatePrey == true) {
                        choosePrey(boids);
    //                    updatePrey = false;
    //                }
                }
            }
            if (avoidWalls) {
                avoidBounds(outer);
                killStrays(outer);
            } else {
                checkBounds(outer);
            }
            flock(boids);
            move();
        }
    }
    
    
    void interactingBodies(vector <ofxBody> bodies) {
        for (int i=0;i<bodies.size();i++) {
            if (bodies[i].charge == 0) {
                ofVec3f att;
                ofVec3f force;
                att.set(bodies[i].pos);
                force = att - pos;
                float dist = force.length();
                float inverseSquare = (mass * bodies[i].mass) / (dist * dist);
                float minDist = (sc + bodies[i].mass);
                if (dist > minDist) {
                    force *= inverseSquare;
                    acc += force;
                } else if (dist < minDist) {
                    force *= inverseSquare;
                    force += -force;
                    acc += force;
                }
            }
            if (bodies[i].charge > 0) {
                ofVec3f rep;
                ofVec3f force;
                rep.set(bodies[i].pos);
                force = rep - pos;
                float dist = force.length();
                float inverseSquare = (mass * bodies[i].mass) / (dist * dist);
                if (dist < mass * 300) {
                    force *= inverseSquare;
                    acc += -force;
                }
            }
        }
    }

    void flock(vector <Boid> boids) {
        ali = alignment(boids);
        sep = separation(boids);
        coh = cohesion(boids);
        
        ali *= alignF;
        sep *= separationF;
        coh *= cohesionF;
        
        acc += ali;
        acc += sep;
        acc += coh;
        
        if (interactWithPredators) {
            if (type == 0) {
                fleePredators(boids);
            }
            if (type == 1) {
                trailPrey(boids);
            }
            gobble(boids);
        }
    }
    
    void choosePrey(vector <Boid> boids) {
        float closest = 500;
        for (int i=0;i<boids.size();i++) {
            if (boids[i].isDead == false && boids[i].type == 0) {
                ofVec3f distance;
                distance = boids[i].pos - pos;
                float dist = distance.length();
                if (dist < closest) {
                    prey = i;
                }
            }
        }
    }

    void trailPrey(vector <Boid> boids) {
        for (int i=0;i<boids.size();i++) {
            if (boids[prey].type == 0) {
                ofVec3f att;
                ofVec3f force;
                att.set(boids[prey].pos);
                force = att - pos;
                float dist = force.length();
                float inverseSquare = (mass * boids[prey].mass) / (dist * dist);
                float minDist = (sc + boids[prey].mass);
                if (dist > minDist) {
                    force *= inverseSquare;
                    acc += force;
                }
            }
        }
    }
    
    void fleePredators(vector <Boid> boids) {
        for (int i=0;i<boids.size();i++) {
            if (boids[i].ID != ID && boids[i].type == 1) {
                ofVec3f rep;
                ofVec3f force;
                rep.set(boids[i].pos);
                force = rep - pos;
                float dist = force.length();
                float inverseSquare = (mass * boids[i].mass) / (dist * dist);
                if (dist < personalSpace * 10) {
                    force *= inverseSquare;
                    force *= evadeForce;
                    acc += -force;
                }
            }
        }
    }
    
    void gobble(vector <Boid> boids) {
        for (int i=0;i<boids.size();i++) {
            if (isDead == false && type == 0 && boids[i].type == 1) {
                ofVec3f distance;
                distance = boids[i].pos - pos;
                float dist = distance.length();
                if (dist < sc*3) {
                    boids[i].updatePrey = true;
                    isDead = true;
                }
            }
        }
    }

    void move() {
        acc += bias;
        vel += acc;
        if (type == 0){
            vel.limit(boidSpeed);
        }
        if (type == 1) {
            vel.limit(predSpeed);
        }
        vel *= dragF; // add a drag force into the mix.
        pos += vel; //add velocity to position
        acc *= 0; //reset acceleration
    }
    
    
    void render() {
        if (isDead != true) {
            ofPushMatrix();
            ofTranslate(pos.x,pos.y,pos.z);
            if (type == 0) {
                ofSetColor(boidColor);
            } else if (type == 1) {
                ofSetColor(200,0,0);
                ofDrawBitmapString(ofToString(type,0),5,10);                
            }
            ofSphere(0,0,0,sc);
//            ofBox(0,0,0,sc);

            ofPopMatrix();
        }
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
    
    ofVec3f steer(ofVec3f target,bool arrival) {
        ofVec3f steer;
        if(!arrival){
            steer.set(target);
            steer -= pos;
            // steer.set(PVector.sub(target,pos));
            // steering vector points towards target (switch target and pos for avoiding)
            steer.limit(boidForce); //limits the steering force to maxSteerForce
        } else {
//            PVector targetOffset = PVector.sub(target,pos);
//            float distance=targetOffset.mag();
//            float rampedSpeed = maxSpeed*(distance/100);
//            float clippedSpeed = min(rampedSpeed,maxSpeed);
//            PVector desiredVelocity = PVector.mult(targetOffset,(clippedSpeed/distance));
//            steer.set(PVector.sub(desiredVelocity,vel));
        }
        return steer;
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
    
    ofVec3f separation(vector <Boid> boids) {
        ofVec3f posSum;
        ofVec3f repulse;
        for(int i=0;i<boids.size();i++) {
            if (boids[i].ID != ID && boids[i].isDead != true) {
                other.set(boids[i].pos);
                dist.set(pos);
                dist -= other;
                float d = dist.length();
                if(type == boids[i].type && d > 0 && d <= personalSpace) {
                    repulse.set(dist);
                    repulse.normalize();
                    repulse /= d;
                    posSum += repulse;
                }
            }
        }
        return posSum;
    }
    
    ofVec3f alignment(vector <Boid> boids) {
        ofVec3f velSum;
        int count = 0;
        for(int i=0;i<boids.size();i++){
            if (boids[i].ID != ID && boids[i].isDead != true) {
                other.set(boids[i].pos);
                dist.set(pos);
                dist -= other;
                float d = dist.length();
                
                if (type == 0 && d > 0 && d <= boidPerception) {
                    velSum += boids[i].vel;
                    count++;
                }
                if (type == 1 && d > 0 && d <= predPerception) {
                    velSum += boids[i].vel;
                    count++;
                }
            }
        }
        if(count > 0) {
            velSum /= (float)count;
            velSum.limit(boidForce);
        }
        return velSum;
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
                
                if(type == 0 && d > 0 && d <= boidPerception) {
                    posSum += boids[i].pos;
                    count++;
                }
                if(type == 1 && d > 0 && d <= predPerception) {
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
        if (type == 0){
            steer.limit(boidForce);
        }
        if (type == 1) {
            steer.limit(predForce);
        }
    
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
        
    
    
    
};


