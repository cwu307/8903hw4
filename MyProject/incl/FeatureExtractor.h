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
#include "../../SharedSources/Dsp/Fft.h"

//class SpectralCentroid;
//class SpectralFlux;

using  std::string;

class FeatureExtractor{
public:
    FeatureExtractor(int samplerate, int blocksize, int numChannel):SampleRate(samplerate),iNumFFT(blocksize),iNumChannel(numChannel), SC(false),SF(false),ZC(false),SR(false),test(false){
        MyFFT = new CFft * [iNumChannel];
        Spectrum = new CFft::complex_t *[iNumChannel];
        FFTMag = new float * [iNumChannel];
        for (int i = 0 ; i < iNumChannel; i++) {
            CFft::createInstance(MyFFT[i]);
            MyFFT[i] -> initInstance(iNumFFT,1);
            iMagLength = MyFFT[i] -> getLength(CFft::kLengthMagnitude);
            iSpecLength = MyFFT[i] -> getLength(CFft::kLengthFft);
            Spectrum[i] = new CFft::complex_t[iSpecLength];
            FFTMag[i] = new float[iMagLength];
        }
    }
    
    ~FeatureExtractor(){
        for (int i = 0 ; i < iNumChannel; i++) {
            delete [] Spectrum[i];
            delete [] FFTMag[i];
            CFft::destroyInstance(MyFFT[i]);
        }
        delete [] Spectrum;
        delete [] FFTMag;
        delete [] MyFFT;
        
    }
    
    void  initFeatureExtractor();
    
    void destroyFeatureExtractor();
    
    void chooseFeature(int index);
    
    string getChosenFeatures() const;
    
    int  getChosenFeatureNum();
    
    void setTest(bool testStatus);
    
    void featureExtract(float ** input, std::vector<float*> &output);
    
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
    int iNumFFT;
    int iNumChannel;
    int iMagLength;
    int iSpecLength;
    
    MyZeroCrossing * MyZC;
    SpectralCentroid * MySC;
    SpectralFlux * MySF;
    SpectralRolloff *MySR;
    
    CFft ** MyFFT;
    CFft::complex_t ** Spectrum;
    float ** FFTMag;
    
    float * SCOutput;
    float * SFOutput;
    float * SROutput;
    float * ZCOutput;
    
    bool SC;
    bool SF;
    bool ZC;
    bool SR;
    
    bool test;
    
};

#endif /* defined(__MyProject__FeatureExtractor__) */
