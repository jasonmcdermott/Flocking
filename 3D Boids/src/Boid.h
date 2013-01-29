#ifndef BOID_H
#define BOID_H
#include "ofVec3f.h"
#include "ofMain.h"

class Boid {
public:
    Boid();
    void update();
    void draw();
    void bounce();
    
    // steering behaviors
    void addForce(ofVec3f oForce);
    void subtractForce(ofVec3f oForce);
    void seek(ofVec3f oTarget); // go to a point
    void flee(ofVec3f oTarget); // flee from this target
    void arrive(ofVec3f oTarget); // arrive at this point.
    void pursue(Boid* pBoid);
    
    //void evade(Boid* pBoid);
    
    // getter|setters
    ofVec3f getPosition();
    ofVec3f getVelocity();
    
    void setPosition(ofVec3f oPos);
    void setMaxVelocity(float fMax);
    float getMaxVelocity();
    void setVelocity(ofVec3f oVel);
    
private:
    ofImage img;
    float max_force;
    float max_vel;
    float mass;
    float arrive_dist;
    ofVec3f force;
    ofVec3f pos;
    ofVec3f acc;
    ofVec3f vel;
    float xMinBounds, xMaxBounds, yMinBounds, yMaxBounds, zMinBounds, zMaxBounds;
    
};

#endif
