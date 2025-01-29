#include <functional>
#include <phonon.h>
#include <unordered_map>

namespace sonar{
    struct global_audio_context{

        IPLContextSettings* context;
        std::unordered_map<std::hash<const char*>, typename Tp>
    };
}