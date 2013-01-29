#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
   
    ofSetVerticalSync(true);
//	ofSetFrameRate(60);
	ofBackground(0,0,0);
	glEnable(GL_DEPTH_TEST);
    
    // turn on smooth lighting //
    
//    ofSetSmoothLighting(true);
//    ofSetSphereResolution(128);
    
    // Point lights emit light in all directions //
    // set the diffuse color, color reflected from the light source //
//    pointLight.setDiffuseColor( ofColor(255.f, 255.f, 255.f));
    // specular color, the highlight/shininess color //
//	pointLight.setSpecularColor( ofColor(255.f, 255.f, 255.f));
//	pointLight.setPointLight();
    
    
//    // shininess is a value between 0 - 128, 128 being the most shiny //
//	material.setShininess( 120 );
//    // the light highlight of the material //
//	material.setSpecularColor(ofColor(255, 255, 255, 255));
//    
//    // tex coords for 3D objects in OF are from 0 -> 1, not 0 -> image.width
//    // so we must disable the arb rectangle call to allow 0 -> 1
//    ofDisableArbTex();
//    // load an image to use as the texture //
//    ofLogoImage.loadImage("D2.png");
//    bUseTexture = true;
    
    boids = new Boids(200);
    eCam.setDistance(500);


    eCam.setPosition(ofPoint(ofGetWidth()/2, ofGetHeight()/2, -1500));
    lookat.setPosition(ofPoint(ofGetWidth()/2, ofGetHeight()/2, 0));
    eCam.lookAt(lookat);  

}



//--------------------------------------------------------------
void testApp::update(){
    boids->setMaxVelocity(5);
	boids->flock();
	if (mode == 'a') {
		boids->arrive(target);
	}
	else if (mode == 'f') {
		boids->flee(target);
	}
	else if (mode == 'e') {
		boids->explode();
		mode = 0;
	}
	boids->update();
	boids->bounce();
  

}

//--------------------------------------------------------------
void testApp::draw(){
    
    // enable lighting //
    ofEnableLighting();
    // enable the material, so that it applies to all 3D objects before material.end() call //
//	material.begin();
    // activate the lights //
    pointLight.enable();
    
    // grab the texture reference and bind it //
    // this will apply the texture to all drawing (vertex) calls before unbind() //
//    ofLogoImage.getTextureReference().bind();

    

    eCam.begin();
    ofPushMatrix();
    
    ofBox(10,10,10,10);
//    drawBox();
    ofTranslate(-10,0,50);
    boids->draw();
    ofPopMatrix();

    eCam.end();    
    
//    ofLogoImage.getTextureReference().unbind();
	
	pointLight.disable();
//    material.end();
	// turn off lighting //
    ofDisableLighting();

}


void testApp::drawBox(){
    ofPushMatrix();
    ofSetColor(0,0,0,255);
    
    ofRect(0,0,ofGetWidth(),ofGetHeight());
    
    ofPushMatrix();
    ofRotateX(90);
    ofRect(0,0,ofGetWidth(),ofGetHeight());
    
    ofTranslate(0,ofGetHeight());
    ofRotateX(-90);
    ofRect(0,0,ofGetWidth(),ofGetHeight());
    
    ofTranslate(0,ofGetHeight());
    ofRotateX(-90);
    ofRect(0,0,ofGetWidth(),ofGetHeight());
    
    ofPopMatrix();
    ofPopMatrix();
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key == ' ') {
		//gui.toggleDraw();
	}
	else if (key == 'a') {
		target.set(ofGetWidth()/2, ofGetHeight()/2,0);
		mode = key;
	}
	else if (key == 'f') {
		target.set(ofGetWidth()/2, ofGetHeight()/2,0);
		mode = key;
	}
	else if (key == 'e') {
		mode = key;
	}
	else if (key == 'c') {
		mode = 0;
	} else if (key == 'r') {
        eCam.setPosition(ofPoint(ofGetWidth()/2, ofGetHeight()/2, -1500));
        lookat.setPosition(ofPoint(ofGetWidth()/2, ofGetHeight()/2, 7750-664));
        eCam.lookAt(lookat);  
    }
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}