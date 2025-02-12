#pragma once
#include <phonon.h>
#include "../steamaudio_dataholding.hpp"

namespace sonar{

    //A struct for configuration, might change later
    struct HRTFconfig{
        HRTFconfig();
        void audioconfig(IPLint32 samplingrateHZ =44100, IPLint32 frameSize = 1024);

        //types
        float volume =1.0f;
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
            //gets the name
            const char* get_name() const {return name;}

            void normalization(IPLHRTFNormType type);
            //global_audio_context* context = nullptr;
            private:
                //pointers
                const char* name = nullptr;
                IPLHRTF hrtf = nullptr;
                //structs
                IPLHRTFSettings hrtfsettings;
                IPLAudioSettings audiosettings;
                //other
                float vol = 0;
    
    };
    
}