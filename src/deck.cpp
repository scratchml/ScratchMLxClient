#include "deck.h"


static const int maxData = 512;

void deck::setup(string deck, int deckNumber, int bufferSize) {
    name = deck;
    deckIndex = deckNumber;
    size = bufferSize;
    absolutePosition.resize(size);
    relativePosition.resize(size);
}

void deck::update() {
    if(absolutePosition.size() > size){
        absolutePosition.pop_back();
    }
    if(relativePosition.size() > size){
        relativePosition.pop_back();
    }
}

string deck::getName(){
    return name;
}

void deck::addSample(float abs, float rel){
    absolutePosition.push_front(abs);
    relativePosition.push_front(rel);
    graphVinyl.update(abs, rel);
}

void deck::customDraw() {

	ofNoFill();    
    ofPushMatrix();
	

    ofTranslate(10, 10);
    graphVinyl.draw(128, 0, 128);
    
    ofPopMatrix();
    
    ofPushMatrix();
    
    ofSetColor(ofColor::magenta);
    graphCurve.draw(absolutePosition, 128);
    
    ofSetColor(ofColor::yellow);
    graphCurve.draw(relativePosition, 128);
	
    ofPopMatrix();
    
    ofFill();
	
}




