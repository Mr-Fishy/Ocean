#include "audio.hpp"

#include "Ocean/Primitives/Assert.hpp"
#include "steamaudio_dataholding.hpp"


#include <cstdlib>
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

    //This should just break everything, as the audio system does not exist anymore.
    if(audioerror(error)){
        exit(EXIT_FAILURE);
    }
    std::cout<<std::endl;

}