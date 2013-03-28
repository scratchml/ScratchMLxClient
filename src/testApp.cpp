#include "testApp.h"

static const int maxData = 512;



void testApp::setup() {
	ofSetWindowTitle("ScratchMLxClient");
	ofSetVerticalSync(true);
	ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
	ofSetCircleResolution(150);
	ofSetLineWidth(1);
	
	loadSettings();
    
    sml.setup();
    
    nChannels = totalDecks * 2;
    
    
    //fader--------_
    //serialReady = serial.setup(serialPort, 115200);
    //----fader----_
    
    
    //osc----------_
    oscRate = (audioSamplerate / audioBuffersize) / oscSubdivide;
    osc.setup(oscHost, oscPort);
    //----osc------_
    
    audioFrame = 0;
    xwax = ofxXwax();
    xwax.setup(audioSamplerate, audioBuffersize, recordFormat);
    
    //soundStream---------_
    
    
    //----soundStream-----_
    
    //inputs-----------_
    incrementCellHeight = 150;
    inputs.resize(nChannels);
    middleAudioBuffers.resize(nChannels);
    for(int c = 0; c < nChannels; c++) {
        inputs[c].resize(audioBuffersize);
        middleAudioBuffers[c].resize(audioBuffersize);
    }
    //------inputs-----_
    
    for (int i = 0; i < totalDecks; i++) {
        //deck-----------_
        string name = "deck-"+ofToString(i);
        sml.addDeck(name);
        deck myDeck;
        myDeck.setup(name, i, audioBuffersize);
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
        
        //------deck-----_
        //graphicAudioInputs----------_
        audioMutex.lock();
        graphicAudioInputs[i].frontAudioBuffer = middleAudioBuffers[i];
        audioMutex.unlock();
        //-----graphicAudioInputs-----_
    }
}

void testApp::exit(){
    sml.exit();
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
        ofPushMatrix();
        ofTranslate(0, cellHeight);
        decks[i].draw();
        ofPopMatrix();
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
    //----fader----_
}


bool testApp::loadSettings() {
	ofxXmlSettings settingsFile;
	settingsFile.loadFile(ofToDataPath("settings.xml"));
	settingsFile.pushTag("sml");
	settingsFile.pushTag("record");
	recordFormat = settingsFile.getValue("format", "serato_2a");
	recordSide = settingsFile.getValue("side", "right");
	settingsFile.popTag();
	
	settingsFile.pushTag("osc");
	oscHost = settingsFile.getValue("host", "127.0.0.1");
	oscPort = settingsFile.getValue("port", 12345);
	oscSubdivide = settingsFile.getValue("subdivide", 8);
	oscPitch = settingsFile.getValue("pitch", true);
	oscRelative = settingsFile.getValue("relative", true);
	oscDegrees = settingsFile.getValue("degrees", true);
	settingsFile.popTag();
	
    settingsFile.pushTag("audio");
    audioSamplerate = settingsFile.getValue("samplerate", 44100);
    audioBuffersize = settingsFile.getValue("buffersize", 256);
    audioDevice = settingsFile.getValue("device", 0);
    audioInterface = settingsFile.getValue("interface", "Apple Inc.: Built-in Input");
    totalDecks = settingsFile.getValue("decks", 2);
    settingsFile.popTag();
    
	
	settingsFile.pushTag("serial");
	serialPort = settingsFile.getValue("port", "");
	serialThreshold = settingsFile.getValue("threshold", 0);
	settingsFile.popTag();
    settingsFile.popTag();
    
    return true;
}



void testApp::audioIn(float * input, int bufferSize, int nChannel){
    //inputs-----------_
    // samples are "interleaved"
    
    int sample = 0;
    for(int i = 0; i < bufferSize; i++) {
        for(int j = 0; j < inputs.size(); j++){
            inputs[j][i*nChannel + 0] = input[sample++];
            inputs[j][i*nChannel + 1] = input[sample++];
        }
    }
    
    //drawudioInput----------_
	for (int i = 0; i < nChannel; i++) {
        audioMutex.lock();
        middleAudioBuffers[i].assign(&inputs[i][0], &inputs[i][0] + bufferSize * 2);
        audioMutex.unlock();
    }
    
    for(int i = 0; i < inputs.size(); i++){
        xwax.update(&inputs[i][0]);
        float pos = xwax.isAbsoluteValid()?xwax.getAbsolute():xwax.getRelative();
        decks.at(i).addSample(xwax.getAbsolute(), xwax.getRelative());
        sml.addSample(decks.at(i).getName(), pos, i);
    }
}




void testApp::keyPressed(int key){
    
}
void testApp::keyReleased(int key){
    
}
void testApp::mouseMoved(int x, int y ){
    
}
void testApp::mouseDragged(int x, int y, int button){
    
}
void testApp::mousePressed(int x, int y, int button){
    
}
void testApp::mouseReleased(int x, int y, int button){
    
}
void testApp::windowResized(int w, int h){
    
}

void testApp::dragEvent(ofDragInfo dragInfo){
    
}
void testApp::gotMessage(ofMessage msg){
    
}


//void testApp::audioIn(float* input, int audioBuffersize, int nChannels) {
//	xwax.update(input);
//
//	audioMutex.lock();
//	middleAudioBuffer.assign(input, input + audioBuffersize * nChannels);
//	audioMutex.unlock();

//	absolutePosition.push_back(xwax.getAbsolute());
//	relativePosition.push_back(xwax.getRelative());
//	if(absolutePosition.size() > maxData) {
//		absolutePosition.pop_front();
//	}
//	if(relativePosition.size() > maxData) {
//		relativePosition.pop_front();
//	}
//
//	if(audioFrame % oscSubdivide == 0) {
//		float position = oscRelative ? xwax.getRelative() : xwax.getAbsolute();
//		ofxOscMessage msg;
//		msg.setAddress("/scratch/record/" + recordSide);
//		msg.addFloatArg(position);
//		osc.sendMessage(msg);
//
//
//        scratchMLfile.pushTag("turntable");
//        scratchMLfile.pushTag(recordSide);
//        scratchMLfile.pushTag("data");
//        scratchMLfile.addValue("p", position);
//        scratchMLfile.popTag();
//        scratchMLfile.popTag();
//        scratchMLfile.popTag();
//
//		if(oscPitch) {
//			ofxOscMessage msg;
//			msg.setAddress("/scratch/record/" + recordSide + "/pitch");
//			msg.addFloatArg(xwax.getPitch());
//			osc.sendMessage(msg);
//            scratchMLfile.pushTag(ofToString(audioFrame));
//            scratchMLfile.pushTag(recordSide);
//            scratchMLfile.addValue("pitch", xwax.getPitch());
//            scratchMLfile.popTag();
//            scratchMLfile.popTag();
//
//		}
//		if(oscDegrees) {
//			ofxOscMessage msg;
//			msg.setAddress("/scratch/record/" + recordSide + "/degrees");
//			msg.addFloatArg(fmodf(xwax.millisToDegrees(position), 360));
//			osc.sendMessage(msg);
//            scratchMLfile.pushTag(ofToString(audioFrame));
//            scratchMLfile.pushTag(recordSide);
//            scratchMLfile.addValue("degrees", fmodf(xwax.millisToDegrees(position), 360));
//            scratchMLfile.popTag();
//            scratchMLfile.popTag();
//		}
//
//    }
//	audioFrame++;
//
//	if(serialReady) {
//		unsigned char faderData;
//		int nRead;
//		while ((nRead = serial.readBytes(&faderData, 1)) > 0) {
//			bool prevOverThreshold = overThreshold;
//			overThreshold = faderData > serialThreshold;
//			if(prevOverThreshold != overThreshold) {
//				ofxOscMessage msg;
//				msg.setAddress("/scratch/mixer/fader");
//				msg.addFloatArg(overThreshold ? 1 : 0);
//				osc.sendMessage(msg);
//
//                scratchMLfile.addTag(ofToString(audioFrame));
//                scratchMLfile.pushTag(ofToString(audioFrame));
//                scratchMLfile.addTag("mixer");
//                scratchMLfile.pushTag("mixer");
//                scratchMLfile.addTag("fader");
//                scratchMLfile.pushTag("fader");
//                scratchMLfile.addValue("position", overThreshold?1:0);
//                scratchMLfile.popTag();
//                scratchMLfile.popTag();
//                scratchMLfile.popTag();
//
//			}
//			faderPosition.push_back(faderData);
//			if(faderPosition.size() > maxData) {
//				faderPosition.pop_front();
//			}
//		}
//	}
//}