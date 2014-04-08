#if !defined(__MyProject_hdr__)
#define __MyProject_hdr__

#include "ErrorDef.h"
#include "InputBuffSrc.h"
#include "../../SharedSources/Dsp/Fft.h"
#include "FeatureExtractor.h"

class CMyProject
{
public:
    /*! version number */
    enum Version_t
    {
        kMajor,                         //!< major version number
        kMinor,                         //!< minor version number
        kPatch,                         //!< patch version number

        kNumVersionInts
    };

    static const int  getVersion (const Version_t eVersionIdx);
    static const char* getBuildDate ();

    static Error_t createInstance (CMyProject*& pCKortIf);
    static Error_t destroyInstance (CMyProject*& pCKortIf);
    
    Error_t initInstance (int NumFFT, int Blocksize, float SampleRate, int NumChannel=1);
    Error_t resetInstance ();
    
    virtual Error_t process (float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames);
    
protected:
    CMyProject ();
    virtual ~CMyProject ();
    
    CInputBuffSrc<float> * MyInputBuff;
    CFft ** MyFFT;
    FeatureExtractor ** MyFeatureExtractor;
    float ** OutputBuffer;
    CFft::complex_t ** Spect;
    float ** SpectMagnit ; 
    
    int iNumChannal;
    int iNumFFT;
    int iBlocksize;
    
};

#endif // #if !defined(__MyProject_hdr__)



