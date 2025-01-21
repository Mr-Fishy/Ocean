#pragma once



#include <phonon.h>
#include "Ocean/Ocean.hpp"

namespace sonar{
    class convert{
        public: 
            convert(int number_channels =2, int samples = 1024);
            ~convert();
            /*
            This calls the steam interleave/deinterleave functions :)
             */
            
            void interleave(float* outData);
            void deinterleave(float* inData);



            void wipe();
        private:

            IPLAudioBuffer buffer;

            float* inData;


            //Do not implement
            convert(const convert&) = delete;
            convert& operator=(const convert&) = delete;

    };


}