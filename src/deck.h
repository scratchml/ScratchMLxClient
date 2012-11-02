#pragma once

#include "ofMain.h"
#include "ofxXwax.h"
#include "ofxSoundStream.h"
#include "ofxXmlSettings.h"
#include "ofxOsc.h"

//graphic
#include "graphicVinyl.h"
#include "graphicCurve.h"
#include "graphicAudioInput.h"



class deck : public ofNode{
public:
	void setup(string foo, ofxXmlSettings &bar);
	void update();
	void draw(float newX, float newY);
    void audioInputListener(float* input, int audioBuffersize);
	void sendMessage();
    bool hasMessage();
    ofxOscMessage getMessage();
    
    ofxXmlSettings scratchMLfile;
    ofxOscSender sender;
    
    float x;
    float y;
    
    string name;
    
    ofxXwax xwax;
	
	bool serialReady;
	unsigned char serialThreshold;
	bool overThreshold;
	
	string recordFormat, recordSide, serialPort, audioInterface, oscHost;
	unsigned int audioSamplerate, audioBuffersize;

    unsigned int oscPort, oscSubdivide;
	float oscRate;
	bool oscPitch, oscRelative, oscDegrees;
    
    
    graphicVinyl graphVinyl;
    graphicCurve graphCurve;
    
	deque<float> absolutePosition, relativePosition, faderPosition;
	
	unsigned int audioFrame;
    
    vector<float> input;
    int nChannels;
    
    bool hasM;

};
