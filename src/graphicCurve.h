#pragma once

#include "ofMain.h"

class graphicCurve {
public:	
	void draw(deque<float> curve, float scale, float min = 0, float max = 0);
};
