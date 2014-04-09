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
            MyTestFeatureExtractor = new FeatureExtractor(SampleRate,NumFFT,iNumChannel);
            MyTestFeatureExtractor -> setTest();
            MyData = new float* [iNumChannel];
            MyPureTone = new float * [iNumChannel];
            for (int i = 0 ;  i < iNumChannel ; i++) {
                MyData[i] = new float [NumFFT];
                MyPureTone[i] = new float [NumFFT];
                memset(MyData[i], 0.0, sizeof(float)*NumFFT);
                for (int j = 0 ; j < NumFFT; j++) {
                    if (j == 511) {
                        MyPureTone[i][j] = 1.0;
                    }
                    else MyPureTone[i][j] = 0.0 ;
                }
            }
            MyOutput = new float [iNumChannel];
            
            
            
        }
        
        ~FeatureExt()
        {
            //MyTestFeatureExtractor -> destroyFeatureExtractor();
            delete MyTestFeatureExtractor;
            MyTestFeatureExtractor = 0 ;
            for (int i = 0 ; i < iNumChannel; i++) {
                delete []MyData[i];
                delete []MyPureTone[i];
            }
            delete MyData;
            delete MyPureTone;
            MyData = 0 ;
            MyPureTone = 0 ;
            
            delete [] MyOutput;
            MyOutput = 0 ;
        }
        
        FeatureExtractor * MyTestFeatureExtractor;
        const int iNumChannel = 2;
        const int NumFFT = 512;
        float ** MyData;
        float ** MyPureTone;
        float * MyOutput;
        float SampleRate = 1024;
        
    };
    TEST_FIXTURE(FeatureExt,ZeroInputSF)
    {
        std::vector<float *> results;
        MyTestFeatureExtractor -> chooseFeature(1);
        MyTestFeatureExtractor -> initFeatureExtractor();
        MyTestFeatureExtractor -> featureExtract(MyData,results);
        memcpy(MyOutput, results[0], sizeof(float)*iNumChannel);
        for (int i = 0 ; i < iNumChannel; i++) {
            CHECK_CLOSE(0, MyOutput[i], 0.01);
        }
        MyTestFeatureExtractor -> destroyFeatureExtractor();
        
    }
    TEST_FIXTURE(FeatureExt, PureToneSF)
    {
        std::vector<float *> results;
        MyTestFeatureExtractor -> chooseFeature(1);
        MyTestFeatureExtractor -> initFeatureExtractor();
        MyTestFeatureExtractor -> featureExtract(MyPureTone,results);
        memcpy(MyOutput, results[0], sizeof(float)*iNumChannel);
        for (int i = 0 ; i < iNumChannel; i++) {
            CHECK_CLOSE(0, MyOutput[i], 0.01);
        }
        MyTestFeatureExtractor -> destroyFeatureExtractor();
    }
    TEST_FIXTURE(FeatureExt, ZeroInputSC){
        std::vector<float *> results;
        MyTestFeatureExtractor -> chooseFeature(0);
        MyTestFeatureExtractor -> initFeatureExtractor();
        MyTestFeatureExtractor -> featureExtract(MyData,results);
        memcpy(MyOutput, results[0], sizeof(float)*iNumChannel);

        for (int i = 0 ; i < iNumChannel; i++) {
            CHECK_EQUAL(0, MyOutput[i]);
        }
        MyTestFeatureExtractor -> destroyFeatureExtractor();
    }
    TEST_FIXTURE(FeatureExt, PureToneSC)
    {
        std::vector<float *> results;
        MyTestFeatureExtractor -> chooseFeature(0);
        MyTestFeatureExtractor -> initFeatureExtractor();
        MyTestFeatureExtractor -> featureExtract(MyPureTone,results);
        memcpy(MyOutput, results[0], sizeof(float)*iNumChannel);
        for (int i = 0 ; i < iNumChannel; i++) {
            CHECK_CLOSE(511, MyOutput[i], 0.1);
        }
        MyTestFeatureExtractor -> destroyFeatureExtractor();
    }
    
}

#endif //WITH_TESTS