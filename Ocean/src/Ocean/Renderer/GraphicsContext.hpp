#pragma once

namespace Ocean {

	// The graphics API context for the window.
	// 
	class GraphicsContext {
	public:
		virtual ~GraphicsContext() = default;

		virtual void Init() = 0;
		virtual	void SwapBuffers() = 0;

		static Scope<GraphicsContext> Create(void* window);
	};

}	// Ocean
