//
//  FeatureCalculate.cpp
//  MyProject
//
//  Created by Xinquan Zhou on 4/6/14.
//
//

#include "FeatureCalculate.h"
#include <cmath>



void SpectralCentroid::process(float **input, float *SC)
{
    memset(SumSpec, 0.0, sizeof(float)*iNumChannel);
    memset(WeightSumSpec, 0.0, sizeof(float)*iNumChannel);
    for (int i = 0; i < iNumChannel; i++) {
        for (int j = 0 ; j < NumFFT ; j++) {
            float tmp = pow(input[i][j],2.0);
            SumSpec[i] += tmp;
            tmp *= j;
            WeightSumSpec[i] += tmp;
            
        }
        if (SumSpec[i]!=0) {
            SC[i] = WeightSumSpec[i]/SumSpec[i];
        }
        else{
            SC[i] = 0;
        }
        SC[i] = SC[i]/NumFFT * SampleRate/2;
        
    }
}

void SpectralFlux::process(float ** input, float * SC)
{
    memset(SC, 0.0, sizeof(float)*iNumChannel);
    for(int i = 0 ; i < iNumChannel; i ++){
        for (int j = 0 ; j < NumFFT ; j++) {
            SC[i] += pow((input[i][j] - PreSpect[i][j]),2.0);
            PreSpect[i][j] = input[i][j];
            SC[i] = sqrtf(SC[i])/NumFFT;
        }
    }
}

void SpectralFlux::reset()
{
    for (int i = 0 ; i < iNumChannel; i++) {
        memset(PreSpect[i], 0.0, sizeof(float)*NumFFT);
    }
}

//========Spectral Rolloff related function implementations here:
void SpectralRolloff::process(float **ppfSpectrumInput, float *pfSROutput)
{
    //first set the sum to be zeros
    this->resetSum();
    
    for (int i = 0; i < m_iNumFFT; i++)
    {
        for (int c = 0; c < m_iNumChannel; c++)
        {
            m_fTotalSpecSum[c] = m_fTotalSpecSum[c] + ppfSpectrumInput[c][i];
        }
    }
    
    //start finding the rolloff point
    
    for (int c = 0; c < m_iNumChannel; c++)
    {
        for (int i = 0; i < m_iNumFFT; i++)
        {
            m_fFractionSpecSum[c] = m_fFractionSpecSum[c] + ppfSpectrumInput[c][i];
            
            if (m_fFractionSpecSum[c] >= m_fKappa * (float)m_fTotalSpecSum[c])
            {
                pfSROutput[c] = ((float)i / (float)m_iNumFFT) * (m_fSampleRate / 2.0);
                break;
            }
        }
    }
    
    //set the sum to be zeros after calculations
    this->resetSum();
    
}

void SpectralRolloff::setKappa(float fNewKappa)
{
    m_fKappa = fNewKappa;
}

float SpectralRolloff::getKappa()
{
    return m_fKappa;
}

void SpectralRolloff::resetSum()
{
    for (int i = 0; i < m_iNumChannel; i++)
    {
        m_fTotalSpecSum[i]    = 0;
        m_fFractionSpecSum[i] = 0;
        m_iRolloffPoint[i]    = 0;
    }
}


//========Zero Crossing rate related function implementations here:
void MyZeroCrossing::process(float **ppfWaveformInput, float *pfZCOutput)
{
    //first set the sum to be zeros
    this->resetSignSum();
    
    //calculate zero crossings
    for (int i = 0; i < m_iBlockSize; i++)
    {
        for (int c = 0; c < m_iNumChannel; c++)
        {
            if (i > 0)
            {
                m_fPreElement = ppfWaveformInput[c][i-1];
            }
            else
            {
                m_fPreElement = 0;
            }
            
            m_fSignSum[c] = m_fSignSum[c] + fabs(copysignf(1.0, ppfWaveformInput[c][i]) - copysignf(1.0, m_fPreElement));
        }
    }
    
    for (int c = 0; c < m_iNumChannel; c++)
    {
        pfZCOutput[c] = 1.0 /(2.0 * m_iBlockSize) * m_fSignSum[c];
    }
    
    //set the sum to be zeros after calculations
    this->resetSignSum();
}



void MyZeroCrossing::resetSignSum()
{
    for (int c = 0; c < m_iNumChannel; c++)
    {
        m_fSignSum[c] = 0;
    }
}











