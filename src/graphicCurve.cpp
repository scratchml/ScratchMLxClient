#include "graphicCurve.h"

static const ofColor yellowPrint = ofColor::fromHex(0xffee00);

void graphicCurve::draw(deque<float> curve, float scale, float min, float max) {
	if(min == 0 && max == 0) {
		for(int i = 0; i < curve.size(); i++) {
			if(i == 0 || curve[i] < min) {
				min = curve[i];
			}
			if(i == 0 || curve[i] > max) {
				max = curve[i];
			}
		}
	}
	if(fabs(min - max) > FLT_EPSILON) {
		ofNoFill();
		ofBeginShape();
		for(int i = 0; i < curve.size(); i++) {
			float y = ofMap(curve[i], min, max, scale, 0);
			ofVertex(i, y);
			if(i == curve.size() - 1) {
				ofDrawBitmapString(ofToString(curve.back()), i - 80, y);
			}
		}
		ofEndShape();
	}
}
