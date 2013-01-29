#pragma once
#include "Boundary.h"

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
    float separationF, alignF, cohesionF, dragF;
    bool reset, isDead;
    int age;
    
    Boid() {
        
    }
    
    Boid(int ID_, Boundary outer, ofVec3f centre_) {
        origin.set(centre_);
        origin.set(origin.x + ofRandom(-outer.halfLength/2,outer.halfLength/2),origin.y + ofRandom(-outer.halfLength/2,outer.halfLength/2),origin.z+ ofRandom(-outer.halfLength/2,outer.halfLength/2));
        ID = ID_;

        initBoid();
    }

    void initBoid() {
        isDead = false;
        age = 0;
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
            initBoid();
        }
        if (isDead != true) {
            a = "";
            age ++;
            if (avoidWalls) {
                avoidBounds(outer);
                killStrays(outer);
            } else {
                checkBounds(outer);
            }
    //            ageBoids();
    //        ofVec3f avo;
    //        avo = avoidBounds(outer);
    //        avo *= 2;
    //        acc += avo;
    //        a += " av is: " + ofToString(pos.x,2) + " , " + ofToString(pos.y,2) + " , " + ofToString(pos.z,2) +  " \n";
    //        a += " av is: " + ofToString(avLeft.x,2) + " , " + ofToString(avLeft.y,2) + " , " + ofToString(avLeft.z,2) +  " \n";


            flock(boids);
            move();
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

            
            ofSetColor(c);
            ofBox(0,0,0,sc);

            
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
    
    //avoid. If weight == true avoidance vector is larger the closer the boid is to the target
    ofVec3f avoid(ofVec3f target, bool weight) {
        ofVec3f steer;
        steer.set(pos);
        steer -= target;
        float distance = steer.length();
        //steering vector points away from target
//        steer.set(-steer);
        if (weight) {
            steer *= (1 / (distance * distance));
        }
        steer *= 15;
//        steer.limit(maxSteerForce); //limits the steering force to maxSteerForce
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


