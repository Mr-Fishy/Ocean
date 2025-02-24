#include "Binaural.hpp"
#include "HRTF.hpp"
#include "Ocean/Primitives/Exceptions.hpp"
#include "Ocean/Types/SmartPtrs.hpp"
#include <phonon.h>

//creates the Binaural effect.
<<<<<<< HEAD
sonar::Binaural::Binaural(std::string hrtfname){
    sonar::HRTF* temp = sonar::global_audio_context::hrtfs[hrtfname.c_str()].get();
    IPLBinauralEffectSettings effectsettings{};
    effectsettings.hrtf = temp->get_ptr();
=======
sonar::Binaural::Binaural(Ref<sonar::HRTF> hrtf,Ref<IPLAudioBuffer> inbuffer){
    this->buffer = inbuffer.get();
    if(this->buffer->numChannels >2 || this->buffer->numChannels<0){
        Ocean::Exception(Ocean::INVALID_ARGUMENT,"Must have 1 to 2 channels.");
    }
    sonar::HRTF* temp = hrtf.get();
    
    this->effectsettings.hrtf = temp->get_ptr();
>>>>>>> 525e3080987d2bc03c9c031e3cd0d4a56c1d0294
    IPLerror error = iplBinauralEffectCreate(*sonar::global_audio_context::audio->context, temp->get_audiosettings(), &effectsettings, &this->effect);
    sonar::audioerror(error);

}

<<<<<<< HEAD
void sonar::Binaural::start(IPLBinauralEffectParams* params){
    sonar::audioerror(iplBinauralEffectApply(this->effect,params,));
=======
void sonar::Binaural::spatialize(IPLBinauralEffectParams* params, IPLAudioBuffer* outbuffer){
    if(outbuffer->numChannels != 2){
        Ocean::Exception(Ocean::INVALID_ARGUMENT,"Must have two channels!");
    }
    IPLAudioEffectState state = iplBinauralEffectApply(this->effect,params,this->buffer,outbuffer);
    if(state == IPLAudioEffectState::IPL_AUDIOEFFECTSTATE_TAILREMAINING){
        Ocean::Exception(Ocean::LENGTH_ERROR,"Given buffer, and output buffer are of different length!");
    }
>>>>>>> 525e3080987d2bc03c9c031e3cd0d4a56c1d0294
}