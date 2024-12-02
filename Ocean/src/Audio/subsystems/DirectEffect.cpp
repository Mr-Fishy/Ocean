#include "DirectEffect.hpp"
#include <phonon.h>
Ocean::DOGsystem::DirectEffect::DirectEffect(uint8_t channels,IPLContext& context ,IPLAudioSettings& audiosettings,IPLAudioBuffer& inBuffer, IPLAudioBuffer& outBuffer){
    this->effectSettings.numChannels = channels;
    this->ref_context = context;
    this->ref_audio =&audiosettings;
    this->in_ref = &inBuffer;
    this->out_ref= &outBuffer;
    iplDirectEffectCreate(this->ref_context, this->ref_audio, &(this->effectSettings), &(this->effect));
}

void Ocean::DOGsystem::DirectEffect::EffectApply(){
    iplDirectEffectApply(this->effect, &(this->effectparams), this->in_ref, this->out_ref);
}

//enables linear audio control
void Ocean::DOGsystem::DirectEffect::DistanceAttenuation_Calculate(float linear_volume){
    this->effectparams.flags = IPL_DIRECTEFFECTFLAGS_APPLYDISTANCEATTENUATION;
    this->effectparams.distanceAttenuation = linear_volume;
}

//enables modular control
void Ocean::DOGsystem::DirectEffect::DistanceAttenuation_Mode(IPLDistanceAttenuationModelType model_type){
    this->effectparams.flags = IPL_DIRECTEFFECTFLAGS_APPLYDISTANCEATTENUATION;
    this->AttenuationModel.type = model_type;
}
//used when calculating based off of model
void Ocean::DOGsystem::DirectEffect::DistanceAttenuation_Calculate(IPLVector3& source, IPLVector3& listener){
    float dist_calc = iplDistanceAttenuationCalculate(this->ref_context, source, listener, &(this->AttenuationModel));
    this->effectparams.distanceAttenuation = dist_calc;
}

void Ocean::DOGsystem::DirectEffect::Air_Absorbtion(){
    this->effectparams.flags =IPL_DIRECTEFFECTFLAGS_APPLYAIRABSORPTION;
    
}