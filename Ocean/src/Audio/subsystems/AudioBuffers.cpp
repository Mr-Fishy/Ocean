#include "AudioBuffers.hpp";
#include "Ocean/Core/Types/Integers.hpp"
#include <phonon.h>


        Ocean::DOGsystem::AudioBuffers::AudioBuffers(int numChannels,int numSamples){
            //Set the setting.
            this->buffer.numChannels = numChannels;
            this->buffer.numSamples = numSamples;
            //Create the channels.
            float* channels[numChannels];
            //Initializes the channels with the correct amount of arrs.
            for(u8 i =0; i<numChannels; i++){
                float fresh_Channel[numSamples];
                channels[i] = fresh_Channel;
            }
            //Assign.
            this->buffer.data = channels;

        }