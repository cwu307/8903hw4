
// standard headers

// project headers
#include "MyProjectConfig.h"

#include "ErrorDef.h"

#include "RingBuffer.h"
#include "Dsp.h"

#include "MyProject.h"

static const char*  kCMyProjectBuildDate             = __DATE__;


CMyProject::CMyProject () :
    m_bIsInitialized(false),
    m_pCLfo(0),
    m_ppCRingBuff(0),
    m_fSampleRate(44100),
    m_iNumChannels(0)
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

Error_t CMyProject::initInstance( float fMaxModWidthInS, float fSampleRateInHz, int iNumChannels )
{
    // set parameters
    m_fSampleRate       = fSampleRateInHz;
    m_iNumChannels      = iNumChannels;

    // set parameter ranges
    m_aafParamRange[kParamModFreqInHz][0] = 0;
    m_aafParamRange[kParamModFreqInHz][1] = fSampleRateInHz*.5F;
    m_aafParamRange[kParamModWidthInS][0] = 0;
    m_aafParamRange[kParamModWidthInS][1] = fMaxModWidthInS;


    // create instances
    m_ppCRingBuff       = new CRingBuffer<float>*[m_iNumChannels];
    for (int c= 0; c < m_iNumChannels; c++)
    {
        m_ppCRingBuff[c]= new CRingBuffer<float>(CUtil::float2int<int>(fMaxModWidthInS*m_fSampleRate*2+1));
        m_ppCRingBuff[c]->setWriteIdx(CUtil::float2int<int>(fMaxModWidthInS*m_fSampleRate+1));
    }

    m_pCLfo             = new CLfo(m_fSampleRate);

    m_bIsInitialized    = true;

    return kNoError;
}

Error_t CMyProject::resetInstance ()
{
    for (int i = 0; i < kNumVibratoParams; i++)
        setParam((VibratoParam_t)i, 0);

    for (int c= 0; c < m_iNumChannels; c++)
        delete m_ppCRingBuff[c];
    delete [] m_ppCRingBuff;
    m_ppCRingBuff       = 0;
    delete m_pCLfo;
    m_pCLfo             = 0;
    
    m_iNumChannels      = 0;
    m_bIsInitialized    = false;

    return kNoError;
}

Error_t CMyProject::setParam( VibratoParam_t eParam, float fParamValue )
{
    if (!m_bIsInitialized)
        return kNotInitializedError;
    if (!isInParamRange(eParam, fParamValue))
        return kFunctionInvalidArgsError;

    switch (eParam)
    {
    case kParamModFreqInHz:
        return m_pCLfo->setParam(CLfo::kLfoParamFrequency, fParamValue);
    case kParamModWidthInS:
        return m_pCLfo->setParam(CLfo::kLfoParamAmplitude, fParamValue * m_fSampleRate);
    }

    return kNoError;
}

float CMyProject::getParam( VibratoParam_t eParam ) const
{
    switch (eParam)
    {
    case kParamModFreqInHz:
        return m_pCLfo->getParam(CLfo::kLfoParamFrequency);
    case kParamModWidthInS:
        return m_pCLfo->getParam(CLfo::kLfoParamAmplitude) / m_fSampleRate;
    }
    return kNoError;
}


bool CMyProject::isInParamRange( VibratoParam_t eParam, float fValue )
{
    if (fValue < m_aafParamRange[eParam][0] || fValue > m_aafParamRange[eParam][1])
    {
        return false;
    }
    else
    {
        return true;
    }
}

Error_t CMyProject::process( float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames )
{
    if (!ppfInputBuffer || !ppfOutputBuffer || iNumberOfFrames < 0)
        return kFunctionInvalidArgsError;

    for (int i = 0; i < iNumberOfFrames; i++)
    {
        float fOffset = m_pCLfo->getNext();
        for (int c = 0; c < m_iNumChannels; c++)
        {
            m_ppCRingBuff[c]->putPostInc(ppfInputBuffer[c][i]);

            ppfOutputBuffer[c][i]   = m_ppCRingBuff[c]->get(fOffset);
            m_ppCRingBuff[c]->getPostInc(); // dummy call to keep write and read idx in sync
        }
    }

}


void PPM::process(float *x, float *output)
{
    for (int i = 0 ; i < iNumChannel; i++) {
        if (fabs(x[i])<vppm_old[i]) {
            output[i] = (1-alfa1)*vppm_old[i];
            
        }
        else{
            output[i] = alfa2*fabs(x[i]) + (1-alfa2)*vppm_old[i];
        }
        vppm_old[i] =  output[i];
        
    }
    
    
}

void PPM::getOld(float *old)
{
    for (int i =0 ; i < iNumChannel; i++) {
        old[i] = vppm_old[i];
    }
}
