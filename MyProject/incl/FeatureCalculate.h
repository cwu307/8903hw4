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
    SpectralCentroid(float samplerate, int NumChannel, int sizeFFT):SampleRate(samplerate),iNumChannel(NumChannel), NumFFT(sizeFFT){
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


class SpectralRolloff{
public:
    SpectralRolloff(float fSampleRate, int iNumChannel, int iNumFFT):
        m_fSampleRate(fSampleRate),
        m_iNumChannel(iNumChannel),
        m_iNumFFT(iNumFFT),
        m_fKappa(0.85)
    {
        m_fTotalSpecSum = new float [m_iNumChannel];
        m_fFractionSpecSum = new float [m_iNumChannel];
        m_iRolloffPoint = new int [m_iNumChannel];
        for (int i = 0; i < m_iNumChannel; i++)
        {
            m_fTotalSpecSum[i] = 0;
            m_fFractionSpecSum[i] = 0;
            m_iRolloffPoint[i] = 0;
        }
        
    }
    
    ~SpectralRolloff()
    {
        delete [] m_fTotalSpecSum;
        delete [] m_fFractionSpecSum;
        delete [] m_iRolloffPoint;
    }
    
    void  process(float **ppfSpectrumInput, float *pfSROutput);
    void  setKappa(float fNewKappa);
    float getKappa();
    void  resetSum();
    
private:
    float m_fSampleRate;
    int   m_iNumChannel;
    int   m_iNumFFT;
    float m_fKappa;
    float *m_fTotalSpecSum;
    float *m_fFractionSpecSum;
    int   *m_iRolloffPoint;
};


class MyZeroCrossing{
public:
    MyZeroCrossing(float fSampleRate, int iNumChannel, int iBlockSize):
    m_fSampleRate(fSampleRate),
    m_iNumChannel(iNumChannel),
    m_iBlockSize(iBlockSize)
    {
        m_fSignSum = new float [m_iNumChannel];
    }
    
    ~MyZeroCrossing()
    {
        delete [] m_fSignSum;
    }
    
    void process(float **ppfWaveformInput, float *pfZCOutput);
    void resetSignSum();
    
private:
    float m_fSampleRate;
    int   m_iNumChannel;
    int   m_iBlockSize;
    float *m_fSignSum;
    float m_fPreElement;
};

#endif /* defined(__MyProject__FeatureCalculate__) */
