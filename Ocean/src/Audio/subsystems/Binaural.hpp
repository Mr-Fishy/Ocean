#pragma once
#include <phonon.h>
namespace Ocean{
    namespace DOGsystem{
        //Create one per EACH SOUND SOURCE in SPACE.
        class Binaural{
            public:
            
            IPLBinauralEffect effect = nullptr;

            Binaural(IPLContext& context, IPLAudioSettings& audiosettings, IPLHRTF& hrtf);
            void Spatilize(IPLAudioBuffer in, IPLAudioBuffer out);
            ~Binaural();

            private:
            //references to important data.
            IPLHRTF ref_hrtf = nullptr;
            IPLContext ref_context = nullptr;

            //Parameters.
            IPLBinauralEffectParams params{};
            //I TURN THE PAGE
            IPLBinauralEffectSettings effectSettings{};

        };
    }
}