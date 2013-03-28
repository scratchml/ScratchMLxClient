//
//  smlWriter.cpp
//  ScratchMLxClient
//
//  Created by Dan Moore on 3/28/13.
//
//

#include "smlWriter.h"


void smlWriter::setup(){
    fileName = "session-"+ofToString(ofGetSystemTime())+".xml";
    newFile(fileName);
}

void smlWriter::newFile(string file){
    
    startTime = ofGetElapsedTimef();
    
    scratchMLfile.loadFromBuffer(ASCII);
    
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
    scratchMLfile.addValue("client","--ScratchML-- Version: BETA.2");
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
    scratchMLfile.addValue("filename", "optional");
    scratchMLfile.addValue("description","optional");
    scratchMLfile.addValue("deck", "optional");
    scratchMLfile.addValue("start", 0.0);
    scratchMLfile.addValue("stop",0.0);
    scratchMLfile.popTag();
    scratchMLfile.popTag();
    
    scratchMLfile.addTag("performance");
    scratchMLfile.pushTag("performance");//<performance>
    
    scratchMLfile.addTag("mixer");
    scratchMLfile.pushTag("mixer");//<mixer>
    scratchMLfile.addTag("fader");
    scratchMLfile.pushTag("fader");//<fader>
    scratchMLfile.addTag("data");
    scratchMLfile.popTag();//</fader>
    scratchMLfile.popTag();//</mixer>
    
}

void smlWriter::saveFile(){
    float stopTime = ofGetElapsedTimef();
    if(scratchMLfile.getNumTags("sml")>0){
        while(scratchMLfile.getPushLevel() > 0){
            scratchMLfile.popTag();
        }
        scratchMLfile.addValue("sml:performance:time:duration", stopTime-startTime);
        scratchMLfile.saveFile(fileName);
        scratchMLfile.clear();
        fileName = "session-"+ofToString(ofGetSystemTime())+".xml";
        newFile(fileName);
    }
}


void smlWriter::exit(){
    float stopTime = ofGetElapsedTimef();
    if(scratchMLfile.getNumTags("sml")>0){
        while(scratchMLfile.getPushLevel() > 0){
            scratchMLfile.popTag();
        }
        scratchMLfile.addValue("sml:performance:time:duration", stopTime-startTime);
    }
    scratchMLfile.saveFile(fileName);
}


bool smlWriter::uploadFile(){
    return false;
}

void smlWriter::addDeck(string name){
    deckIndex[name] = 0;
    int index = scratchMLfile.addTag("turntable");
    scratchMLfile.pushTag("turntable", index);
    scratchMLfile.addValue("deck", name);
    scratchMLfile.addValue("samplerate","375");
    scratchMLfile.addTag("data");
    scratchMLfile.popTag();
}

void smlWriter::addSample(string name, float pos, int index){
    scratchMLfile.pushTag("turntable", index);
    scratchMLfile.pushTag("data");
    scratchMLfile.addValue("p", deckIndex[name]);
    
    scratchMLfile.popTag();//pop data
    scratchMLfile.popTag();//pop turntable
    deckIndex[name]++;
}

void smlWriter::setHamster(bool isHamster){
    scratchMLfile.pushTag("mixer");//<mixer>
    scratchMLfile.pushTag("fader");//<fader>
    scratchMLfile.addValue("hamster", isHamster);
    scratchMLfile.popTag();//</fader>
    scratchMLfile.popTag();//</mixer>
}

void smlWriter::trackFader(float fader){
    scratchMLfile.pushTag("mixer");//<mixer>
    scratchMLfile.pushTag("fader");//<fader>
    scratchMLfile.pushTag("data");//<data>
    
    int index = scratchMLfile.addTag("event");
    scratchMLfile.pushTag("event", index);//<event>
    scratchMLfile.addValue("t", ofGetElapsedTimef()-startTime);
    scratchMLfile.addValue("p", fader);
    
    scratchMLfile.popTag();//</event>
    scratchMLfile.popTag();//</data>
    scratchMLfile.popTag();//</fader>
    scratchMLfile.popTag();//</mixer>
}
