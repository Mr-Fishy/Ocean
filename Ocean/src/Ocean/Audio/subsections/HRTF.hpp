#pragma once
#include <phonon.h>
#include "../steamaudio_dataholding.hpp"
namespace sonar{
    
    class HRTF{
        public:
            HRTF(std::string name, int samplingrate = 44100, int buffersize = 1024, float volume = 1.0f);
            virtual ~HRTF();

            virtual void volume();
            virtual void normalization(IPLHRTFNormType type);

        protected:
            
            //global_audio_context* context = nullptr;

        private:
            IPLAudioSettings audiosettings;
    };
    class basicHRTF: public HRTF{
        basicHRTF();
        ~basicHRTF();
        void volume() override;
        void normalization(IPLHRTFNormType type) override;

        private:
        
        IPLAudioSettings audiosettings;
        float vol = 0;
        

    };
    class customHRTF: public HRTF{
        customHRTF();
        ~customHRTF();
        void volume() override;
        void normalization(IPLHRTFNormType type) override;
        
        private:
        
        IPLAudioSettings audiosettings;
        float vol = 0;

    };


    /*CREATE TWO CHILDREN CLASSES
    ONE FOR THE BASE STUFF, ONE FOR THE CUSTOM STUFF
    USE INHERITANCE, MAKE SURE TO DELETE THIS BEFORE PUSHING FUTURE ME YOU DUMB APE
    THE MAIN CLASS SHOULD HAVE ALL OF THE VIRTUAL THINGS*/
}