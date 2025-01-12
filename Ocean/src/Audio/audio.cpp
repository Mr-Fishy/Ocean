#include "audio.hpp"
#include <phonon.h>

sonar::steamaudio* sonar::steamaudio::instance(IPLuint32 steamversion,IPLSIMDLevel simdlevel, IPLAllocateFunction allocation = nullptr, IPLFreeFunction freemem = nullptr){

    if(sonar::steamaudio::struct_pointer == nullptr){
        struct_pointer = new steamaudio(steamversion, simdlevel, allocation, freemem);
    }
    return struct_pointer;

}
sonar::steamaudio::steamaudio(IPLuint32 steamversion,IPLSIMDLevel simdlevel,IPLAllocateFunction allocation , IPLFreeFunction freemem){
    //initialize
    steamaudio::context = nullptr;
    steamaudio::settings = IPLContextSettings{};
    //modify
    steamaudio::settings.version =steamversion;
    steamaudio::settings.simdLevel = simdlevel;
    steamaudio::settings.allocateCallback = allocation;
    steamaudio::settings.freeCallback = freemem;
    //create
    IPLerror error = iplContextCreate(&steamaudio::settings, steamaudio::context);
    switch(error){
        case IPL_STATUS_SUCCESS:
        
        break;
        case IPL_STATUS_FAILURE:

        break;
        case IPL_STATUS_INITIALIZATION:

        break;
        case IPL_STATUS_OUTOFMEMORY:

        break;
    }
}