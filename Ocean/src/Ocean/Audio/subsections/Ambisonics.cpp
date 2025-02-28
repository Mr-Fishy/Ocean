#pragma once
#include "Ambisonics.hpp"


sonar::Ambisonic::Ambisonic(Ref<sonar::HRTF> hrtf ,int order){
    
    this->effectsetting.maxOrder = order;
    iplAmbisonicsEncodeEffectCreate(*sonar::global_audio_context::audio->context, hrtf.get()->get_audiosettings(), &this->effectsetting, this->effect);
}