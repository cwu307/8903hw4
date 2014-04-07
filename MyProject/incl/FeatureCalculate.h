//
//  FeatureCalculate.h
//  MyProject
//
//  Created by Xinquan Zhou on 4/6/14.
//
//

#ifndef __MyProject__FeatureCalculate__
#define __MyProject__FeatureCalculate__

#include <iostream>

class SpectralCentroid{
public:
    SpectralCentroid(int samplerate):SampleRate(samplerate){};
    ~SpectralCentroid(){};
    void setParameter(int NumBlocksize);
    void process(float ** input, float * SC,int NumFFT);
    void reset();
private:
    float SampleRate;
    int Observations;
    float * SumSpec;
    float * WeightSumSpec;
};


class SpectralFlux{
public:
    SpectralFlux(){};
    SpectralFlux(int samplerate):SampleRate(samplerate){};
    ~SpectralFlux(){};
    void setParameter(int NumBlocksize);
    void process(float ** input, float * SC,int NumFFT);
private:
    float SampleRate;
    int Observations;

};
#endif /* defined(__MyProject__FeatureCalculate__) */
