#include "deck.h"

static const ofColor cyanPrint = ofColor::fromHex(0x00abec);
static const ofColor magentaPrint = ofColor::fromHex(0xec008c);
static const ofColor yellowPrint = ofColor::fromHex(0xffee00);
static const int maxData = 512;

void deck::setup(string foo, ofxXmlSettings &bar) {
    ofSetWindowTitle("ScratchOSC");
	ofSetVerticalSync(true);
	ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
	ofSetCircleResolution(50);
	ofSetLineWidth(2);	
    xwax.setup(audioSamplerate, audioBuffersize, recordFormat);
    audioFrame = 0;
    input.resize(audioBuffersize);
    name = foo;
    scratchMLfile = bar;
}  


void deck::draw(float newX, float newY) {
    x = newX;
    y = newY;
	
    ofBackground(0);
	ofNoFill();
    
    ofPushMatrix();
	ofTranslate(10+x, 10+y);
    graphVinyl.draw(128, 0, 128);
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(265+x, 0+y);
    ofSetColor(yellowPrint);
    graphCurve.draw(absolutePosition, 128);
    ofSetColor(magentaPrint);
    graphCurve.draw(relativePosition, 128);
    ofTranslate(0+x, 200+y);
	ofSetColor(255);
    graphCurve.draw(faderPosition, 128, 0, 255);
	ofPopMatrix();
	
	ofSetColor(255);
	ofPushMatrix();
	ofTranslate(790+x, 10+y);
	ofDrawBitmapString("pitch: " + ofToString(xwax.getPitch(), 2) + "x", 0, 20);
	ofDrawBitmapString("velocity: " + ofToString(xwax.getVelocity(), 2) + " ms", 0, 40);
    ofSetColor(magentaPrint);
	ofDrawBitmapString("relative: " + ofToString(xwax.getRelative(), 0) + " ms", 0, 60);
    ofSetColor(yellowPrint);
	ofDrawBitmapString("absolute: " + ofToString(xwax.getAbsolute(), 0) + " ms", 0, 80);
    ofSetColor(255);
	ofDrawBitmapString("absolute valid: " + ofToString(xwax.isAbsoluteValid() ? "yes" : "no"), 0, 100);
	ofPopMatrix();
}



bool deck::hasMessage(){
    return hasM;
}


ofxOscMessage deck::getMessage(){
    float position = oscRelative?xwax.getRelative():xwax.getAbsolute();
    ofxOscMessage msg;
    msg.setAddress("/scratch/record/" + name);
    msg.addFloatArg(position);
    
    
    if(oscPitch) {
        ofxOscMessage msg_p;
        msg_p.setAddress("/scratch/record/" + name + "/pitch");
        msg_p.addFloatArg(xwax.getPitch());
        
    }
    if(oscDegrees) {
        ofxOscMessage msg_d;
        msg_d.setAddress("/scratch/record/" + name + "/degrees");
        float temp_d = fmodf(xwax.millisToDegrees(position), 360);
        msg_d.addFloatArg(temp_d);
    }
    hasM = false;
    return msg;
}


//--------------------------------------------------------------
void deck::audioInputListener(float* input, int audioBuffersize){	
    
    //xwax--------_
    xwax.update(input);
    
	absolutePosition.push_back(xwax.getAbsolute());
	relativePosition.push_back(xwax.getRelative());
	if(absolutePosition.size() > maxData) {
		absolutePosition.pop_front();
	}
	if(relativePosition.size() > maxData) {
		relativePosition.pop_front();
	}
    //----xwax----_
    
    graphVinyl.rotateAbsolute = ofxXwax::millisToDegrees(xwax.getAbsolute());
    graphVinyl.rotateRelative = ofxXwax::millisToDegrees(xwax.getRelative());
    
    if(audioFrame % oscSubdivide == 0) {
        hasM = true;
    }
    
    scratchMLfile.pushTag(name);
    scratchMLfile.pushTag("data");
    scratchMLfile.addValue("p", xwax.getAbsolute());
    scratchMLfile.popTag();
    scratchMLfile.popTag();

    audioFrame++;
}
