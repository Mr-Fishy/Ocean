#pragma once

#include <iostream>
#include <phonon.h>
namespace Ocean{
    namespace DOGsystem{
        class audio{
            public:
                audio();
                virtual ~audio();
            private:
            //TODO: THIS IS JUST TO VISUALIZE WHAT I NEED TO BREAK UP.
                //Context Settings
                IPLContextSettings contextSettings{};
                IPLAudioSettings audioSettings{};
                IPLHRTFSettings hrtfSettings{};
                //Binaural effect settings
                IPLBinauralEffectSettings Binaural_effectSettings{};
                IPLBinauralEffectParams Binaural_params{};
                //Abisonic effect
                IPLAmbisonicsEncodeEffectSettings Ambisonic_effectSettings{};
                IPLAmbisonicsEncodeEffectParams Ambisonic_params{};
                IPLAmbisonicsDecodeEffectSettings Ambisonicdec_effectSettings{};
                //Direct effect
                IPLDirectEffectSettings Direct_effectSettings{};
                IPLDirectEffectParams Direct_params{};
                //Attenuation
                IPLDistanceAttenuationModel distanceAttenuationModel{};
                //Air absorbtion
                IPLAirAbsorptionModel airAbsorptionModel{};
                //Directivity
                IPLDirectivity directivity{};
                //Scene
                    //Scene runtime
                    IPLSceneSettings sceneSettings{};
                    IPLStaticMeshSettings staticMeshSettings{};
                    IPLInstancedMeshSettings instancedMeshSettings{};
                    //Scene not runtime
                    IPLSerializedObjectSettings soSettings{};
                //Simulation
                IPLSimulationSettings simulationSettings{};
                IPLSimulationInputs inputs{};
                IPLSimulationSharedInputs sharedInputs{};
                IPLSimulationOutputs outputs{};
                    //Sources
                    IPLSourceSettings sourceSettings{};
                    //Reflections
                    IPLReflectionEffectSettings effectSettings{};
                    IPLSimulationSettings Reflect_simulationSettings{};
                    IPLSimulationInputs Reflect_inputs{};
                    IPLSimulationSharedInputs Reflect_sharedInputs{};
                    IPLSimulationOutputs Reflect_outputs{};
                //Baking
                IPLProbeGenerationParams probeParams{};
                    //Baked reflections
                    IPLBakedDataIdentifier Baked_identifier{};
                    IPLReflectionsBakeParams Baked_bakeParams{};
                    //Load into sim
                    IPLSimulationInputs Baked_inputs{};
                //Pathing
                    //Baking Pathing
                    IPLBakedDataIdentifier Pathing_identifier{};
                    IPLPathBakeParams Pathing_bakeParams{};
                    //more sim
                    IPLSimulationSettings Pathing_simulationSettings{};
                    IPLSourceSettings Pathing_sourceSettings{};
                    IPLSimulationInputs Pathing_inputs{};
                    IPLSimulationOutputs Pathing_outputs{};

                IPLPathEffectSettings Path_effectSettings{};
                

                virtual void context_Creation(IPLContextSettings& contextSettings, IPLContext& context){};
                virtual void HRTF_Creation(IPLHRTFSettings& context){};
                virtual void buffer_Creation(int audio_Channels =1, int audio_Samples =512){};
                virtual void Binaural_Effect(IPLContext& context,IPLAudioSettings& audioSettings,IPLBinauralEffectSettings& effectSetting, IPLBinauralEffect& hrtf){};
        };
        class UPDOG{
            public:
            
            UPDOG(IPLContext& context, void* alloc = nullptr, void* free = nullptr);
            ~UPDOG();

            private:
            IPLContextSettings contextSettings{};
            IPLContext context =nullptr;
            IPLHRTF hrtf = nullptr;
            //For memory;
            void* callback = nullptr;
            void* free = nullptr;
        };
    }
}