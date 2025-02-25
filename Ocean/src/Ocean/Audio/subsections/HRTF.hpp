#pragma once
#include <phonon.h>
#include "../steamaudio_dataholding.hpp"

namespace sonar{

    //A struct for configuration, might change later
    struct HRTFconfig{
        HRTFconfig();
       
        //types
        IPLAudioSettings audiosetting{};
        IPLHRTFSettings hrtfsetting{};

    };

    
    class HRTF{
        public:
            HRTF(std::string name, sonar::HRTFconfig& config);
            virtual ~HRTF();

            //volume adjustment
            void volume(float volume);
            
            //gets the bolume
            float get_volume() const {return vol;}
            
            //gets the pointer ref to the HRTF.
            IPLHRTF get_ptr()const {return hrtf;}

            IPLAudioSettings* get_audiosettings();

            void normalization(IPLHRTFNormType type);
            //global_audio_context* context = nullptr;
            private:
                //pointers
                IPLHRTF hrtf = nullptr;
                //structs
                IPLHRTFSettings* hrtfsettings;
                IPLAudioSettings* audiosettings;
                //other
                float vol = 0;
    
    };
    
}