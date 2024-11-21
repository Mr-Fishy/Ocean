#include "Binaural.hpp"
#include <phonon.h>

Ocean::DOGsystem::Binaural::Binaural(IPLContext& context, IPLAudioSettings& audiosettings, IPLHRTF& hrtf){
    IPLHRTF ref_hrtf = hrtf;
    IPLContext ref_context = context;
    this->effectSettings.hrtf= ref_hrtf;
    iplBinauralEffectCreate(context,&audiosettings,&(this->effectSettings),&(this->effect));
}
void Ocean::DOGsystem::Binaural::Spatilize(IPLAudioBuffer in, IPLAudioBuffer out){
    this->params.direction = IPLVector3{1,1,1};
    this->params.hrtf = this->ref_hrtf;
    this->params.interpolation = IPL_HRTFINTERPOLATION_NEAREST;
    this->params.spatialBlend =1.0;
    this->params.peakDelays = nullptr;
    iplBinauralEffectApply(this->effect,&(this->params),&in,&out);
}