#pragma once

#include "ofMain.h"

class graphicVinyl {
public:
	void draw(float x, float y, float scale);
    void update(float abs, float rel);
    float rotateAbsolute, rotateRelative;
};
