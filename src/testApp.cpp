#include "testApp.h"

static const int maxData = 512;

void testApp::loadSettings() {
	
    
	settingsFile.loadFile("settings.xml");
    unsigned long systemtime = ofGetSystemTime();
    
    if(settingsFile.getNumTags("sml") != 1){
        
        settingsFile.addTag("sml");
        
        settingsFile.pushTag("sml");
        
        settingsFile.addTag("record");
        
        settingsFile.pushTag("record");
        settingsFile.addValue("format", "serato_2a");
        settingsFile.addValue("side","right");
        settingsFile.popTag();
        
        settingsFile.addTag("osc");
        
        settingsFile.pushTag("osc");
        settingsFile.addValue("host", "10.0.0.255");
        settingsFile.addValue("port","12234");
        settingsFile.popTag();
        
        settingsFile.addTag("audio");
        
        settingsFile.pushTag("audio");
        settingsFile.addValue("sampleRate", 44100);
        settingsFile.addValue("buffersize", 16);
        settingsFile.addValue("interface", "");
        settingsFile.addValue("device", 1);
        settingsFile.popTag();
        settingsFile.addTag("turntables");
        settingsFile.pushTag("turntables");
        settingsFile.addValue("decks", 1);
        settingsFile.addValue("deck0", "left");
        settingsFile.addValue("deck1", "right");
        settingsFile.addValue("deck2", "left_two");
        settingsFile.addValue("deck3", "right_two");
        settingsFile.popTag();
        
        settingsFile.addTag("serial");
        settingsFile.popTag();
    }
    settingsFile.pushTag("sml");
    settingsFile.pushTag("record");
    recordFormat = settingsFile.getValue("format", "serato_2a");
    recordSide = settingsFile.getValue("side", "right");
    settingsFile.popTag();
    
    settingsFile.pushTag("osc");
    oscHost = settingsFile.getValue("host", "255.255.255.255");
    oscPort = settingsFile.getValue("port", 1235);
    oscSubdivide = settingsFile.getValue("subdivide", 8);
    oscPitch = settingsFile.getValue("pitch", 0);
    oscRelative = settingsFile.getValue("relative", 0);
    oscDegrees = settingsFile.getValue("degrees", 0);
    settingsFile.popTag();
    
    settingsFile.pushTag("turntables");
    totalDecks = settingsFile.getValue("decks", 1);
    
    settingsFile.pushTag("audio");
    audioSamplerate = 44100;
    audioBuffersize = 16;
    audioSamplerate = settingsFile.getValue("sampleRate", 44100);
    audioBuffersize = settingsFile.getValue("buffersize", 16);
    audioDevice = settingsFile.getValue("device", 1);
    audioInterface = settingsFile.getValue("interface", "");
    settingsFile.popTag();
    
    settingsFile.pushTag("serial");
    serialPort = settingsFile.getValue("port", "");
    serialThreshold = settingsFile.getValue("threshold", 0);
    settingsFile.popTag();
    settingsFile.popTag();
    
    
    string filename = "session-"+ofToString(systemtime)+".xml";
    scratchMLfile.loadFile(filename);
    
    if(scratchMLfile.getNumTags("sml") == 0){
        //sml header
        scratchMLfile.addTag("sml");
        scratchMLfile.pushTag("sml");
        //version
        scratchMLfile.addTag("version");
        scratchMLfile.pushTag("version");
        //should pull this from the settings.xml but whatever
        scratchMLfile.addValue("major", 0);
        scratchMLfile.addValue("minor", 6);
        scratchMLfile.popTag();
        //end version 
        
        //client about
        scratchMLfile.addTag("about");
        scratchMLfile.pushTag("about");
        scratchMLfile.addValue("client","--ScratchML-- Version: BETA.0");
        scratchMLfile.addValue("authors", "@ScratchML, so blame/tweet them if it sucks ballz");
        scratchMLfile.addValue("url","www.scratchml.com");
        scratchMLfile.addValue("github", "https://github.com/scratchml");
        scratchMLfile.popTag();
        //client end about
        
        
        //define audio sample, filename, description, deck, start, end time
        scratchMLfile.addTag("audio");
        scratchMLfile.pushTag("audio");
        scratchMLfile.addTag("sample");
        scratchMLfile.pushTag("sample");
        scratchMLfile.addValue("filename", "connect me to the GUI");
        scratchMLfile.addValue("description","connect me to the GUI");
        scratchMLfile.addValue("deck", "connect me to the GUI");
        scratchMLfile.addValue("start", "connect me to the GUI");
        scratchMLfile.addValue("stop","connect me ot the GUI");
        scratchMLfile.popTag();
        scratchMLfile.popTag();
        
        scratchMLfile.addTag("performance");
        scratchMLfile.pushTag("performance");
        
        scratchMLfile.addTag("turntable");
        scratchMLfile.pushTag("turntable");
        scratchMLfile.addTag("deck0");
        scratchMLfile.pushTag("deck0");
        scratchMLfile.addValue("samplerate","375");
        scratchMLfile.addTag("data");
        scratchMLfile.popTag();
        scratchMLfile.addTag("deck1");
        scratchMLfile.pushTag("deck1");
        scratchMLfile.addValue("samplerate","375");
        scratchMLfile.addTag("data");
        scratchMLfile.popTag();
    }else{
        scratchMLfile.pushTag("sml");
        scratchMLfile.pushTag("performance");
        scratchMLfile.pushTag("turntable");
    }
    
    fatLogo.load("fatLogo.svg");
}


void testApp::setup() {
	ofSetWindowTitle("ScratchML");
	ofSetVerticalSync(true);
	ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
	ofSetCircleResolution(150);
	ofSetLineWidth(1);
    
	loadSettings();
    
    
    //fader--------_
    //serialReady = serial.setup(serialPort, 115200);
    //----fader----_
    
    oscRate = (audioSamplerate / audioBuffersize) / oscSubdivide;
    
    osc.setup(oscHost, oscPort);
    
    audioFrame = 0;
    
    nChannels = totalDecks * 2;
    
    //soundStream---------_
	c1.listDevices();
	if(audioInterface != ""){
        c1.setDeviceIdByName(audioInterface);
    }else{
        c1.setup(audioDevice, nChannels, audioSamplerate, audioBuffersize, nChannels);
    }
	ofAddListener(c1.audioReceivedEvent, this, &testApp::audioInputListener);
    //----soundStream-----_
    
    //inputs-----------_
    incrementCellHeight = 150;
    inputs.resize(nChannels);
    middleAudioBuffers.resize(nChannels);
    for(int c = 0; c < nChannels; c++) {
        inputs[c].resize(audioBuffersize * 2); // 2 for stereo
        middleAudioBuffers[c].resize(audioBuffersize * 2);
    }
    //------inputs-----_
    
    osc.setup(oscHost, oscPort);
    
    for (int i = 0; i < totalDecks; i++) {
        //deck-----------_
        deck myDeck;
        myDeck.audioSamplerate = audioSamplerate;
        myDeck.audioBuffersize = audioBuffersize;
        myDeck.audioInterface = audioInterface;
        myDeck.nChannels = nChannels;
        myDeck.recordFormat = recordFormat;
        myDeck.recordSide = recordSide;
        myDeck.serialPort = serialPort;
        myDeck.serialThreshold = serialThreshold;
        myDeck.setup("deck"+ofToString(i), scratchMLfile);
        decks.push_back(myDeck);
        //------deck-----_
        
        //graphicAudioInputs----------_
        graphicAudioInput myGraphicAudioInput;
        myGraphicAudioInput.setup(audioBuffersize);    
        myGraphicAudioInput.audioBuffersize = audioBuffersize;
        graphicAudioInputs.push_back(myGraphicAudioInput);
        //-----graphicAudioInputs-----_
    }
    
}  

void testApp::update() {
    for (int i = 0; i < totalDecks; i++) {
        //deck-----------_
        decks[i].update();
        
        if(decks[i].hasMessage()){
            ofxOscMessage m =decks[i].getMessage();
            osc.sendMessage(m);
            if(m.getAddress() == "/scratch/record/deck0"){
                step_R = ofMap(count, 0, 1800, 0, 1);
            }
            if(m.getAddress() == "/scratch/record/deck1"){
                step_L = ofMap(count, 0, 1800, 0, 1);
            }
        }
        //------deck-----_
        //graphicAudioInputs----------_
        audioMutex.lock();
        graphicAudioInputs[i].frontAudioBuffer = middleAudioBuffers[i];
        audioMutex.unlock();
        //-----graphicAudioInputs-----_
    }
    count+=15;
    if(count > 1800){
        count = 0;
    }
    step_L = abs(step_L);
    step_R = abs(step_R);
}

void testApp::exit(){
    scratchMLfile.popTag();
    scratchMLfile.popTag();
    scratchMLfile.popTag();
    scratchMLfile.popTag();
    settingsFile.popTag();
    settingsFile.popTag();
    settingsFile.popTag();
    scratchMLfile.saveFile();
    settingsFile.saveFile();
}

void testApp::draw() {
    ofBackground(0);
	ofNoFill();
    
    for (int i = 0; i < totalDecks; i++) {
        if (i == 0) {
            cellHeight = 0; 
        }else{
            cellHeight = cellHeight + incrementCellHeight;
        }
        //deck-----------_
        decks[i].draw(0,cellHeight);
        //------deck-----_
        //graphicAudioInputs----------_
        graphicAudioInputs[i].draw(10, cellHeight+10, 128);
        //-----graphicAudioInputs-----_
    }
    //fader--------_
    ofPushMatrix();
    ofSetColor(255);
    drawFader.draw(faderPosition, 128, 0, 255);
	ofPopMatrix();
    
    ofPushMatrix();
	ofTranslate(ofGetWidth()-10-fatLogo.getWidth()*.75, ofGetHeight()-10-fatLogo.getHeight()*.75);
    ofScale(.75, .75);
    
    for (int i = 0; i < fatLogo.getNumPath(); i++)
    {
        ofPath &p = fatLogo.getPathAt(i);
        ofColor c = fatLogo.getPathAt(i).getFillColor();
        vector<ofPolyline>& lines = p.getOutline();
        
        for (int k = 0; k < lines.size(); k++)
        {
            ofPolyline line = lines[k].getResampledBySpacing(1);
            line = line.getSmoothed(1);
            
            int num = step_R * line.size();
            if(step_R == 0){
                num = line.size();
            }
            
            glBegin(GL_LINE_STRIP);
            for (int j = 0; j < num; j++)
            {
                ofVec3f &vv = line[j];
                ofSetColor(c);
                glVertex3f(vv.x, vv.y, vv.z);
            }
            glEnd();
        }
    }
    ofPopMatrix();
    
    ofPushMatrix();
	ofTranslate(10, ofGetHeight()-10-fatLogo.getHeight()*.75);
    ofScale(.75, .75);

    for (int i = 0; i < fatLogo.getNumPath(); i++)
    {
        ofPath &p = fatLogo.getPathAt(i);
        ofColor c = fatLogo.getPathAt(i).getFillColor();
        vector<ofPolyline>& lines = p.getOutline();
        for (int k = 0; k < lines.size(); k++)
        {
            ofPolyline line = lines[k].getResampledBySpacing(1);
            line = line.getSmoothed(1);
            
            int num = step_R * line.size();
            if(step_L == 0){
                num = line.size();
            }
            
            
            glBegin(GL_LINE_STRIP);
            for (int j = 0; j < num; j++)
            {
                ofVec3f &vv = line[j];
                ofSetColor(c);
                glVertex3f(vv.x, vv.y, vv.z);
            }
            glEnd();
        }
    }
    ofPopMatrix();
}

//--------------------------------------------------------------
void testApp::audioInputListener(ofxAudioEventArgs &args){	
    
    //inputs-----------_
    // samples are "interleaved"    
    int sample = 0;
    for(int i = 0; i < args.bufferSize; i++) {
        int k = i * 2; // 2 for stereo
        for(int c = 0; c < nChannels; c++) {
            inputs[c][k + 0] = args.buffer[sample++];
            inputs[c][k + 1] = args.buffer[sample++];
        }
    }
    //------inputs-----_
    
    //drawudioInput----------_
	for (int i = 0; i < totalDecks; i++) {
        
        decks[i].xwax.update(&inputs[i][0]);
        audioMutex.lock();
        middleAudioBuffers[i].assign(&inputs[i][0], &inputs[i][0] + args.bufferSize * 2); 
        audioMutex.unlock();
        decks[i].audioInputListener(&inputs[i][0], audioBuffersize);
        
    }
}
