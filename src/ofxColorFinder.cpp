//
//  ofxColorFinder.cpp
//
//  Created by Jan de Boer on 28/03/17.
//
//

#include "ofxColorFinder.h"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <chrono>
#include <map>

using namespace std;
using namespace chrono;

template<typename Out>
void split(const string &s, char delim, Out result)
{
    stringstream ss;
    ss.str(s);
    string item;
    
    while (getline(ss, item, delim))
    {
        *(result++) = item;
    }
}


vector<string> split(const string &s, char delim)
{
    vector<string> elems;
    split(s, delim, back_inserter(elems));
    return elems;
}

ofxColorFinder::Callback ofxColorFinder::dominantColor = [](int r, int g, int b) { return 1; };

ofColor ofxColorFinder::getColor(const ofImage &image, const Callback &callback)
{
#if MEASURE_PEFORMANCE > 0
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
#endif
    
    map<string, Rgb> result;
    Rgb rgb;
    
    fillData(image, result, callback);
    
    rgb = getRgb(result, 6, nullptr);
    rgb = getRgb(result, 4, &rgb);
    rgb = getRgb(result, 2, &rgb);
    rgb = getRgb(result, 0, &rgb);
    
#if MEASURE_PEFORMANCE > 0
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>( t2 - t1 ).count();
    cout << "Image " << &image << " took: " << duration << " ms" << endl;
#endif
    
    return ofColor(rgb.r, rgb.g, rgb.b);
}

void ofxColorFinder::fillData(const ofImage &image, std::map<std::string, Rgb> &result,
                           const Callback &callback)
{
    if(image.getImageType() == ofImageType::OF_IMAGE_COLOR
       || image.getImageType() == ofImageType::OF_IMAGE_COLOR_ALPHA)
    {
        auto pixels = image.getPixels();
        
        int length = pixels.size();
        float factor = max(float(1), round(float(length) / float(length)));
        
        int channels = pixels.getNumChannels();
        int i = -channels;
        int degrade = 0;
        
        while ( (i += channels * factor) < length )
        {
            if (pixels[i + 3] > 32)
            {
                stringstream ss;
                ss << (pixels[i] >> degrade) << ",";
                ss << (pixels[i+1] >> degrade) << ",";
                ss << (pixels[i+2] >> degrade);
                string key = ss.str();
                
                if (result.find(key) == result.end()) {
                    Rgb rgb = { pixels[i], pixels[i+1], pixels[i+2], 1, 0, 0 };
                    rgb.weight = callback(rgb.r, rgb.g, rgb.b);
                    
                    if (rgb.weight <= 0) rgb.weight = 1e-10;
                    
                    result[key] = rgb;
                }
                else
                {
                    Rgb rgb = result[key];
                    rgb.count++;
                    result[key] = rgb;
                }
            }
        }
    }
}

ofxColorFinder::Rgb ofxColorFinder::getRgb(map<string, Rgb> pixels, int degrade, Rgb *rgbMatch)
{
    int count = 0;
    map<string, int> db;
    
    for(auto const &entry : pixels)
    {
        Rgb pixel = entry.second;
        int totalWeight = pixel.weight * pixel.count;
        ++count;
        
        if(doesRgbMatch(rgbMatch, pixel.r, pixel.g, pixel.b))
        {
            stringstream ss;
            ss << (pixel.r >> degrade) << ",";
            ss << (pixel.g >> degrade) << ",";
            ss << (pixel.b >> degrade);
            string pixelGroupKey = ss.str();
            
            if(db.find(pixelGroupKey) != db.end())
            {
                db[pixelGroupKey] += totalWeight;
            }
            else
            {
                db[pixelGroupKey] = totalWeight;
            }
        }
    }
    
    Rgb rgb = {0, 0, 0, 0, 0, degrade };
    for(auto const &entry : db)
    {
        auto data = split(entry.first, ',');
        int r = stoi(data[0]);
        int g = stoi(data[1]);
        int b = stoi(data[2]);
        int count = entry.second;
        
        if (count > rgb.count)
        {
            rgb.count = count;
            rgb.r = r;
            rgb.g = g;
            rgb.b = b;
        }
    }

    return rgb;
}

bool ofxColorFinder::doesRgbMatch(Rgb *rgb, int r, int g, int b)
{
    if (rgb == nullptr) return true;
    
    r = r >> rgb->d;
    g = g >> rgb->d;
    b = b >> rgb->d;
    
    return rgb->r == r && rgb->g == g && rgb->b == b;
}

void ofxColorFinder::printRgb(Rgb &rgb)
{
    stringstream ss;
    ss << "{ r: " << rgb.r << " g: " << rgb.g << " b: " << rgb.b
    << " count: " << rgb.count << " degrage: " << rgb.d << " }" << endl;
    
    cout << ss.str();
}