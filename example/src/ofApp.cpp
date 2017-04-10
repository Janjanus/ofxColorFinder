#include "ofApp.h"

using namespace std;

void ofApp::setup()
{
    // Load the images
    vector<string> paths = {"0.jpg", "1.jpg", "2.jpg", "3.jpg"};
    
    for(string path : paths)
    {
        ofImage image;
        image.load(path);
        
        images.push_back(image);
    }
    
    // Add some callbacks to the map that can be used by pressing one of the keys
    
    // Dominant color
    callbacks['1'] = [](int r, int g, int b) { return 1; };
    
    // Favor hue
    callbacks['2'] = [](int r, int g, int b) {
        return (abs(r-g)*abs(r-g) + abs(r-b)*abs(r-b) + abs(g-b)*abs(g-b))/65535*50+1; };
    
    // Favor bright
    callbacks['3'] = [](int r, int g, int b) { return r+g+b+1; };
    
    // Favor dark
    callbacks['4'] = [](int r, int g, int b) { return 768-r-g-b+1; };
    
    // Exclude white
    callbacks['5'] = [](int r, int g, int b) { return (r>245 && g>245 && b>245) ? 0 : 1; };
    
    // Exclude black
    callbacks['6'] = [](int r, int g, int b) { return (r<10 && g<10 && b<10) ? 0 : 1; };
    
    // Favor bright exclude white
    callbacks['7'] = [](int r, int g, int b) {
        if (r>245 && g>245 && b>245) return 0; return (r*r+g*g+b*b)/65535*20+1; };
    
    activeCallback = '1';
}

void ofApp::draw()
{
    int offsetX = 10;
    int offsetY = 10;
    
    for(int i = 0; i < images.size(); i++)
    {
        ofPushMatrix();
        ofPushStyle();
        
        ofTranslate(offsetX, offsetY);
        
        ofImage image = images[i];
        ofColor imageColor = ofxColorFinder::getColor(image, callbacks[activeCallback]);
     
        image.draw(0, 0);
        
        ofSetColor(imageColor);
        ofDrawRectangle(210, 0, 200, 200);
        
        offsetX += 420;
        
        if(i == 1)
        {
            offsetY += 210;
            offsetX = 10;
        }
        
        ofPopStyle();
        ofPopMatrix();
    }
}

void ofApp::keyPressed(int key)
{

    if(callbacks.find(key) != callbacks.end())
    {
        activeCallback = key;
    }
}