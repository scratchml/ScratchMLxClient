#pragma once

#include "ofMain.h"
#include "deck.h"
#include "ofxXmlSettings.h"
#include "ofxOsc.h"
#include "ofxXwax.h"
#include "smlWriter.h"

#define HOST 192.168.1.255
#define PORT 12345


class testApp : public ofBaseApp {
public:
    
    void setup();
    void update();
    void draw();
    void exit();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    
    bool loadSettings();
    
    smlWriter sml;
    
    ofxOscSender osc;
	ofSerial serial;
	graphicCurve drawFader;
    
    ofxXwax xwax;
    
	bool serialReady;
	unsigned char serialThreshold;
	bool overThreshold;
	
	string recordFormat, recordSide, oscHost, serialPort, audioInterface;
	unsigned int oscPort, oscSubdivide, audioSamplerate, audioBuffersize;
	float oscRate;
	bool oscPitch, oscRelative, oscDegrees;
    unsigned int audioFrame;
    
    deque<float> faderPosition;
    
    //deck-----------_
    int totalDecks;
    float cellWidth, cellHeight, incrementCellHeight;
    vector<deck> decks;
    //------deck-----_
    
    //soundStream---------_
    void audioIn(float * input, int bufferSize, int nChannels);
    vector< vector<float> > inputs;
    int nChannels, audioDevice;
    ofSoundStream soundStream;
    //----soundStream-----_
    
    //drawAudioInput----------_
    vector<graphicAudioInput> graphicAudioInputs;
    vector < vector<float> > middleAudioBuffers;
    ofMutex audioMutex;
    //-----drawAudioInput-----_

};
