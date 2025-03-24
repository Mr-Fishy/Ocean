#pragma once
#include <cstddef>
#include <cstdint>
#include <phonon.h>
#include "HRTF.hpp"
#include "../steamaudio_dataholding.hpp"
#include "Ocean/Primitives/Exceptions.hpp"

namespace sonar{
    //useful for ambient sounds, or spatialized music.

    class Ambisonic{
        public:
        
        enum audiotype{
            binaural = -1,
            mono = IPL_SPEAKERLAYOUTTYPE_MONO,
            stereo = IPL_SPEAKERLAYOUTTYPE_STEREO,
            quadraphonic = IPL_SPEAKERLAYOUTTYPE_QUADRAPHONIC,
            FIVE_1 = IPL_SPEAKERLAYOUTTYPE_SURROUND_5_1,
            SEVEN_1 = IPL_SPEAKERLAYOUTTYPE_SURROUND_7_1,
            CUSTOM = IPL_SPEAKERLAYOUTTYPE_CUSTOM
        };

        //must be given a max order.
            Ambisonic(const char* hrtf_name, int order = 1);
            //Will place the outbuffer on the global context.
            void encode(IPLVector3 vec,
                Ref<IPLAudioBuffer> inbuffer,
                IPLAudioBuffer* outbuffer);
            
            //by default decodes to BINAURAL
            void decode(audiotype type, IPLCoordinateSpace3 *listener_orientation,Ref<IPLAudioBuffer> inbuffer, Ref<IPLAudioBuffer> outbuffer);
            //handles caluclation internally, just pass in up and right vec (This should be handled by somethign above.)
            void rotate(IPLVector3 right, IPLVector3 up);

            ~Ambisonic();
            //vector from listener to source (HALF LIFE REFERENCE>RAHHHHH)
            //by default it is in your skull/ears idk.
            IPLVector3 direction_from_listener = IPLVector3{0.0f,0.0f,0.0f};
            
        private:
  
            void decode(IPLCoordinateSpace3 &listener_orientation,Ref<IPLAudioBuffer> inbuffer,Ref<IPLAudioBuffer> outbuffer);


            void decode(IPLCoordinateSpace3 &listener_orientation,IPLSpeakerLayoutType layout,Ref<IPLAudioBuffer> inbuffer,Ref<IPLAudioBuffer> outbuffer);


            IPLAmbisonicsDecodeEffect dedeffect = nullptr;
            IPLAmbisonicsDecodeEffectSettings deeffectsetting{};
            IPLAmbisonicsDecodeEffectParams deeffectparams{};

            const char* hrtf_name = nullptr;
            IPLAmbisonicsEncodeEffect* enceffect = nullptr;
            IPLAmbisonicsEncodeEffectSettings effectsetting{};
            IPLAmbisonicsEncodeEffectParams effectparams{};

            //just do this inline.



    };
}