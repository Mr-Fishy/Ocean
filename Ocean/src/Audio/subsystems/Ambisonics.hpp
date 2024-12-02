#pragma once
#include "Ocean/Core/Types/Integers.hpp"
#include "phonon.h"

namespace Ocean{
    namespace DOGsystem{
        class Ambisonics{

            public:
            //for audio emitted by a point source.
            Ambisonics(IPLContext& context,IPLAudioSettings& audiosettings,IPLHRTF& hrtf,u8 maxOrder);
            void Effect_Encode(IPLAudioBuffer& inbuffer, IPLAudioBuffer& outbuffer,IPLVector3 space_xyz);
            void Effect_Decode(IPLAudioBuffer& inbuffer, IPLAudioBuffer& outbuffer,IPLCoordinateSpace3 listener_xyz);
            void orientation(IPLVector3 right_vec,IPLVector3 up_vec,IPLVector3 ahead_vec);
            void orientation(IPLVector3 origin_vec,IPLVector3 right_vec,IPLVector3 up_vec,IPLVector3 ahead_vec);
            ~Ambisonics();
            private:
            u8 maxOrder = 0;
            //references if needed.
            IPLContext ref_context =nullptr;
            IPLAudioSettings ref_audio;
            IPLHRTF ref_hrtf = nullptr;

            //holds the effect settings
                //encode
            IPLAmbisonicsEncodeEffectSettings encode_Settings{};
            IPLAmbisonicsEncodeEffectParams encode_params{};
            IPLAmbisonicsEncodeEffect encode_effect = nullptr;
                //decode
            IPLAmbisonicsDecodeEffectSettings decode_Settings{};
            IPLAmbisonicsDecodeEffectParams decode_params{};
            IPLAmbisonicsDecodeEffect decode_effect = nullptr;

        
            
        };
    }
}