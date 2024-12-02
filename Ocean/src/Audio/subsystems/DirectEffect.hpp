#pragma once
#include <phonon.h>
#include <stdint.h>



namespace Ocean{
    namespace DOGsystem{

        class DirectEffect{
        public:
        DirectEffect(uint8_t channels,IPLContext& context,IPLAudioSettings& audiosettings,IPLAudioBuffer& inBuffer, IPLAudioBuffer& outBuffer);
        void EffectApply();

        //this allows for selecgint the model
        void DistanceAttenuation_Mode(IPLDistanceAttenuationModelType model_type);
        //done for linear control
        void DistanceAttenuation_Calculate(float linear_volume);
        //does simulated calculation
        void DistanceAttenuation_Calculate(IPLVector3& sourcePosition, IPLVector3& listenerPosition);


        void Air_Absorbtion();

        void Directivity();

        void Occlusion();

        void Transmission();

        private:

        //required references.
        IPLContext ref_context = nullptr;
        IPLAudioSettings* ref_audio;
        //buffer reference
        IPLAudioBuffer* in_ref = nullptr;
        IPLAudioBuffer* out_ref= nullptr;
        //Direct effect
        IPLDirectEffectSettings effectSettings{};
        IPLDirectEffectParams effectparams{};
        IPLDirectEffect effect = nullptr;
        //attenuation
        IPLDistanceAttenuationModel AttenuationModel{};
        };
    }
}