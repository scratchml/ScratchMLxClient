#include "deck.h"

static const ofColor cyanPrint = ofColor::fromHex(0x00abec);
static const ofColor magentaPrint = ofColor::fromHex(0xec008c);
static const ofColor yellowPrint = ofColor::fromHex(0xffee00);
static const int maxData = 512;
deck::deck(){
    
}


void deck::setup(string foo, ofxXmlSettings &bar) {
    ofSetCircleResolution(50);
	ofSetLineWidth(2);	
    xwax.setup(audioSamplerate, audioBuffersize); 
    audioFrame = 0;
    oscSubdivide = 8;
    input.resize(audioBuffersize);
    name = foo;
    scratchMLfile = bar;
    currentPostion = 0;
    currentRelativePos = 0;
    currentPitch = 0;
    currentVelocity = 0;
}  


void deck::draw(float newX, float newY) {
    x = newX;
    y = newY;
	
    
    
    
	ofNoFill();
    
    ofPushMatrix();
    ofSetColor(255, 255, 0);
    ofTranslate(10+x, 10+y);
    ofRect(0, 0, 10, 10);
    ofPopMatrix();
    
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
	ofDrawBitmapString("pitch: " + ofToString(currentPitch, 2) + "x", 0, 20);
	ofDrawBitmapString("velocity: " + ofToString(currentVelocity, 2) + " ms", 0, 40);
    ofSetColor(magentaPrint);
	ofDrawBitmapString("relative: " + ofToString(currentRelativePos, 0) + " ms", 0, 60);
    ofSetColor(yellowPrint);
	ofDrawBitmapString("absolute: " + ofToString(currentPostion, 0) + " ms", 0, 80);
    ofSetColor(255);
	ofDrawBitmapString("absolute valid: " + ofToString(xwax.isAbsoluteValid() ? "yes" : "no"), 0, 100);
	ofPopMatrix();
}



bool deck::hasMessage(){
    return hasM;
}


ofxOscMessage deck::getMessage(){
    float position = oscRelative?currentRelativePos:currentPostion;
    ofxOscMessage msg;
    msg.setAddress("/scratch/record/" + name);
    msg.addFloatArg(position);
    
    
    if(oscPitch) {
        ofxOscMessage msg_p;
        msg_p.setAddress("/scratch/record/" + name + "/pitch");
        msg_p.addFloatArg(currentPitch);
        
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

    currentPitch = xwax.getPitch();
    currentVelocity = xwax.getVelocity();
    
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
    graphVinyl.rotateRelative = ofxXwax::millisToDegrees(xwax.getAbsolute());
    
    if(audioFrame % oscSubdivide == 0) {
        hasM = true;
    }
    
    scratchMLfile.pushTag(name);
    scratchMLfile.pushTag("data");
    scratchMLfile.addValue("p", currentPostion);
    scratchMLfile.popTag();
    scratchMLfile.popTag();
    
    audioFrame++;
}
