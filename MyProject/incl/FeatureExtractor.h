//
//  FeatureExtractor.h
//  MyProject
//
//  Created by Xinquan Zhou on 4/6/14.
//
//

#ifndef __MyProject__FeatureExtractor__
#define __MyProject__FeatureExtractor__

#include <iostream>
#include <string>
#include <vector>

#include "FeatureCalculate.h"
//class SpectralCentroid;
//class SpectralFlux;

using  std::string;

class FeatureExtractor{
public:
    FeatureExtractor(int samplerate, int blocksize):SampleRate(samplerate),NumBlocksize(blocksize),SC(false),SF(false),ZC(false),SR(false){};
    
    ~FeatureExtractor(){};
    
    void  initFeatureExtractor();
    void destroyFeatureExtractor();
    
    void chooseFeature(int index);
    
    void featureExtract(float ** input, std::vector<float*> &output, int fft);
    
    void reset();
    enum FeatureType{
        SpectCent = 0 ,
        SpectFlux,
        SpectRolloff,
        ZeroCrossing,
        NumFeature
    };
    
private:
    float TakeMean(float * input);
    
    float SampleRate;
    int NumBlocksize;
    
    SpectralCentroid * MySC;
    SpectralFlux * MySF;
    float * SCOutput;
    float * SFOutput;
    float * SROutput;
    float * ZCOutput;
    
    bool SC;
    bool SF;
    bool ZC;
    bool SR;
    
};

#endif /* defined(__MyProject__FeatureExtractor__) */
