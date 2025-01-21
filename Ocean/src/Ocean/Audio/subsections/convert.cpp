#include "convert.hpp"

sonar::convert::convert(int number_channels, int samples){
    buffer.numChannels = number_channels;
    buffer.numSamples = samples;
}
sonar::convert::~convert(){

}


void sonar::convert::interleave(float* inData){

    return;
}

void sonar::convert::deinterleave(float* outData){

    return;
}
//This should be called if it needs to be used again for something else, basically when its done with its process.
void sonar::convert::wipe(){

    return;
}