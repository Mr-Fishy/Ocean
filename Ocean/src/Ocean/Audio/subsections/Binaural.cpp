#include "Binaural.hpp"
#include "HRTF.hpp"
#include "Ocean/Primitives/Exceptions.hpp"
#include "Ocean/Types/SmartPtrs.hpp"
#include <phonon.h>

//creates the Binaural effect.
sonar::Binaural::Binaural(Ref<sonar::HRTF> hrtf,Ref<IPLAudioBuffer> inbuffer){
    this->buffer = inbuffer.get();
    if(this->buffer->numChannels >2 || this->buffer->numChannels<0){
        //no error handling here, dont mess up.
        throw Ocean::Exception(Ocean::INVALID_ARGUMENT,"Must have 1 to 2 channels.");
    }
    sonar::HRTF* temp = hrtf.get();
    
    this->effectsettings.hrtf = temp->get_ptr();
    IPLerror error = iplBinauralEffectCreate(*sonar::global_audio_context::audio->context, temp->get_audiosettings(), &effectsettings, &this->effect);
    sonar::audioerror(error);

}

void sonar::Binaural::spatialize(IPLBinauralEffectParams* params, IPLAudioBuffer* outbuffer){
    if(outbuffer->numChannels != 2){
        throw Ocean::Exception(Ocean::INVALID_ARGUMENT,"Must have two channels!");
    }
    IPLAudioEffectState state = iplBinauralEffectApply(this->effect,params,this->buffer,outbuffer);
    if(state == IPLAudioEffectState::IPL_AUDIOEFFECTSTATE_TAILREMAINING){
        throw Ocean::Exception(Ocean::LENGTH_ERROR,"Given buffer, and output buffer are of different length!");
    }
}