//
//  ofxColorFinder.h
//
//  Created by Jan de Boer on 28/03/17.
//
//

#pragma once

#include "ofImage.h"
#include "ofColor.h"

#include <functional>
#include <string>
#include <map>

#define MEASURE_PEFORMANCE 0

class ofxColorFinder
{
    
public:
    
    typedef std::function<float(int, int, int)> Callback;
    
    static Callback dominantColor;
    
    static ofColor getColor(const ofImage &image, const Callback &callback = dominantColor);
    
private:
    
    typedef struct
    {
        int r;
        int g;
        int b;
        int count;
        float weight;
        int d;
    } Rgb;
    
    static void fillData(const ofImage &image, std::map<std::string, Rgb> &data,
                         const Callback &callback);
    
    static Rgb getRgb(map<string, Rgb> pixels, int degrade, Rgb *rgbMatch);
    
    static bool doesRgbMatch(Rgb *rgb, int r, int g, int b);
    
    static void printRgb(Rgb &rgb);
    
};