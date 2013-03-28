#pragma once

#include "ofMain.h"



//graphic
#include "graphicVinyl.h"
#include "graphicCurve.h"
#include "graphicAudioInput.h"



class deck : public ofNode{
public:
	void setup(string name, int deckNumber, int bufferSize);
	void update();
	void customDraw();
    void addSample(float abs, float rel);
    string getName();
protected:
    
    string name;
    int deckIndex;
    int size;
    graphicVinyl graphVinyl;
    graphicCurve graphCurve;
	deque<float> absolutePosition, relativePosition;
	
};
