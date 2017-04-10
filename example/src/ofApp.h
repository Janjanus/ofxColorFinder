#pragma once

#include "ofMain.h"

#include <vector>
#include <map>

#include "ofxColorFinder.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void draw();

		void keyPressed(int key);
		
private:
    
    std::vector<ofImage> images;
    
    std::map<unsigned char, ofxColorFinder::Callback> callbacks;
    
    unsigned char activeCallback;
    
};
