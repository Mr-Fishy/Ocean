#include "audio.hpp"
#include <phonon.h>


audio::steamaudio* audio::steamaudio::instance(IPLuint32 steamversion,IPLSIMDLevel smidlevel, IPLAllocateFunction allocation = nullptr, IPLFreeFunction freemem = nullptr){

    if(audio::steamaudio::struct_pointer == nullptr){
        struct_pointer = new steamaudio();
    }
    return struct_pointer;

}
audio::steamaudio::steamaudio(){

}