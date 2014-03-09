
// standard headers

// project headers
#include "MyProjectConfig.h"

#include "ErrorDef.h"

#include "MyProject.h"

static const char*  kCMyProjectBuildDate             = __DATE__;



void LFO::setPara(float amp,float freq, float samplerate)
{
    Amp = amp;
    Freq = freq;
    SampleRate = samplerate;
    time = 0 ;
    
}
int vibrato::round(float x)
{
    int n = static_cast<int>(x);
    if ((x-n)<0.5) {
        return n;
    }
    else return n+1;
}
void vibrato::setParameter(float d,float Mf, float w){
    Modfreq = Mf/samplerate;
    if (w>MAX_BUFFER) {
        w=MAX_BUFFER;
    }
    WIDTH = round(w * samplerate);
    if (d>MAX_BUFFER) {
        d=MAX_BUFFER;
    }
    Delay = d;
    DELAY = round(Delay*samplerate);
    
    sinwav->setPara(WIDTH, Modfreq, samplerate);
    
    
    
}
void vibrato::resetInstance()
{
    for (int i = 0 ; i < iNumChannel; i++) {
        Delay_buffer[i]->resetInstance();
        
    }
    sinwav->reset();
    
}

//void vibrato::process(float **input, float **output, int len)
//{
//    for (int n = 0 ; n<len; n++) {
//        float tap = DELAY+sinwav->getvalue();
//        for (int i = 0 ; i < iNumChannel; i++) {
//            Delay_buffer[i]->setReadIdx(Delay_buffer[i]->getWriteIdx()-tap);
//            Delay_buffer[i]->putPostInc(input[i][n]);
//            output[i][n] = Delay_buffer[i]->getInterpolationPostInc();
//        }
//        
//    }
//}

void vibrato::processBypass(float **input, float **output, int len)
{
    setParameter(Delay, 0, 0);
    process(input, output, len);
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