#pragma once



#include <phonon.h>
#include "Ocean/Ocean.hpp"

namespace sonar{
    class convert{
        public: 

            /*
            This calls the steam interleave/deinterleave functions :)
             */
            
            void interleave(float* inData);
            void deinterleave(float* outData);

            void wipe();
        private:
            
            IPLAudioBuffer buffer;

            float* inData;


            //Do not implement
            convert(const convert&) = delete;
            convert& operator=(const convert&) = delete;

    };


}