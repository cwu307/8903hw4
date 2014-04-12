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
        MySF = new SpectralFlux(iNumChannel,iMagLength);
        SFOutput = new float[iNumChannel];
    }
    if (SC) {
        MySC = new SpectralCentroid(SampleRate,iNumChannel,iMagLength);
        SCOutput = new float[iNumChannel];
    }
    if (SR) {
        MySR = new SpectralRolloff(SampleRate, iNumChannel, iMagLength);
        SROutput = new float [iNumChannel];
    }
    if (ZC) {
        MyZC = new MyZeroCrossing(SampleRate, iNumChannel, iNumFFT);
        ZCOutput = new float [iNumChannel];
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
    if (SR) {
        delete MySR;
        MySR = 0;
        delete [] SROutput;
        SROutput = 0;
        SR = false;
    }
    if (ZC) {
        delete MyZC;
        MyZC = 0;
        delete [] ZCOutput;
        ZCOutput = 0;
        ZC = false;
    }
}

string FeatureExtractor::getChosenFeatures() const
{
    std::string allFeatures;
    if (SC)
    {
        allFeatures = allFeatures + std::string(" Spectral Centroid");
    }
    if (SF)
    {
        allFeatures = allFeatures + std::string(" Spectral Flux");
    }
    if (SR)
    {
        allFeatures = allFeatures + std::string(" Spectral Rolloff");
    }
    if (ZC)
    {
        allFeatures = allFeatures + std::string(" Zero Crossing");
    }
    
    return allFeatures;
}


void FeatureExtractor::setTest(bool testStatus)
{
    //if test = true, treat input as spectrum
    //if test = false, treat input as time-domain signal
    test = testStatus;
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
        if (SR) {
            MySR -> process(FFTMag, SROutput);
            output.push_back(SROutput);
        }
        if (ZC) {
            MyZC -> process(input, ZCOutput);
            output.push_back(ZCOutput);
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
        if (SR) {
            MySR -> process(input, SROutput);
            output.push_back(SROutput);
        }
        if (ZC) {
            MyZC -> process(input, ZCOutput);
            output.push_back(ZCOutput);
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


int FeatureExtractor::getChosenFeatureNum()
{
    int count = 0;
    if (SF) {
        count++;
    }
    if (SC)
    {
        count++;
    }
    if (SR)
    {
        count++;
    }
    if (ZC)
    {
        count++;
    }
    
    return count;
}









