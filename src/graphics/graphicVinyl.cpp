#include "graphicVinyl.h"


void graphicVinyl::draw(float x, float y, float scale) {
	scale /= 2;
	ofPushStyle();
	ofPushMatrix();
	
	ofTranslate(x, y);
	ofScale(scale, scale);
	ofTranslate(1, 1);
	ofNoFill();
	ofSetColor(255);
	ofCircle(0, 0, 1);
	
	ofPushMatrix();
	ofSetColor(ofColor::yellow);
	ofRotate(rotateAbsolute);
	ofLine(0, 0, 0, 1);
	ofPopMatrix();
	
	ofPushMatrix();
	ofSetColor(ofColor::magenta);
	ofRotate(rotateRelative);
	ofLine(0, 0, 0, 1);
	ofPopMatrix();
	
	ofPopMatrix();
	ofPopStyle();
}

void graphicVinyl::update(float abs, float rel){
    rotateAbsolute = ofMap(rel, 0, 1800, 0, 360);
    rotateRelative = ofMap(abs, 0, 1800, 0, 360);
}