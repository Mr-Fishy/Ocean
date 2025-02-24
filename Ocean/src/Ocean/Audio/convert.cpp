
#include "Ocean/Audio/convert.hpp"
//
void sonar::convert::allocatebuffer(std::string name, IPLAudioBuffer &buff){

    IPLAudioBuffer* buffer = &buff;
    IPLerror err =iplAudioBufferAllocate(*sonar::steamaudio::context, buffer->numChannels, buffer->numSamples, buffer);
    sonar::audioerror(err);
    sonar::global_audio_context::tmpbuffer[name.c_str()] = MakeRef<IPLAudioBuffer>(buffer);

}
//

void sonar::convert::deinterleave(std::string name, float* indata, int channels, int samples){
    IPLAudioBuffer* buffer = new IPLAudioBuffer;
    buffer->numChannels = channels;
    buffer->numSamples = samples;

    Ref temp = MakeRef<IPLAudioBuffer>(buffer);
    sonar::global_audio_context::outbuffer[name.c_str()] =temp;
    iplAudioBufferDeinterleave(*sonar::steamaudio::context, indata, temp.get());
}
//
void sonar::convert::deinterleave(std::string name, float* indata, IPLAudioBuffer* buff){
    Ref temp = MakeRef<IPLAudioBuffer>(buff);

    sonar::global_audio_context::outbuffer[name.c_str()] = temp;
    iplAudioBufferDeinterleave(*sonar::steamaudio::context, indata, temp.get());
}
//
void sonar::convert::interleave(std::string name, float* outdata){
        
    iplAudioBufferInterleave(*sonar::steamaudio::context, sonar::global_audio_context::inbuffers[name.c_str()].get(), outdata);

}
