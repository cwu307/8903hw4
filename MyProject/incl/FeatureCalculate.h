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
    SpectralCentroid(int samplerate, int NumChannel, int sizeFFT):SampleRate(samplerate),iNumChannel(NumChannel), NumFFT(sizeFFT){
        SumSpec = new float[iNumChannel];
        WeightSumSpec = new float[iNumChannel];
        
    };
    ~SpectralCentroid(){
        delete [] SumSpec;
        delete [] WeightSumSpec;
        SumSpec = 0 ;
        WeightSumSpec = 0 ;
    }
    void process(float ** input, float * SC);
private:
    float SampleRate;
    int iNumChannel;
    int NumFFT;
    
    float * SumSpec;
    float * WeightSumSpec;
};


class SpectralFlux{
public:
    SpectralFlux(int NumChannel,int NumBlocksize):iNumChannel(NumChannel),NumFFT(NumBlocksize){
        PreSpect = new float* [NumChannel];
        for (int i = 0 ; i < iNumChannel; i++) {
            PreSpect[i] = new float [NumFFT];
            memset(PreSpect[i], 0.0, sizeof(float)*NumFFT);
        }
    }
    ~SpectralFlux(){
        for (int i = 0 ; i < iNumChannel ; i++) {
            delete [] PreSpect[i];
        }
        //delete PreSpect;
        PreSpect =  0 ;
    }
    void process(float ** input, float * SC);
    
    void reset(); 
private:
    int iNumChannel;
    int NumFFT;
    float ** PreSpect;
};

#endif /* defined(__MyProject__FeatureCalculate__) */
