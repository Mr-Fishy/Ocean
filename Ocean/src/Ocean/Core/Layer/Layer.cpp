#include "Layer.hpp"

namespace Ocean {

#ifdef OC_DEBUG

    Layer::Layer(const string& name) : m_Enabled(true), m_DebugName(name) { }

#else

    Layer::Layer(OC_UNUSED const string& name) : m_Enabled(true) { }

#endif

}   // Ocean
