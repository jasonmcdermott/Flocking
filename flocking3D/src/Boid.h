#pragma once
#include "Boundary.h"
#include "ofxBody.h"
#include <math.h>
#define X .525731112119133606
#define Z .850650808352039932

class Boid {
public:
    
    ofVec3f pos, vel, acc,ali,coh,sep, origin;
    float personalSpace, perception;
    bool avoidWalls = true;
    ofVec3f other, dist;
    float c;
    float maxSpeed = 3;
    float maxForce = .5;
    float h;
    float sc = 3;
    float flap = 0;
    float t = 0;
    int ID;
    string a;
    ofVec3f outerTemp;
    float separationF, alignF, cohesionF, dragF, mass;
    bool reset, isDead, interactWithBodies;
    int age;
    
    Boid() {
        
    }
    
    Boid(int ID_, Boundary outer, ofVec3f centre_) {
        origin.set(centre_);
        origin.set(origin.x + ofRandom(-outer.halfLength,outer.halfLength),origin.y + ofRandom(-outer.halfLength,outer.halfLength),origin.z+ ofRandom(-outer.halfLength,outer.halfLength));
        ID = ID_;

        initBoid();
    }

    void initBoid() {
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
        interactWithBodies = true;
    }
    
    void run(vector <Boid> boids, Boundary outer, vector <ofxBody> bodies) {
        if (reset == true) {
            initBoid();
        }
        if (isDead != true) {
            a = "";
            age ++;
            if (interactWithBodies == true) {
                interactingBodies(bodies);
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
            if (bodies[i].charge == 0) {             // be attracted to global attractors
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
        coh = cohesion(boids);
        sep = seperation(boids);
        
        ali *= alignF;
        coh *= cohesionF;
        sep *= separationF;
        
        acc += ali;
        acc += coh;
        acc += sep;
        
    }
    
    void move() {
        vel += acc; //add acceleration to velocity
        vel.limit(maxSpeed); //make sure the velocity vector magnitude does not exceed maxSpeed
        vel *= dragF; // add a drag force into the mix.
        pos += vel; //add velocity to position
        acc *= 0; //reset acceleration
    }
    
    
    void render() {
        if (isDead != true) {
            ofPushMatrix();
            ofTranslate(pos.x,pos.y,pos.z);

            
            ofSetColor(255);
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
            steer.limit(maxForce); //limits the steering force to maxSteerForce
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
    
    ofVec3f seperation(vector <Boid> boids) {
        ofVec3f posSum;
        ofVec3f repulse;
        for(int i=0;i<boids.size();i++) {
            if (boids[i].ID != ID && boids[i].isDead != true) {
                other.set(boids[i].pos);
                dist.set(pos);
                dist -= other;
                float d = dist.length();
                if(d > 0 && d <= personalSpace) {
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
                if(d > 0 && d <= perception) {
                    velSum += boids[i].vel;
                    count++;
                }
            }
        }
        if(count > 0) {
            velSum /= (float)count;
            velSum.limit(maxForce);
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
    
    void ageBoids() {
        if (age > 100) {
            isDead = true;
        }
    }
    
    
    
    
};


