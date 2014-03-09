#include "MyProjectConfig.h"

#ifdef WITH_TESTS
#include <cassert>
#include <cstdio>

#include "UnitTest++.h"
#include "SignalGen.h"

#include "MyProject.h"

SUITE(MyProject)
{
    struct MyProjectData
    {
        MyProjectData() 
        {
            testPPM = new PPM(defaultAttack,defaultRelease,defaulSampleRate,iNumChannel);
            testData = new float[iBlocksize];
            CSignalGen::generateSine(testData, 20.F, defaulSampleRate, iBlocksize, .7F, static_cast<float>(M_PI_2));
            output = new float [iBlocksize];
            alfa1 = 1 - exp(-2.2/(defaulSampleRate*defaultRelease));
            alfa2 = 1 - exp(-2.2/(defaulSampleRate*defaultAttack));
            
        }

        ~MyProjectData() 
        {
            delete testPPM;
            delete []testData;
        }
        PPM * testPPM;
        float * testData;
        float * output;
        float * tmpInput;
        float * tmpOutput;
        float defaultAttack = 0.3;
        float defaultRelease = 1.0;
        float defaulSampleRate = 44100;
        int iBlocksize = 2048;
        int iNumChannel = 1;
        float alfa1;
        float alfa2;
    };

    TEST_FIXTURE(MyProjectData, NoTest)
    {
        CHECK_EQUAL (0 ,0);
    }
    TEST_FIXTURE(MyProjectData, ZeroInput)
    {
        memset(testData, 0, sizeof(float)*iBlocksize);
        tmpInput = new float[iNumChannel];
        tmpOutput = new float[iNumChannel];
        for (int i = 0 ; i < iBlocksize; i++) {
            for (int j = 0 ; j < iNumChannel; j++) {
                tmpInput[j] = testData[i];
            }
            testPPM -> process(tmpInput, tmpOutput);
            output [i] = tmpOutput[j];
            CHECK_EQUAL(output[i], testData[i]);
        }
        delete [] tmpInput;
        delete [] tmpOutput;

        
    }
    TEST_FIXTURE(MyProjectData, DC)
    {
        memset(testData, 1, sizeof(float)*iBlocksize);
        tmpInput = new float[iNumChannel];
        tmpOutput = new float[iNumChannel];
        for (int i = 0 ; i < iBlocksize; i++) {
            for (int j = 0 ; j < iNumChannel; j++) {
                tmpInput[j] = testData[i];
            }
            testPPM -> process(tmpInput, tmpOutput);
            output [i] = tmpOutput[j];
            CHECK_EQUAL(output[i], testData[i]);
        }
        delete [] tmpInput;
        delete [] tmpOutput;
    }
    TEST_FIXTURE(MyProjectData, MultiChannel)
    {
        float ** MultiInput;
        float ** MultiOutput;
        MultiInput = new float *[2];
        MultiOutput = new float *[2];
        
        for (int i = 0 ; i < 2 ; i ++) {
            MultiInput[i] = new float(iBlocksize);
            CSignalGen::generateSine(MultiInput[i], 20.F, defaulSampleRate, iBlocksize, .7F, static_cast<float>(M_PI_2));

            MultiOutput[i] = new float[iBlocksize];
            
        }
        tmpInput = new float[iNumChannel];
        tmpOutput = new float[iNumChannel];
        for (int i = 0 ; i < iBlocksize; i++) {
            for (int j = 0 ; j < iNumChannel; j++) {
                tmpInput[j] = testData[i];
            }
            testPPM -> process(tmpInput, tmpOutput);
            output [i] = tmpOutput[j];
        }
        
        delete []tmpInput;
        delete [] tmpOutput;
        tmpInput = 0 ;
        tmpOutput = 0 ;
        iNumChannel = 2;
        delete testPPM;
        tmpInput = new float[iNumChannel];
        tmpOutput = new float[iNumChannel];
        testPPM = new PPM(defaultAttack,defaultRelease,iNumChannel);
        
        for (int i = 0 ; i < iBlocksize; i++) {
            for (int j = 0 ; j < iNumChannel; j++) {
                tmpInput[j] = MultiInput[j][i];
            }
            testPPM -> process(tmpInput, tmpOutput);
            for (int j = 0 ;  j < iNumChannel; j++) {
                MultiInput[j][i] = tmpOutput[j];
            }
            CHECK_EQUAL(MultiOutput[1][i], output[i]);
            
        }
        for (int i = 0 ;  i < iNumChannel; i++) {
            delete [] MultiOutput[i];
            delete [] MultiInput[i];
        }
        delete []tmpInput;
        delete [] tmpOutput;
        delete MultiInput;
        delete MultiOutput;
    }
    TEST_FIXTURE(MyProjectData, testAttack)
    {
        for (int i = 0 ; i < iBlocksize; i++) {
            testData[i] = (float)(i)/10;
        }
        tmpInput = new float[iNumChannel];
        tmpOutput = new float(iNumChannel);
        float * Old = new float(iNumChannel);
        for (int i = 0 ; i < iBlocksize; i++) {
            for (int j = 0 ; j < iNumChannel; j++) {
                tmpInput[j] = testData[i];
            }
            testPPM -> process(tmpInput, tmpOutput);
            testPPM -> getOld(Old);
            float expect = alfa2*fabs(tmpInput[0]) + (1-alfa2)*Old[0];
            CHECK_EQUAL(tmpOutput[0], expect);
        }
        delete []tmpInput;
        delete [] tmpOutput;
        delete [] Old;
    }
    TEST_FIXTURE(MyProjectData, testRelease)
    {
        for (int i = 0 ; i < iBlocksize; i++) {
            testData[i] = (float)(iBlocksize-i)/10;
        }
        tmpInput = new float[iNumChannel];
        tmpOutput = new float(iNumChannel);
        float * Old = new float(iNumChannel);
        for (int i = 0 ; i < iBlocksize; i++) {
            for (int j = 0 ; j < iNumChannel; j++) {
                tmpInput[j] = testData[i];
            }
            testPPM -> process(tmpInput, tmpOutput);
            testPPM -> getOld(Old);
            float expect = (1-alfa1)*Old[i];
            CHECK_EQUAL(tmpOutput[0], expect);
        }
        delete []tmpInput;
        delete [] tmpOutput;
        delete [] Old;

    }
}

#endif //WITH_TESTS