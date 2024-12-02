#include "Ambisonics.hpp"
#include "Ocean/Core/Types/Integers.hpp"
#include <phonon.h>

Ocean::DOGsystem::Ambisonics::Ambisonics(IPLContext& context, IPLAudioSettings& audiosettings,IPLHRTF& hrtf ,u8 maxOrder){
    this->ref_context = context;
    this->ref_audio = audiosettings;
    this->maxOrder =maxOrder;
    this->ref_hrtf = hrtf;
}

void Ocean::DOGsystem::Ambisonics::Effect_Encode(IPLAudioBuffer &inbuffer, IPLAudioBuffer &outbuffer,IPLVector3 space_xyz){
    this->encode_Settings.maxOrder=this->maxOrder;
    iplAmbisonicsEncodeEffectCreate(this->ref_context,&(this->ref_audio), &(this->encode_Settings), &(this->encode_effect));
    
    this->encode_params.direction = space_xyz;
    this->encode_params.order = this->maxOrder;
    
    iplAmbisonicsEncodeEffectApply(this->encode_effect, &encode_params, &inbuffer, &outbuffer);
}
//TODO enable decoding without using the binaural param  and set the speaker layout type.
void Ocean::DOGsystem::Ambisonics::Effect_Decode(IPLAudioBuffer &inbuffer, IPLAudioBuffer & outbuffer, IPLCoordinateSpace3 listener_xyz){
    decode_Settings.maxOrder = this->maxOrder;
    decode_Settings.hrtf = this->ref_hrtf;   

    iplAmbisonicsDecodeEffectCreate((this->ref_context),&(this->ref_audio),&(this->decode_Settings),&(this->decode_effect));
    
    
    this->decode_params.order=this->maxOrder;
    this->decode_params.hrtf = this->ref_hrtf;
    this->decode_params.orientation = listener_xyz;
    this->decode_params.binaural = IPL_TRUE;

    iplAmbisonicsDecodeEffectApply(this->decode_effect,&decode_params, &inbuffer, &outbuffer);   
}
//enables setting the orientation of the field.
void Ocean::DOGsystem::Ambisonics::orientation(IPLVector3 right_vec = IPLVector3{1.0,0.0,0.0},IPLVector3 up_vec = IPLVector3{0.0,1.0,0.0},IPLVector3 ahead_vec = IPLVector3{0.0,0.0,-1.0}){
    this->decode_params.orientation.right = right_vec;
    this->decode_params.orientation.up = up_vec;
    this->decode_params.orientation.ahead = ahead_vec;
}

void Ocean::DOGsystem::Ambisonics::orientation(IPLVector3 origin_vec = IPLVector3{0,0,0},IPLVector3 right_vec = IPLVector3{1.0,0.0,0.0},IPLVector3 up_vec = IPLVector3{0.0,1.0,0.0},IPLVector3 ahead_vec = IPLVector3{0.0,0.0,-1.0}){
    this->decode_params.orientation.origin=origin_vec;
    this->decode_params.orientation.right = right_vec;
    this->decode_params.orientation.up = up_vec;
    this->decode_params.orientation.ahead = ahead_vec;
}