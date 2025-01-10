#include "audio.hpp"


audio::steamaudio* audio::steamaudio::instance(){

    

    if(audio::steamaudio::struct_pointer == nullptr){
        struct_pointer = new steamaudio();
    }
    return struct_pointer;
}