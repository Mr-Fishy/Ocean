#include "./convert.hpp"
#include <phonon.h>


//Standard input.
sonar::convert::convert(int number_channels, int samples){

    buffer.numChannels = number_channels;
    buffer.numSamples = samples;

}

//This will just pass in the data.
sonar::convert::convert(float* indata,int number_channel, int samples){
    buffer.numChannels = number_channel;
    buffer.numSamples = samples;
    buffer.data = &indata;
}

//Modifies given pointer.
void sonar::convert::interleave(float* outData){

    iplAudioBufferInterleave(*context, &buffer, outData);

    return;
}

//Adds result to internal buffer.
void sonar::convert::deinterleave(float* inData){

    iplAudioBufferDeinterleave(*context, inData, &buffer);

    return;
}

//This should be called if it needs to be used again for something else, basically when its done with its process.
void sonar::convert::wipe(){

    buffer.data = nullptr;
    
    return;
}