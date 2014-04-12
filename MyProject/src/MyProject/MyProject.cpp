
// standard headers

// project headers
#include "MyProjectConfig.h"

#include "ErrorDef.h"

#include "MyProject.h"


static const char*  kCMyProjectBuildDate             = __DATE__;


CMyProject::CMyProject ()
{
    // this hurts
    // this->resetInstance ();
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
    
    //pCMyProject->resetInstance ();
    
    delete pCMyProject;
    pCMyProject = 0;
    

    return kNoError;

}

Error_t CMyProject::initInstance(int NumFFT, int Blocksize, float SampleRate, int NumChannel)
{
    assert(NumFFT <= Blocksize);
    // allocate memory
    OutputBuffer = new float*[NumChannel];
    
    
    // initialize variables and buffers
    
    iNumChannal = NumChannel;
    iNumFFT = NumFFT;
    iBlocksize = Blocksize;
    
    pfSizeOfResult = new float [2];
    for (int i = 0; i < 2; i++)
    {
        pfSizeOfResult[i] = 0;
    }
    
    MyFeatureExtractor = new FeatureExtractor(SampleRate, iNumFFT,iNumChannal);
    
    MyFeatureExtractor->chooseFeature(0);
    MyFeatureExtractor->chooseFeature(1);
    MyFeatureExtractor->chooseFeature(2);
    MyFeatureExtractor->chooseFeature(3);
    
    MyFeatureExtractor->initFeatureExtractor();
    
    
    MyInputBuff = new CInputBuffSrc<float>(iNumChannal,Blocksize*2,iNumFFT-1);
    
    for (int i = 0 ;  i < NumChannel; i++) {
        
        OutputBuffer[i] = new float[iNumFFT];
    }
    return kNoError;
}

Error_t CMyProject::resetInstance ()
{
    // reset buffers and variables to default values
    MyFeatureExtractor->destroyFeatureExtractor();
    delete MyFeatureExtractor;
    delete [] pfSizeOfResult;
    MyInputBuff -> reset();

    return kNoError;
}

Error_t CMyProject::process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames)
{
    MyInputBuff -> setDataPtr2Hold(ppfInputBuffer, iNumberOfFrames);
    
    int repeat = 0 ;
    while (MyInputBuff -> getBlock(OutputBuffer, iNumFFT, iNumFFT))
    {
        MyFeatureExtractor -> featureExtract(OutputBuffer, FeatureVector);
        repeat ++;
    }
    MyInputBuff -> releaseDataPtr();
    
    for (int c = 0; c < iNumChannal; c++)
    {
        for (int i = 0; i < repeat * MyFeatureExtractor->getChosenFeatureNum(); i++)
        {
            ppfOutputBuffer[c][i] = FeatureVector[c][i];
        }
    }
    
    FeatureVector.clear();
    
    return kNoError;
}

void CMyProject::getSizeOfResult(float *pfSizeOfResult, int iNumOfFrames)
{
    int iRepeat = iNumOfFrames / iNumFFT;
    int iNumOfFeaetures = MyFeatureExtractor->getChosenFeatureNum();
    pfSizeOfResult[0] = iNumChannal;
    pfSizeOfResult[1] = iRepeat * iNumOfFeaetures;
}









