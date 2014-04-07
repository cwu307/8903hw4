#include "MyProjectConfig.h"

#ifdef WITH_TESTS
#include <cassert>
#include <cstdio>
#include<vector>
#include "UnitTest++.h"
#include "SignalGen.h"
#include "MyProject.h"
#include "FeatureExtractor.h"


SUITE(MyProject)
{
    struct MyProjectData
    {
        MyProjectData() 
        {
        }

        ~MyProjectData() 
        {
        }
    };

    TEST_FIXTURE(MyProjectData, NoTest)
    {
        CHECK_EQUAL (0 ,0);
    }
}

SUITE(FeatureExtrct){
    struct FeatureExt{
        FeatureExt()
        {
            MyTestFeatureExtractor = new FeatureExtractor(SampleRate,NumberBlocksize);
            MyData = new float* [NumFFT];
            MySinData = new float * [NumFFT];
            for (int i = 0 ;  i < NumFFT ; i++) {
                MyData[i] = new float [NumberBlocksize];
                MySinData[i] = new float [NumberBlocksize];
                memset(MyData[i], 0.0, sizeof(float)*NumberBlocksize);
                CSignalGen::generateSine(MySinData[i], 20.F, SampleRate, NumberBlocksize, .7F, static_cast<float>(M_PI_2));
            }
            MyOutput = new float [NumberBlocksize];
            
            
            
        }
        
        ~FeatureExt()
        {
            //MyTestFeatureExtractor -> destroyFeatureExtractor();
            delete MyTestFeatureExtractor;
            MyTestFeatureExtractor = 0 ;
            for (int i = 0 ; i < NumFFT; i++) {
                delete []MyData[i];
                delete []MySinData[i];
            }
            delete MyData;
            delete MySinData;
            MyData = 0 ;
            MySinData = 0 ;
            
            delete [] MyOutput;
            MyOutput = 0 ;
        }
        
        FeatureExtractor * MyTestFeatureExtractor;
        const int NumberBlocksize = 1024;
        const int NumFFT = 512;
        float ** MyData;
        float ** MySinData;
        float * MyOutput;
        float SampleRate = 4410;
        
    };
    TEST_FIXTURE(FeatureExt,ZeroInputSF)
    {
        std::vector<float *> results;
        MyTestFeatureExtractor -> chooseFeature(1);
        MyTestFeatureExtractor -> initFeatureExtractor();
        MyTestFeatureExtractor -> featureExtract(MyData,results, NumFFT);
        memcpy(MyOutput, results[0], sizeof(float)*NumberBlocksize);
        for (int i = 0 ; i < NumberBlocksize; i++) {
            CHECK_EQUAL(0, MyOutput[i]);
        }
        MyTestFeatureExtractor -> destroyFeatureExtractor();
        
    }
    TEST_FIXTURE(FeatureExt, ZeroInputSC){
        std::vector<float *> results;
        MyTestFeatureExtractor -> chooseFeature(0);
        MyTestFeatureExtractor -> initFeatureExtractor();
        MyTestFeatureExtractor -> featureExtract(MyData,results, NumFFT);
        memcpy(MyOutput, results[0], sizeof(float)*NumberBlocksize);

        for (int i = 0 ; i < NumberBlocksize; i++) {
            CHECK_EQUAL(0, MyOutput[i]);
        }
        MyTestFeatureExtractor -> destroyFeatureExtractor();
    }
    
}

#endif //WITH_TESTS