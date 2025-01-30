#include "audio.hpp"

#include "Ocean/Primitives/Assert.hpp"

#include <phonon.h>


sonar::steamaudio* sonar::steamaudio::instance(IPLuint32 steamversion,IPLSIMDLevel simdlevel, IPLAllocateFunction allocation = nullptr, IPLFreeFunction freemem = nullptr){

    if(sonar::steamaudio::struct_pointer == nullptr){

        struct_pointer = new steamaudio(steamversion, simdlevel, allocation, freemem);

    }


    return struct_pointer;

}

sonar::steamaudio* sonar::steamaudio::getinstance(){

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

        std::cout<<"Success";

        break;

        case IPL_STATUS_FAILURE:
        case IPL_STATUS_INITIALIZATION:
        case IPL_STATUS_OUTOFMEMORY:

        default:

            std::cout<<"Unknown error occured, throwing exception";

        OASSERTM(false, "Error has occured");    
        
    }
    std::cout<<std::endl;

}