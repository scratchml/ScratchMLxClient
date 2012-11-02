#include "graphicVinyl.h"

static const ofColor magentaPrint = ofColor::fromHex(0xec008c);
static const ofColor yellowPrint = ofColor::fromHex(0xffee00);

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
	ofSetColor(yellowPrint);
	ofRotate(rotateAbsolute);
	ofLine(0, 0, 0, 1);
	ofPopMatrix();
	
	ofPushMatrix();
	ofSetColor(magentaPrint);
	ofRotate(rotateRelative);
	ofLine(0, 0, 0, 1);
	ofPopMatrix();
	
	ofPopMatrix();
	ofPopStyle();
}
