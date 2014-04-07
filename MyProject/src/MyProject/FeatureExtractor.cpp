//
//  FeatureExtractor.cpp
//  MyProject
//
//  Created by Xinquan Zhou on 4/6/14.
//
//

#include "FeatureExtractor.h"

using std::vector;

void FeatureExtractor::chooseFeature(int index)
{
    switch (index) {
        case SpectCent:
            SC = true;
            break;
        case SpectFlux:
            SF = true;
            break;
        case SpectRolloff:
            SR = true;
            break;
        case ZeroCrossing:
            ZC = true;
            break;
        default:
            break;
    }
}

void FeatureExtractor::initFeatureExtractor()
{
    if (SF) {
        MySF = new SpectralFlux();
        MySF -> setParameter(NumBlocksize);
        SFOutput = new float[NumBlocksize];
        
    }
    if (SC) {
        MySC = new SpectralCentroid(SampleRate);
        MySC -> setParameter(NumBlocksize);
        SCOutput = new float[NumBlocksize];
    }
    if (ZC) {
        
    }
    if (SR) {
        
    }
    
}
void FeatureExtractor::destroyFeatureExtractor()
{
    if (SF) {
        delete MySF;
        MySF = 0 ;
        delete []SFOutput;
        SFOutput = 0;
        SF = false;
        
    }
    if (SC) {
        MySC -> reset();
        delete MySC;
        MySC = 0;
        delete []SCOutput;
        SCOutput = 0;
        SC = false;
    }
    if (ZC) {
        
    }
    if (SR) {
        
    }
}

void  FeatureExtractor::featureExtract(float **input, std::vector<float *> & output, int Num)
{
    if (SC) {
        MySC -> process(input, SCOutput, Num);
        //float result = TakeMean(SCOutput);
        output.push_back(SCOutput);
    }
    if (SF) {
        MySF -> process(input, SFOutput, Num);
        //float result = TakeMean(SFOutput);
        output.push_back(SFOutput);
    }
    if (ZC) {
        
    }
    if (SR) {
        
    }
}


float FeatureExtractor::TakeMean(float *input)
{
    float sum = 0 ;
    for (int i = 0 ; i < NumBlocksize; i++) {
        sum += input[i];
    }
    sum /= NumBlocksize;
    return sum;
}



void  FeatureExtractor::reset()
{
    SC = false;
    SF = false;
    SR = false;
    ZC = false;
}












