#include "Boid.h"

Boid::Boid() {
	pos.set(300.0f,300.0f,0.0f);
	acc.set(0.4,0.4,0.0f);
	vel.set(ofRandom(0.1f,40.0f), ofRandom(0.1f, 40.0f), ofRandom(0.1f, 40.0f));
    
    //max_vel.set(20.0f, 20.0f, 0.0f);
	max_vel = 50.0f;
	mass = 1.0f;
    
	// steering forces.
	max_force = 5.0f;
	force.set(0.0f, 0.0f, 0.0f);
    
	// arrive threshold
	arrive_dist = 50.0f;
    
//	img.loadImage("alpha.png");
    

    xMinBounds = 0;
    xMaxBounds = ofGetWidth();
    yMinBounds = 0;
    yMaxBounds = ofGetHeight();
    zMinBounds = 0;
    zMaxBounds = 800;
    
}

void Boid::update() {
	force.limit(max_force);
	vel += force;
    
	vel.limit(max_vel);
	pos += vel;
	//force = 0;
}


void Boid::draw() {

    ofPushMatrix();
    
    ofTranslate(pos.x, pos.y, pos.z);

    ofPopMatrix();

//	ofSetLineWidth(3);
    
//	float arrow_len = 20;
//	ofVec3f arrow_end = pos +(vel.normalized() * arrow_len);
////	ofFill();
//	ofEnableSmoothing();
//	ofSetColor(0);
//	ofLine(pos.x, pos.y,pos.z, arrow_end.x, arrow_end.y,arrow_end.z);
//    ofBox(arrow_end.x,arrow_end.y,arrow_end.z,10);
    
	ofSetColor(255,0,0);
    ofSphere(pos.x,pos.y,pos.z,10);
//	ofCircle(pos.x, pos.y, 10);
//	ofNoFill();
    
//	ofEnableSmoothing();
    
//	ofSetColor(255,0,0);
	//ofSetLineWidth(20);
//	ofCircle(pos.x, pos.y, 10);
    
    
//	ofSetColor(0);
//	ofSetLineWidth(3);
//	ofCircle(pos.x, pos.y, 12);
    
}

void Boid::bounce() {
	if (pos.x > xMaxBounds) {
		pos.x = xMaxBounds;
		vel.x *= -1;
	}
	else if (pos.x < xMinBounds) {
		pos.x = xMinBounds;
		vel.x *= -1;
	}
	if (pos.y > yMaxBounds) {
		pos.y = yMaxBounds;
		vel.y *= -1;
	}
	else if (pos.y < yMinBounds) {
		pos.y = yMinBounds;
		vel.y *= -1;
	}
	if (pos.z < -zMinBounds) {
		pos.z = -zMinBounds;
		vel.z *= -1;
	}
	else if (pos.z > zMaxBounds) {
		pos.z = zMaxBounds;
		vel.z *= -1;
	}
}


//void Boid::bounce() {
//	if (pos.x > xMaxBounds) {
//		pos.x = xMinBounds;
//	}
//	else if (pos.x < xMinBounds) {
//		pos.x = xMaxBounds;
//	}
//	if (pos.y > yMaxBounds) {
//		pos.y = yMinBounds;
//	}
//	else if (pos.y < yMinBounds) {
//		pos.y = yMaxBounds;
//	}
//	if (pos.z < -zMinBounds) {
//		pos.z = -zMaxBounds;
//	}
//	else if (pos.z > zMaxBounds) {
//		pos.z = zMinBounds;
//	}
//}

// steering behaviors

void Boid::seek(ofVec3f oTarget) {
	// This is how it works.
	// 1. We calculate the change in position which would bring
	// use directly to the target. This is "desired_vel". Though we don't
	// want to jump directly to this target, but slowly move to it.
	ofVec3f desired_vel = oTarget - pos;
    
	// 2. Therefore we want to move to this position at the maximum
	// allowed speed. We do this by normalizing the vector and then
	// multiplying it with the maximum allowed speed. This is exactly
	// what ofxVec3f.scale() does.
	desired_vel.scale(max_vel);
    
	// 3. Now we have the maximum desired velocity at the maximum speed.
	// Though we need to adjust this speed as we want to go into that direction
	// at the best we are allowed, which is the remaining velocity.
	desired_vel -= vel;
    
	addForce(desired_vel);
    
	//cout << "Desired vel: " << desired_vel.x << ", " << desired_vel.y << endl;
}

/**
 * Fleeing is the exactly opposite of seek. We calculate the exactly same force
 * as we would for seek(), though instead of adding a force into the target direction
 * we subtract this force. See seek() for a description of how this calculation works.
 */
void Boid::flee(ofVec3f oTarget) {
	ofVec3f desired_vel = oTarget - pos;
	desired_vel.scale(max_vel);
	desired_vel -= vel;
	subtractForce(desired_vel);
}


/**
 * Arrive makes the boid slow down when it moves towards its targets. The closer
 * it gets the slower it will move. The boid only slows down when it is withing a
 * certain distance of the target.
 */
void Boid::arrive(ofVec3f oTarget) {
	ofVec3f desired_vel = oTarget - pos;
	float dist = desired_vel.length();
	desired_vel.normalize();
    
	//ofSetColor(0xFF0000);
	//ofCircle(oTarget.x, oTarget.y, 4);
    
	// When we are within the arrive distance, we switch to slow-down mode.
	if (dist <= arrive_dist) {
		desired_vel *= ((max_vel * dist / arrive_dist));
		ofSetColor(0);
	}
	// else we continue our journey they same we did when we were seeking
	else {
		desired_vel *= max_vel;
	}
	// We add a little margin here;
	// w/o the boid will oversh0ot its target.
	if (dist > 10) {
		desired_vel -= vel;
		addForce(desired_vel);
	}
	// When were are within the margin, stop moving.
	else{
		force.set(0,0,0);
		vel.set(0,0,0);
	}
}

/**
 * Pursueing is somewhat the same as seeking, though we seek for a
 * moving target. But we also add something extra. We predict the
 * future position of the target and use that as a target.
 *
 * We take the velocity and position of the Boid we pursue and add
 * a little extra to this position using the velocity and a "look
 * ahead time". This look ahead time indicates how far we look in the
 * future ;-). A good solution to find out what the look ahead time
 * should be is using the distance between 'this' and the Boid we
 * pursue.
 *
 */
void Boid::pursue(Boid* pBoid) {
	float dist = (pBoid->getPosition() - getPosition()).length();
	float ahead_time = dist / max_vel;
	ofVec3f predicted_target = pBoid->getPosition() + pBoid->getVelocity()*ahead_time;
	seek(predicted_target);
}



void Boid::addForce(ofVec3f oForce) {
	force += oForce;
}
void Boid::subtractForce(ofVec3f oForce) {
	force -= oForce;
}

ofVec3f Boid::getPosition() {
	return pos;
}

ofVec3f Boid::getVelocity() {
	return vel;
}

void Boid::setPosition(ofVec3f oPos) {
	pos = oPos;
}

void Boid::setMaxVelocity(float fMax) {
	max_vel = fMax;
}
float Boid::getMaxVelocity() {
	return max_vel;
}

void Boid::setVelocity(ofVec3f oVel) {
	vel = oVel;
}

