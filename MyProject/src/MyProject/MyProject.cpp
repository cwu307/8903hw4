
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
    
    pfSizeOfBlock = new float [2];
    m_pfSizeOfResult = new float [2];
    for (int i = 0; i < 2; i++)
    {
        pfSizeOfBlock[i] = 0;
        m_pfSizeOfResult[i] = 0;
    }
    
    MyFeatureExtractor = new FeatureExtractor(SampleRate, iNumFFT,iNumChannal);
    
    MyFeatureExtractor->chooseFeature(0);
    MyFeatureExtractor->chooseFeature(1);
    MyFeatureExtractor->chooseFeature(2);
    MyFeatureExtractor->chooseFeature(3);
    
    MyFeatureExtractor->initFeatureExtractor();
    
    
    MyInputBuff = new CInputBuffSrc<float>(iNumChannal,Blocksize*2);
    
    for (int i = 0 ;  i < NumChannel; i++)
    {
        OutputBuffer[i] = new float[iNumFFT];
    }
    return kNoError;
    
    //intialize col & row counter
    iRowCount = 0;
}

Error_t CMyProject::resetInstance ()
{
    // reset buffers and variables to default values
    MyFeatureExtractor->destroyFeatureExtractor();
    delete MyFeatureExtractor;
    delete [] pfSizeOfBlock;
    delete [] m_pfSizeOfResult;
    MyInputBuff -> reset();
    iRowCount = 0;    
    return kNoError;
}

Error_t CMyProject::process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames)
{
    MyInputBuff -> setDataPtr2Hold(ppfInputBuffer, iNumberOfFrames);
    
    int repeat = 0 ;
    while (MyInputBuff -> getBlock(OutputBuffer, iNumFFT, iNumFFT-1))
    {
        MyFeatureExtractor -> featureExtract(OutputBuffer, FeatureVector);
        repeat ++;
        
        MyFeatureExtractor -> featureExtract(OutputBuffer, AllFeatureVector);
        iRowCount++;
    }
    MyInputBuff -> releaseDataPtr();


    for (int i = 0; i < repeat * MyFeatureExtractor->getChosenFeatureNum(); i++)
    {
        for (int c = 0; c < iNumChannal; c++)
        {
            ppfOutputBuffer[c][i] = FeatureVector[i][c];

        }
        //AllFeatureVector.push_back(FeatureVector[i]);
    }
    
    FeatureVector.clear();
    
    return kNoError;
}

void CMyProject::getSizeOfBlock(float *pfSizeOfBlock, int iNumOfFrames)
{
    int iRepeat = iNumOfFrames / iNumFFT;
    int iNumOfFeaetures = MyFeatureExtractor->getChosenFeatureNum();
    pfSizeOfBlock[0] = iNumChannal;
    pfSizeOfBlock[1] = iRepeat * iNumOfFeaetures;
}

void CMyProject::getSizeOfResult(float *pfSizeOfResult)
{
    pfSizeOfResult[0] = iNumChannal; //Channel
    pfSizeOfResult[1] = iRowCount * MyFeatureExtractor->getChosenFeatureNum(); //features
    m_pfSizeOfResult[0] = pfSizeOfResult[0];
    m_pfSizeOfResult[1] = pfSizeOfResult[1];
}

void CMyProject::getResult(float **ppfAllResults)
{
    for (int i = 0; i < m_pfSizeOfResult[1]; i++)
    {
        for (int c = 0; c < m_pfSizeOfResult[0]; c++)
        {
            ppfAllResults[c][i] = AllFeatureVector[i][c];
        }
    }
}





