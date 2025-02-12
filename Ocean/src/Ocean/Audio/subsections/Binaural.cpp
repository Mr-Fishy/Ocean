#include "Binaural.hpp"
#include "HRTF.hpp"
#include <phonon.h>

//creates the Binaural effect.
sonar::Binaural::Binaural(Ref<sonar::HRTF> hrtf){
    sonar::HRTF* temp = hrtf.get();
    IPLBinauralEffectSettings effectsettings{};
    effectsettings.hrtf = temp->get_ptr();
    IPLerror error = iplBinauralEffectCreate(*sonar::global_audio_context::audio->context, temp->get_audiosettings(), &effectsettings, &this->effect);
    sonar::audioerror(error);

}

void sonar::Binaural::start(IPLBinauralEffectParams* params){
    sonar::audioerror(iplBinauralEffectApply(this->effect,params,))
}