#pragma once



#include <phonon.h>
namespace sonar{
    class audiobuffer{
        public: 

            void interleave();
            void deinterleave();
        private:

            IPLAudioBuffer buffer;

            //Do not implement
            audiobuffer(const audiobuffer&) = delete;
            audiobuffer& operator=(const audiobuffer&) = delete;

    };


}