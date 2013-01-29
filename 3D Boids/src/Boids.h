#ifndef BOIDS_H
#define BOIDS_H

#include <vector>
#include "Boid.h"
#include "ofMain.h"
#include "ofVec3f.h"

/**
 * Aggregator for our boids.
 */
class Boids {
	public:
		Boids(int iNumBoids);
		void update();
		void draw();

		void bounce();
		void seekParent(ofVec3f oMainTarget);
		void flock();
		vector<Boid*> getBoids();
		void setMaxVelocity(float fMaxVel);
		void arrive(ofVec3f oTarget);
		void flee(ofVec3f oTarget);
		void explode(); // apply random velocity (impuls);

	private:
		int num_boids;
		vector<Boid*> boids;

};

#endif
