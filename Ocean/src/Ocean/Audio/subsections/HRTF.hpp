
#include <phonon.h>
namespace sonar{
    class HRTF{
        public:
        HRTF(int& sampling_rate_hz, int& frame_size, IPLHRTFType type =IPL_HRTFTYPE_DEFAULT, const char* const file_path = "");
        ~HRTF();
        private:
        IPLHRTFSettings hrtf_settings;
        IPLAudioSettings* audio_settings;

    };
}