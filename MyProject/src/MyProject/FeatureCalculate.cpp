//
//  FeatureCalculate.cpp
//  MyProject
//
//  Created by Xinquan Zhou on 4/6/14.
//
//

#include "FeatureCalculate.h"
#include <cmath>


void SpectralCentroid::setParameter(int NumBlocksize)
{
    SumSpec = new float [NumBlocksize];
    memset(SumSpec, 0.0, sizeof(float)*NumBlocksize);
    WeightSumSpec = new float [NumBlocksize];
    memset(WeightSumSpec, 0.0, sizeof(float)*NumBlocksize);
    Observations = NumBlocksize;
}

void     SpectralCentroid::reset()
{
    delete []SumSpec;
    delete []WeightSumSpec;
    WeightSumSpec = 0;
    SumSpec = 0;
}

void SpectralCentroid::process(float **input, float *SC,int NumFFT)
{
    for (int i = 0; i < Observations; i++) {
        for (int j = 0 ; j < NumFFT ; j++) {
            input[j][i] = pow(input[j][i],2.0);
            SumSpec[i] += input[j][i];
            input[j][i] *= j;
            WeightSumSpec[i] += input[j][i];
            
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



void SpectralFlux::setParameter(int NumBlocksize)
{
    Observations = NumBlocksize;
}

void SpectralFlux::process(float ** input, float * SC,int NumFFT)
{
    memset(SC, 0, sizeof(float)*Observations);
    for(int i = 0 ; i < Observations; i ++){
        for (int j = 0 ; j < NumFFT ; j++) {
            if (i == 0) {
                SC[i-1] = 0;
            }
            else{
                SC[i] += pow(input[j][i] - input[j][i-1],2.0);
                SC[i] = sqrtf(SC[i])/NumFFT;
            }
        }
    }
}