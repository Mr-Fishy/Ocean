
#include <Ocean.hpp>
/* --- Entry Point --- */
#include <Ocean/Core/EntryPoint.hpp>

/* --- Renderable Layers --- */
// Layers Must Be Declared Before Sandbox Appliction
#include "EditorLayer.hpp"


namespace Ocean {

	class Coral : public Application {
	public:
		Coral() : Application("Coral Editor") {
			PushLayer(new EditorLayer());
		}

		~Coral() {

		}
	};

	Application* CreateApplication() {
		return new Coral();
	}

}	// Ocean
