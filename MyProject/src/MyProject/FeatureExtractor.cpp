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
        MySF = new SpectralFlux(iNumChannel,iNumFFT);
        SFOutput = new float[iNumChannel];
    }
    if (SC) {
        MySC = new SpectralCentroid(SampleRate,iNumChannel,iNumFFT);
        SCOutput = new float[iNumChannel];
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

string FeatureExtractor::getChosenFeatures() const
{
    
}

void FeatureExtractor::setTest()
{
    test = true;
}
void  FeatureExtractor::featureExtract(float **input, std::vector<float *> & output)
{
    if (!test) {
        if (SF || SC || SR) {
            for (int i = 0 ; i < iNumChannel; i++) {
                MyFFT[i] -> doFft(Spectrum[i], input[i]);
                MyFFT[i] -> getMagnitude(FFTMag[i], Spectrum[i]);
                
            }
        }
        if (SC) {
            //        for (int i = 0 ; i < iNumFFT; i++) {
            //            std::cout<<FFTMag[0][i]<<std::endl;
            //        }
            MySC -> process(FFTMag, SCOutput);
            output.push_back(SCOutput);
        }
        if (SF) {
            MySF -> process(FFTMag, SFOutput);
            output.push_back(SFOutput);
        }
        if (ZC) {
            
        }
        if (SR) {
            
        }

    }
    else {
        if (SC) {
            MySC -> process(input, SCOutput);
            output.push_back(SCOutput);
        }
        if (SF) {
            MySF -> process(input, SFOutput);
            output.push_back(SFOutput);
        }
        if (ZC) {
            
        }
        if (SR) {
            
        }

    }
    
}

void  FeatureExtractor::reset()
{
    SC = false;
    SF = false;
    SR = false;
    ZC = false;
    for (int i = 0 ; i < iNumChannel; i++) {
        MyFFT[i] -> resetInstance();
    }
    MySF -> reset();
}












