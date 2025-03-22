#include "Directeffect.hpp"
#include "HRTF.hpp"


sonar::DirectEffect::DirectEffect(Ref<sonar::HRTF> hrtf,int channels){
    settings.numChannels = channels;
    
}