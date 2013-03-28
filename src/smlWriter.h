//
//  smlWriter.h
//  ScratchMLxClient
//
//  Created by Dan Moore on 3/28/13.
//
//

#ifndef __ScratchMLxClient__smlWriter__
#define __ScratchMLxClient__smlWriter__

#include "ofMain.h"
#include "ofxXmlSettings.h"



#define ASCII " <!-- .d8888b.  888b     d888 888      -->\n<!--d88P  Y88b 8888b   d8888 888      -->\n<!--Y88b.      88888b.d88888 888      -->\n<!-- \"Y888b.   888Y88888P888 888      -->\n<!--    \"Y88b. 888 Y888P 888 888      -->\n<!--      \"888 888  Y8P  888 888      -->\n<!--Y88b  d88P 888   \"   888 888      -->\n<!-- \"Y8888P\"  888       888 88888888 -->\n\n<?xml version=\"1.0\" encoding=\"UTF-8\"?>"

class smlWriter{
public:
    void setup();
    void newFile(string fileName);
    void saveFile();
    bool uploadFile();
    void reset();
    void addSample(string name, float pos, int index);
    void trackFader(float fader);
    void addDeck(string name);
    void setHamster(bool isHamster);
    void exit();
protected:

    ofxXmlSettings scratchMLfile;
    string fileName;
    map<string, int> deckIndex;
    float startTime;
    
};
#endif
