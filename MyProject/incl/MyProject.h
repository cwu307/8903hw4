#if !defined(__MyProject_hdr__)
#define __MyProject_hdr__

#include "ErrorDef.h"

class CLfo;
template <class T>
class CRingBuffer;

class CMyProject
{
public:
    /*! version number */
    enum Version_t
    {
        kMajor, //!< major version number
        kMinor, //!< minor version number
        kPatch, //!< patch version number

        kNumVersionInts
    };

    enum VibratoParam_t
    {
        kParamModWidthInS,
        kParamModFreqInHz,

        kNumVibratoParams
    };
    static const int getVersion (const Version_t eVersionIdx);
    static const char* getBuildDate ();

    static Error_t createInstance (CMyProject*& pCMyProject);
    static Error_t destroyInstance (CMyProject*& pCMyProject);

    Error_t initInstance (float fMaxModWidthInS, float fSampleRateInHz, int iNumChannels);
    Error_t resetInstance ();

    Error_t setParam (VibratoParam_t eParam, float fParamValue);
    float getParam (VibratoParam_t eParam) const;

    Error_t process (float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames);

protected:
    CMyProject ();
    virtual ~CMyProject ();

private:
    bool isInParamRange (VibratoParam_t eParam, float fValue);

    bool m_bIsInitialized;

    CLfo *m_pCLfo;
    CRingBuffer<float> **m_ppCRingBuff;

    float   m_fSampleRate;
    int     m_iNumChannels;
    float m_aafParamRange[kNumVibratoParams][2];
};


class PPM{
public:
    PPM(float attackTime, float releaseTime, float fs, int iNumChannel):attack_time(attackTime),release_time(releaseTime),sample_rate(fs){
        this->iNumChannel = iNumChannel;
        alfa1 = 1 - exp(-2.2/(sample_rate*release_time));
        alfa2 = 1 - exp(-2.2/(sample_rate*attack_time));
        vppm_old = new float[iNumChannel];
        
        memset(vppm_old, 0, sizeof(float)*iNumChannel);
    }
    virtual ~PPM(){
        delete [] vppm_old;
    }
    void process(float *x, float * output);
    void getOld(float *old);
    
private:
    float *vppm_old;
    
    float release_time;
    float attack_time;
    float alfa1;
    float alfa2;
    float sample_rate;
    int iNumChannel;
};


#endif // #if !defined(__MyProject_hdr__)