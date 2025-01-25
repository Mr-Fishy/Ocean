#pragma once

#include <phonon.h>
#include <iostream>

namespace sonar{
    class steamaudio{
        public:
            static steamaudio* instance(IPLuint32 steamversion,IPLSIMDLevel smidlevel,IPLAllocateFunction allocation , IPLFreeFunction freemem);
                    
            static steamaudio* getinstance();
            static IPLContext* context;
            static IPLContextSettings settings;


        private:
            //makes the instance
            steamaudio(IPLuint32 steamversion,IPLSIMDLevel smidlevel,IPLAllocateFunction allocation , IPLFreeFunction freemem);
            ~steamaudio();

            static steamaudio* struct_pointer;
            //Do not implement
            steamaudio(const steamaudio&) = delete;
            steamaudio& operator=(const steamaudio&) = delete;

    };
};