#pragma once

#include "ofMain.h"
#include "includes.h"

#define HOST 192.168.1.255
#define PORT 12345


class testApp : public ofBaseApp {
public:
	void loadSettings();
	void setup();
	void update();
	void draw();
	void exit();
    void audioInputListener (ofxAudioEventArgs &args);
    bool setupXwax();
    void drawXwax();
    void drawSetup();
    ofxXmlSettings scratchMLfile;
    ofxXmlSettings settingsFile;
    ofxOscSender osc;
	ofSerial serial;
	graphicCurve drawFader;
    
    ofxSVG fatLogo;
    
	bool serialReady;
	unsigned char serialThreshold;
	bool overThreshold;
	
	string recordFormat, recordSide, oscHost, serialPort, audioInterface;
	unsigned int oscPort, oscSubdivide, audioSamplerate, audioBuffersize;
	float oscRate;
	bool oscPitch, oscRelative, oscDegrees;
    unsigned int audioFrame;
    
    deque<float> faderPosition;
    
    bool up;
    bool doneUp;
    float step;
    int count;
    //deck-----------_
    int totalDecks;
    float cellWidth, cellHeight, incrementCellHeight;
    vector<deck> decks;
    //------deck-----_
    
    //soundStream---------_
    vector< vector<float> > inputs;
    int nChannels, audioDevice;
    ofxSoundStream c1;
    //----soundStream-----_
    
    //drawAudioInput----------_
    vector<graphicAudioInput> graphicAudioInputs;
    vector < vector<float> > middleAudioBuffers;
    ofMutex audioMutex;
    //-----drawAudioInput-----_
    
    bool frame;
    
};
