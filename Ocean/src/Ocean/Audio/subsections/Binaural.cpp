#include "Binaural.hpp"
#include "HRTF.hpp"
#include <phonon.h>

sonar::Binaural::Binaural(Ref<sonar::HRTF> hrtf,IPLfloat32 spatialblend, IPLVector3 source){
    sonar::HRTF* temp = hrtf.get();
    IPLBinauralEffectSettings effectsettings{};
    effectsettings.hrtf = temp->get_ptr();
}