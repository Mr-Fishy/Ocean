#include "Binaural.hpp"
#include <phonon.h>

sonar::Binaural::Binaural(){
    Binaural_settings.hrtf= *hrtf;
    iplBinauralEffectCreate(*context_settings, audio_settings, &Binaural_settings, effect);
}

sonar::Binaural::~Binaural(){
    
}