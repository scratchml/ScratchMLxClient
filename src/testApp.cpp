#include "testApp.h"

static const int maxData = 512;

void testApp::loadSettings() {
	ofxXmlSettings xml;
	xml.loadFile("settings.xml");
    unsigned long systemtime = ofGetSystemTime();
    string filename = "session-"+ofToString(systemtime)+".xml";
    scratchMLfile.loadFile(filename);
	
	xml.pushTag("record");
	recordFormat = xml.getValue("format", "serato_2a");
	recordSide = xml.getValue("side", "right");
	xml.popTag();
	
	xml.pushTag("osc");
	oscHost = xml.getValue("host", "");
	oscPort = xml.getValue("port", 0);
	oscSubdivide = xml.getValue("subdivide", 8);
	oscPitch = xml.getValue("pitch", 0);
	oscRelative = xml.getValue("relative", 0);
	oscDegrees = xml.getValue("degrees", 0);
	xml.popTag();
	
	xml.pushTag("audio");
	audioSamplerate = 44100;
	audioBuffersize = 16;
    audioSamplerate = xml.getValue("sampleRate", 44100);
    audioBuffersize = xml.getValue("bitRate", 16);
	xml.popTag();
	
	xml.pushTag("serial");
	serialPort = xml.getValue("port", "");
	serialThreshold = xml.getValue("threshold", 0);
	xml.popTag();
    
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

}


void testApp::setup() {
	ofSetWindowTitle("ScratchOSC");
	ofSetVerticalSync(true);
	ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
	ofSetCircleResolution(50);
	ofSetLineWidth(2);
	
    totalDecks = 1;
    
	loadSettings();
    

    //fader--------_
    serialReady = serial.setup(serialPort, 115200);
    //----fader----_
    
    oscRate = (audioSamplerate / audioBuffersize) / oscSubdivide;
    
    osc.setup(oscHost, oscPort);
    
    audioFrame = 0;
    
    nChannels = totalDecks * 2;
    
    //soundStream---------_
	c1.listDevices();
	//c1.setDeviceIdByName(audioInterface);//"M-Audio, Inc.: M-Audio Conectiv"
    c1.setup(1, nChannels, audioSamplerate, audioBuffersize, nChannels);
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
        }
        //------deck-----_
        //graphicAudioInputs----------_
        audioMutex.lock();
        graphicAudioInputs[i].frontAudioBuffer = middleAudioBuffers[i];
        audioMutex.unlock();
        //-----graphicAudioInputs-----_
    }
}

void testApp::exit(){
    scratchMLfile.popTag();
    scratchMLfile.popTag();
    scratchMLfile.popTag();
    scratchMLfile.popTag();
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
    //----fader----_
    scratchMLfile.saveFile();
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
    //-----drawudioInput-----_
    
    
    //    if(audioFrame % oscSubdivide == 0) {
    //        for(int i = 0; i < totalDecks; i++){
    //            
    //            float position = oscRelative ? decks[i].xwax.getRelative() : decks[i].xwax.getAbsolute();
    //            ofxOscMessage msg;
    //            msg.setAddress("/scratch/record/" + recordSide);
    //            msg.addFloatArg(position);
    //            osc.sendMessage(msg);
    //    
    //    
    //            scratchMLfile.pushTag("turntable");
    //            scratchMLfile.pushTag(recordSide);
    //            scratchMLfile.pushTag("data");
    //            scratchMLfile.addValue("p", position);
    //            scratchMLfile.popTag();
    //            scratchMLfile.popTag();
    //            scratchMLfile.popTag();
    //            
    //            if(oscPitch) {
    //                ofxOscMessage msg;
    //                msg.setAddress("/scratch/record/" + recordSide + "/pitch");
    //                msg.addFloatArg(decks[i].xwax.getPitch());
    //                osc.sendMessage(msg);
    //                scratchMLfile.pushTag(ofToString(audioFrame));
    //                scratchMLfile.pushTag(recordSide);
    //                scratchMLfile.addValue("pitch", decks[i].xwax.getPitch());
    //                scratchMLfile.popTag();
    //                scratchMLfile.popTag();
    //            
    //            }
    //            if(oscDegrees) {
    //                ofxOscMessage msg;
    //                msg.setAddress("/scratch/record/" + recordSide + "/degrees");
    //                msg.addFloatArg(fmodf(decks[i].xwax.millisToDegrees(position), 360));
    //                osc.sendMessage(msg);
    //                scratchMLfile.pushTag(ofToString(audioFrame));
    //                scratchMLfile.pushTag(recordSide);
    //                scratchMLfile.addValue("degrees", fmodf(decks[i].xwax.millisToDegrees(position), 360));
    //                scratchMLfile.popTag();
    //                scratchMLfile.popTag();
    //            }
    //        }
    //          
    //    }
    //    audioFrame++;
    //    	
    //    
    //fader--------_
	if(serialReady) {
		unsigned char faderData;
		int nRead;
		while ((nRead = serial.readBytes(&faderData, 1)) > 0) {
			bool prevOverThreshold = overThreshold;
			overThreshold = faderData > serialThreshold;
			if(prevOverThreshold != overThreshold) {
				ofxOscMessage msg;
				msg.setAddress("/scratch/mixer/fader");
				msg.addFloatArg(overThreshold ? 1 : 0);
				osc.sendMessage(msg);
                scratchMLfile.addTag(ofToString(audioFrame));
                scratchMLfile.pushTag(ofToString(audioFrame));
                scratchMLfile.addTag("mixer");
                scratchMLfile.pushTag("mixer");
                scratchMLfile.addTag("fader");
                scratchMLfile.pushTag("fader");
                scratchMLfile.addValue("position", overThreshold?1:0);
                scratchMLfile.popTag();
                scratchMLfile.popTag();
                scratchMLfile.popTag();
			}
			faderPosition.push_back(faderData);
            if(faderPosition.size() > maxData) {
                faderPosition.pop_front();
            }
		}
	}
    //---fader----_
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