#include "Ambisonics.hpp"
#include "Ocean/Core/Types/Integers.hpp"
#include <phonon.h>

Ocean::DOGsystem::Ambisonics::Ambisonics(IPLContext& context, IPLAudioSettings& audiosettings, u8 maxOrder){

    this->effectSettings.maxOrder = maxOrder;
    this->ref_context = context;
    this->ref_audio = audiosettings;
    iplAmbisonicsEncodeEffectCreate(this->ref_context, &(this->ref_audio), &(this->effectSettings), &(this->effect));
}