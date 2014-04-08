
// standard headers

// project headers
#include "MyProjectConfig.h"

#include "ErrorDef.h"

#include "MyProject.h"

static const char*  kCMyProjectBuildDate             = __DATE__;


CMyProject::CMyProject ()
{
    // this never hurts
    this->resetInstance ();
}


CMyProject::~CMyProject ()
{
    this->resetInstance ();
}

const int  CMyProject::getVersion (const Version_t eVersionIdx)
{
    int iVersion = 0;

    switch (eVersionIdx)
    {
    case kMajor:
        iVersion    = MyProject_VERSION_MAJOR; 
        break;
    case kMinor:
        iVersion    = MyProject_VERSION_MINOR; 
        break;
    case kPatch:
        iVersion    = MyProject_VERSION_PATCH; 
        break;
    case kNumVersionInts:
        iVersion    = -1;
        break;
    }

    return iVersion;
}
const char*  CMyProject::getBuildDate ()
{
    return kCMyProjectBuildDate;
}

Error_t CMyProject::createInstance (CMyProject*& pCMyProject)
{
    pCMyProject = new CMyProject ();

    if (!pCMyProject)
        return kUnknownError;


    return kNoError;
}

Error_t CMyProject::destroyInstance (CMyProject*& pCMyProject)
{
    if (!pCMyProject)
        return kUnknownError;
    
    pCMyProject->resetInstance ();
    
    delete pCMyProject;
    pCMyProject = 0;

    return kNoError;

}

Error_t CMyProject::initInstance(int NumFFT, int Blocksize, float SampleRate, int NumChannel)
{
    // allocate memory
    OutputBuffer = new float*[NumChannel];
    Spect = new CFft::complex_t*[NumChannel];
    SpectMagnit = new float*[NumChannel];
    // initialize variables and buffers
    
    iNumChannal = NumChannel;
    iNumFFT = NumFFT;
    iBlocksize = Blocksize;
    
    MyFFT = new CFft*[NumChannel];
    MyFeatureExtractor = new FeatureExtractor*[NumChannel];
    MyInputBuff = new CInputBuffSrc<float>(iNumChannal,Blocksize*2,iNumFFT-1);
    
    for (int i = 0 ;  i < NumChannel; i++) {
        CFft::createInstance(MyFFT[i]);
        MyFFT[i] -> initInstance(Blocksize,NumFFT);
        MyFeatureExtractor[i] = new FeatureExtractor(SampleRate,Blocksize);
        OutputBuffer[i] = new float[Blocksize];
        Spect[i] = new CFft::complex_t[iNumFFT];
        SpectMagnit[i] = new float[iNumFFT];
    }
    return kNoError;
}

Error_t CMyProject::resetInstance ()
{
    // reset buffers and variables to default values
    for (int i = 0 ; i < iNumChannal; i++) {
        MyFFT[i] -> resetInstance();
        MyFeatureExtractor[i] -> reset();
    }
    MyInputBuff -> reset();

    return kNoError;
}

Error_t CMyProject::process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames)
{
    MyInputBuff -> setDataPtr2Hold(ppfInputBuffer, iNumberOfFrames);
    while (MyInputBuff -> getBlock(OutputBuffer, iNumFFT)) {
        for (int i = 0 ; i < iNumChannal ; i++) {
            MyFFT[i] -> doFft(Spect[i], OutputBuffer[i]);
            
        }
    }
}
