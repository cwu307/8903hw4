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
            float s = SC[i];
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