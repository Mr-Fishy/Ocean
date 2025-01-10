#pragma once

#include <phonon.h>

namespace audio{
    class steamaudio{
        public:
            static steamaudio* instance();
        
            static steamaudio* struct_pointer;


        private:
            //makes the instance
            steamaudio();
            ~steamaudio();


            //Do not implement
            steamaudio(const steamaudio&);
            steamaudio& operator=(const steamaudio&);

    };
};