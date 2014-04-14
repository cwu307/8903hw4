
#include <iostream>

#include "MyProjectConfig.h"

// include project headers
#include "AudioFileIf.h"

#include "MyProject.h"

#define WITH_FLOATEXCEPTIONS
#define WITH_MEMORYCHECK

// include exception header
#if (defined(WITH_FLOATEXCEPTIONS) && !defined(NDEBUG) && defined (GTCMT_WIN32))
#include <float.h>
#endif // #ifndef WITHOUT_EXCEPTIONS

// include memory leak header
#if (defined(WITH_MEMORYCHECK) && !defined(NDEBUG) && defined (GTCMT_WIN32))
#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#else
#include <stdlib.h>
#endif

using std::cout;
using std::endl;

// local function declarations
void    showClInfo ();

void    getClArgs (std::string &sInputFilePath, std::string &sOutputFilePath, int argc, char* argv[]);

/////////////////////////////////////////////////////////////////////////////////
// main function
int main(int argc, char* argv[])
{
    std::string             sInputFilePath,
                            sOutputFilePath;

    float                   **ppfAudioData  = 0;
    float                   **ppfOutput     = 0;
    static const int        kBlockSize      = 1024;
    int                     iNumFFT         = 1024;
    
    
    CAudioFileIf            *phInputFile    = 0;
    std::fstream            hOutputFile;
    CAudioFileIf::FileSpec_t stFileSpec;
    CMyProject              *phMyProject = 0;
    float                   *pfSizeOfBlock;
    float                   *pfSizeOfResults;
    float                   **ppfAllResults;


    // detect memory leaks in win32
#if (defined(WITH_MEMORYCHECK) && !defined(NDEBUG) && defined (GTCMT_WIN32))
    // set memory checking flags
    int iDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    iDbgFlag       |= _CRTDBG_CHECK_ALWAYS_DF;
    iDbgFlag       |= _CRTDBG_LEAK_CHECK_DF;
    _CrtSetDbgFlag( iDbgFlag );
#endif

    // enable floating point exceptions in win32
#if (defined(WITH_FLOATEXCEPTIONS) && !defined(NDEBUG) && defined (GTCMT_WIN32))
    // enable check for exceptions (don't forget to enable stop in MSVC!)
    _controlfp(~(_EM_INVALID | _EM_ZERODIVIDE | _EM_OVERFLOW | _EM_UNDERFLOW | _EM_DENORMAL), _MCW_EM) ;
#endif // #ifndef WITHOUT_EXCEPTIONS

    showClInfo ();

    // parse command line arguments
    getClArgs (sInputFilePath, sOutputFilePath, argc, argv);

    
    // open the input wave file
    CAudioFileIf::createInstance(phInputFile);
    phInputFile->openFile(sInputFilePath, CAudioFileIf::kFileRead);
    if (!phInputFile->isOpen())
    {
        cout << "Wave file open error!";
        return -1;
    }
    phInputFile->getFileSpec(stFileSpec);

    // open the output text file
     hOutputFile.open (sOutputFilePath.c_str(), std::ios::out);
     if (!hOutputFile.is_open())
     {
         cout << "Text file open error!";
         return -1;
     }

    // allocate audio data buffer
    ppfAudioData            = new float* [stFileSpec.iNumChannels];
    for (int i = 0; i < stFileSpec.iNumChannels; i++)
        ppfAudioData[i] = new float [kBlockSize];
    
    CMyProject::createInstance(phMyProject);
    phMyProject->initInstance(iNumFFT, kBlockSize, stFileSpec.fSampleRateInHz, stFileSpec.iNumChannels);
    
    pfSizeOfBlock = new float [2];
    pfSizeOfResults = new float [2];
    for (int i = 0; i < 2; i++)
    {
        pfSizeOfBlock[i] = 0;
        pfSizeOfResults[i]  = 0;
    }
    

    // read wave
    while (!phInputFile->isEof())
    {
        
        //prepare to process
        int iNumFrames = kBlockSize;
        phMyProject->getSizeOfBlock(pfSizeOfBlock, iNumFrames);
        
        //create a buffer for output
        ppfOutput = new float* [stFileSpec.iNumChannels];
        for (int c = 0; c < stFileSpec.iNumChannels; c++)
        {
            ppfOutput[c] = new float [(int)pfSizeOfBlock[1]];
        }
        
        //read in one block of signal (simulate process call)
        phInputFile->readData(ppfAudioData, iNumFrames);
        phMyProject->process(ppfAudioData, ppfOutput, iNumFrames);
        
        
        //write ppfOutput into txt file
        for (int i = 0; i < (int)pfSizeOfBlock[1]; i++)
        {
            
            for (int c = 0; c < stFileSpec.iNumChannels; c++)
            {
                hOutputFile << "\t" << ppfOutput[c][i];
                //cout << ppfOutput[c][i] << endl;
            }
            hOutputFile << endl;
        }
        
        //delete the buffer
        for (int c = 0 ; c < stFileSpec.iNumChannels; c++)
        {
            delete [] ppfOutput[c];
        }
        delete [] ppfOutput;
    }
    

    
    phMyProject->getSizeOfResult(pfSizeOfResults);
    ppfAllResults = new float *[(int)pfSizeOfResults[0]];
    for (int i = 0; i < pfSizeOfResults[0]; i++)
    {
        ppfAllResults[i] = new float [(int)pfSizeOfResults[1]];
    }
    phMyProject->getResult(ppfAllResults);
    
    for (int i = 0; i < (int)pfSizeOfResults[1]; i++)
    {
        for (int c = 0; c < (int)pfSizeOfResults[0]; c++)
        {
            //hOutputFile << "\t" << ppfOutput[c][i];
            cout << ppfAllResults[c][i] << "\t" ;
        }
        //hOutputFile << endl;
        cout << endl;
    }
    
    
    

    
    // close the files
    CAudioFileIf::destroyInstance(phInputFile);
    hOutputFile.close();
    CMyProject::destroyInstance(phMyProject);

    // free memory
    delete [] pfSizeOfBlock;
    delete [] pfSizeOfResults;
    for (int i = 0; i < stFileSpec.iNumChannels; i++)
        delete [] ppfAudioData[i];
    delete [] ppfAudioData;
    ppfAudioData = 0;

    return 0;
    
}


void     showClInfo()
{
    cout << "GTCMT template app" << endl;
    cout << "(c) 2013 by Alexander Lerch" << endl;
    cout    << "V" 
        << CMyProject::getVersion (CMyProject::kMajor) << "." 
        << CMyProject::getVersion (CMyProject::kMinor) << "." 
        << CMyProject::getVersion (CMyProject::kPatch) << ", date: " 
        << CMyProject::getBuildDate () << endl;
    cout  << endl;

    return;
}

void getClArgs( std::string &sInputFilePath, std::string &sOutputFilePath, int argc, char* argv[] )
{
    if (argc > 1)
        sInputFilePath.assign (argv[1]);
    if (argc > 2)
        sOutputFilePath.assign (argv[2]);
}
