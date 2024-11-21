#pragma once
#include "Ocean/Core/Types/Integers.hpp"
#include "phonon.h"

namespace Ocean{
    namespace DOGsystem{
        class Ambisonics{

            public:

            Ambisonics(IPLContext& context,IPLAudioSettings& audiosettings, u8 maxOrder);
            ~Ambisonics();
            private:
            IPLContext ref_context =nullptr;
            IPLAudioSettings ref_audio;
            IPLAmbisonicsEncodeEffectSettings effectSettings{};
            IPLAmbisonicsEncodeEffect effect = nullptr;
        };
    }
}