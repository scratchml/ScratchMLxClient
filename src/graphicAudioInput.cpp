#include "graphicAudioInput.h"

static const ofColor cyanPrint = ofColor::fromHex(0x00abec);
static const ofColor magentaPrint = ofColor::fromHex(0xec008c);

void graphicAudioInput::setup(unsigned int newAudioBuffersize) {
	frontAudioBuffer.resize(newAudioBuffersize * 2);
}

void graphicAudioInput::draw(float x, float y, float scale) {
	frontAudioBuffer.resize(audioBuffersize * 2);
	scale /= 4;
	int resolution = MAX(audioBuffersize / scale, 1);
	
	ofPushStyle();
	
	ofPushMatrix();
	ofTranslate(x + scale, y + scale);
	ofScale(scale, scale);
	
	ofNoFill();
	ofSetColor(255);
	ofRect(-1, -1, 2, 2);
	ofBeginShape();
	for(int i = 0; i < audioBuffersize; i += resolution) {
		ofVertex(frontAudioBuffer[i * 2], frontAudioBuffer[i * 2 + 1]);
	}
	ofEndShape();
	
	ofPushMatrix();
	ofSetColor(magentaPrint);
	ofTranslate(1, 0);
	ofScale(2. / audioBuffersize, 1);
	ofBeginShape();
	for(int i = 0; i < audioBuffersize; i += resolution) {
		ofVertex(i, frontAudioBuffer[i * 2]);
	}
	ofEndShape();
	ofPopMatrix();
	
	ofPushMatrix();
	ofSetColor(cyanPrint);
	ofTranslate(0, 1);
	ofScale(1, 2. / audioBuffersize);
	ofBeginShape();
	for(int i = 0; i < audioBuffersize; i += resolution) {
		ofVertex(frontAudioBuffer[i * 2 + 1], i);
	}
	ofEndShape();
	ofPopMatrix();
	
	ofPopMatrix();
	ofPopStyle();
}
