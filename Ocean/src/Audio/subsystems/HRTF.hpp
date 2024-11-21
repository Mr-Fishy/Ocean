#pragma once
#include <phonon.h>
namespace Ocean{
    namespace DOGsystem{
        class HRTF{
            public:
            IPLHRTF hrtf = nullptr;
            //Array data is stored contiguously.
            //Load_Custom(bool custom_SOFA = false, string path_to_SOFA ="");
            //Create ONE PER SOUND SOURCE
            HRTF(int samplingRate, int frameSize, IPLContext& context, IPLHRTFSettings& hrtfsettings, IPLAudioSettings& audiosettings);
            private:
            IPLContext ref_context = nullptr;
            
            
            ~HRTF();

            
        };
    }
}