#pragma once
#include "HRTF.hpp"
#include <cstdio>
#include <phonon.h>
#include <string>

namespace sonar{
    HRTF::HRTF(std::string name, sonar::HRTFconfig& config){
        this->name = name.c_str();
        //copy them.
        *(this->audiosettings) = config.audiosetting;
        *(this->hrtfsettings) = config.hrtfsetting;
       //create the hrtf
        IPLerror error = iplHRTFCreate(*sonar::global_audio_context::audio->context, this->audiosettings,  this->hrtfsettings, &this->hrtf);
        //if it broke.
        if(sonar::audioerror(error)){
            std::cerr<<"WARNING HRTF CREATION FAILURE"<<std::endl;
        }

    }

    //sets the audionormalization type.
    void HRTF::normalization(IPLHRTFNormType type){
        this->hrtfsettings->normType = type;
    }

    //sets volume
    void HRTF::volume(float volume){
        //some clamping
        this->hrtfsettings->volume = volume;

    }

    IPLAudioSettings* HRTF::get_audiosettings(){
        return this->audiosettings;
    }


    //config things.
    HRTFconfig::HRTFconfig(){
        //Audio settings
        this->audiosetting.frameSize = 1024;
        this->audiosetting.samplingRate =44100;
        //volume settings
        this->hrtfsetting.volume = 1.0f;
        this->hrtfsetting.normType = IPL_HRTFNORMTYPE_NONE;
        this->hrtfsetting.type = IPL_HRTFTYPE_DEFAULT;
        //custom file things.
        this->hrtfsetting.sofaFileName = nullptr;
        this->hrtfsetting.sofaData = nullptr;
        this->hrtfsetting.sofaDataSize = 0;

    }
}
