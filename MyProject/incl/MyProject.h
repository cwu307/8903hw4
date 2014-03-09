#if !defined(__MyProject_hdr__)
#define __MyProject_hdr__

#include "ErrorDef.h"
#include "RingBuffer.h"
#include<cmath>

// the LFO class
class LFO{
public:
    LFO(float amp,float freq, float samplerate, float phase=0):Freq(freq),Amp(amp),SampleRate(samplerate),Phase(phase),time(0){
        period_samples = SampleRate/Freq;
    }
    virtual ~LFO(){
    }
    // get the sin sample value of the LFO
    float getvalue()
    {
        //time = time % period_samples;
        return Amp*sin(Freq*2*pi*(time++)/SampleRate+Phase);
    }
    // set the LFP parameter
    void setPara(float amp,float freq, float samplerate);
    // set the timer to 0 and 0 phase
    float getPhase() const
    {
        return float(time)*Freq/SampleRate;
    }
    void reset()
    {
        time = 0 ;
        Phase = 0;
    }
private:
    const double pi = 3.1415926;
    int period_samples;
    float Freq;
    float Phase = 0 ;
    float Amp;
    float SampleRate;
    long long time;
    
};

// the vibrato class
class vibrato{
public:
    struct Para{
        int samplerate;
        float Modfreq;
        float Delay;
        float WIDTH;
        int iNumChannel;
    };
    vibrato(int sp, float d, float Mf, float w):samplerate(sp),Delay(d),Modfreq(Mf/samplerate),iNumChannel(2){
        // if the user set delay and width(in sec) are more than max, then truncate them with max
        if (Delay>MAX_BUFFER) {
            Delay = MAX_BUFFER;
        }
        if (w>MAX_BUFFER) {
            w= MAX_BUFFER;
        }
        DELAY = round(Delay * samplerate);
        WIDTH = round(w * samplerate);
        // assure delay is bigger than width
        assert(DELAY>=WIDTH);
        
        // every time just allocate the max length of delay buffer
        L = 2+MAX_BUFFER*2*samplerate;
        
        // init the LPO
        Delay_buffer = new CRingBuffer<float> * [iNumChannel];
        sinwav = new LFO(WIDTH,Mf,samplerate);
        
        for (int i = 0 ; i<iNumChannel; i++) {
            
            Delay_buffer[i] = new CRingBuffer<float>(L);
            Delay_buffer[i]->resetInstance();
        }
        
        
    }
    virtual ~vibrato(){
        for (int i = 0 ; i < iNumChannel; i++) {
            delete Delay_buffer[i];
            
        }
        delete  Delay_buffer;
        delete sinwav;
        Delay_buffer = 0;
        sinwav = 0 ;
    }
    // get the parameter of the state
    Para getParameters() const
    {
        Para p;
        p.samplerate = samplerate;
        p.WIDTH = WIDTH;
        p.Delay = Delay;
        p.Modfreq = Modfreq;
        return p;
    }
    // the process function with input and ouput buffer and the length of them
    void process(float ** input, float ** output, int len);
    void processBypass(float ** input, float** output, int len);
    void setParameter(float d,float Mf, float w);
    // reset the delay-buffer and the LPO
    void resetInstance();
    float getPhase() const
    {
        return sinwav->getPhase();
    }
private:
    LFO * sinwav = 0 ;
    
    // THE max delay and width in second
    const int MAX_BUFFER = 3;
    int round(float x);
    CRingBuffer<float> **Delay_buffer=0;
    int samplerate;
    float Modfreq;
    float Delay;
    int DELAY;
    int WIDTH;
    int L;
    int iNumChannel;
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



