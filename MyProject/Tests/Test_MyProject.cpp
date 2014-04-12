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

SUITE(FeatureExtract){
    struct FeatureExt{
        FeatureExt():iNumChannel(2), NumFFT(512), SampleRate(1024)
        {
            MyTestFeatureExtractor = new FeatureExtractor(SampleRate,NumFFT,iNumChannel);
            MyTestFeatureExtractor -> setTest(true);
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
        const int iNumChannel;
        const int NumFFT;
        float ** MyData;
        float ** MyPureTone;
        float * MyOutput;
        float SampleRate;
        
    };
    TEST_FIXTURE(FeatureExt,ZeroInputSF)
    {
        std::vector<float *> results;
        MyTestFeatureExtractor -> chooseFeature(1);
        MyTestFeatureExtractor -> initFeatureExtractor();
        MyTestFeatureExtractor -> featureExtract(MyData,results);
        memcpy(MyOutput, results[0], sizeof(float)*iNumChannel);
        for (int i = 0 ; i < iNumChannel; i++) {
            CHECK_CLOSE(0, MyOutput[i], 0.001);
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
    
    
    
    //=============spectral rolloff + zero crossing tests
    TEST_FIXTURE(FeatureExt, ZeroInputSR)
    {
        std::vector<float *> results;
        MyTestFeatureExtractor -> chooseFeature(2);
        MyTestFeatureExtractor -> initFeatureExtractor();
        MyTestFeatureExtractor -> setTest(true);
        MyTestFeatureExtractor -> featureExtract(MyData,results);
        memcpy(MyOutput, results[0], sizeof(float)*iNumChannel);
        for (int i = 0 ; i < iNumChannel; i++) {
            //CHECK_CLOSE(0, MyOutput[i], 0.01);
            CHECK_EQUAL(0, MyOutput[i]);
        }
        MyTestFeatureExtractor -> destroyFeatureExtractor();
        
    }
    
    TEST_FIXTURE(FeatureExt, DCinputSR)
    {
        std::vector<float *> results;
        MyTestFeatureExtractor -> chooseFeature(2);
        MyTestFeatureExtractor -> initFeatureExtractor();
        MyTestFeatureExtractor -> setTest(true);
        float **myTestSig;
        myTestSig = new float* [iNumChannel];
        for (int c = 0; c < iNumChannel; c++)
        {
            myTestSig[c] = new float [NumFFT];
            CSignalGen::generateDc(myTestSig[c], NumFFT);
        }
        
        MyTestFeatureExtractor -> featureExtract(myTestSig,results);
        memcpy(MyOutput, results[0], sizeof(float)*iNumChannel);
        for (int i = 0 ; i < iNumChannel; i++) {
            //default kappa = 0.85
            CHECK_EQUAL( (int)(0.85 * (SampleRate/2)),  MyOutput[i]);
        }
        MyTestFeatureExtractor -> destroyFeatureExtractor();
        
    }
    
    
    TEST_FIXTURE(FeatureExt, ZeroInputZC)
    {
        std::vector<float *> results;
        MyTestFeatureExtractor -> chooseFeature(3);
        MyTestFeatureExtractor -> initFeatureExtractor();
        MyTestFeatureExtractor -> setTest(false);
        MyTestFeatureExtractor -> featureExtract(MyData,results);
        memcpy(MyOutput, results[0], sizeof(float)*iNumChannel);
        for (int i = 0 ; i < iNumChannel; i++) {
            //CHECK_CLOSE(0, MyOutput[i], 0.01);
            CHECK_EQUAL(0, MyOutput[i]);
        }
        MyTestFeatureExtractor -> destroyFeatureExtractor();
        
    }
    
    TEST_FIXTURE(FeatureExt, oneAlterZC)
    {
        std::vector<float *> results;
        MyTestFeatureExtractor -> chooseFeature(3);
        MyTestFeatureExtractor -> initFeatureExtractor();
        MyTestFeatureExtractor -> setTest(false);
        float **myTestSig;
        myTestSig = new float* [iNumChannel];
        for (int c = 0; c < iNumChannel; c++)
        {
            myTestSig[c] = new float [NumFFT];
            memset(myTestSig[c], 0.0, sizeof(float)*NumFFT);
            myTestSig[c][0] = 1;
            myTestSig[c][1] = -1;
        }
        
        
        MyTestFeatureExtractor -> featureExtract(myTestSig,results);
        memcpy(MyOutput, results[0], sizeof(float)*iNumChannel);
        for (int i = 0 ; i < iNumChannel; i++) {
            //CHECK_CLOSE(0, MyOutput[i], 0.01);
            CHECK_EQUAL(1/NumFFT, MyOutput[i]);
        }
        MyTestFeatureExtractor -> destroyFeatureExtractor();
        
    }

    
}

#endif //WITH_TESTS