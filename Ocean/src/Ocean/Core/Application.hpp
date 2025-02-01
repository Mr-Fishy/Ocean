#pragma once

#include "Ocean/Primitives/Memory.hpp"
#include "Ocean/Types/Bool.hpp"
#include "Ocean/Types/Integers.hpp"
#include "Ocean/Types/Strings.hpp"
#include "Ocean/Types/Timestep.hpp"
#include "Ocean/Types/SmartPtrs.hpp"

#include "Ocean/Primitives/Macros.hpp"

#include "Ocean/Core/Layer/LayerStack.hpp"

#include "Ocean/Platform/Window.hpp"

extern int main(int argc, char** argv);

/**
 * @brief The namespace of Ocean Engine. All primary Ocean functionality is within this namespace.
 */
namespace Ocean {

	/**
	 * @brief A struct to configure the application at startup.
	 */
	struct ApplicationConfig {

		cstring name = nullptr; /** @brief The name of the application. */

		u32 width  = 900; /** @brief The starting width of the application window. */
		u32 height = 600; /** @brief The starting height of the application window. */

		b8 fullscreen = false; /** @brief If the application is fullscreen or not at startup. */

		/**
		 * @brief Construct a new ApplicationConfig with the given parameters.
		 * 
		 * @param name The name of the Application.
		 * @param w The initial width of the Application.
		 * @param h The initial height of the Application.
		 * @param fullscreen Set's the Application fullscreen at startup. (OPTIONAL)
		 */
		ApplicationConfig(cstring name, u32 w, u32 h, b8 fullscreen = false) : name(name), width(w), height(h), fullscreen(fullscreen) { }

	};	// ApplicationConfig

	/**
	 * @brief The base Ocean application class.
	 */
	class Application {
	public:
		/**
		 * @brief Construct a new Application object.
		 * 
		 * @param config The ApplicationConfig to use when starting the application.
		 */
		Application(const ApplicationConfig& config);
		virtual ~Application();

		/**
		 * @brief Closes the application.
		 */
		void Close();

		// void OnEvent(Event& e);

		/**
		 * @brief Pushes a layer onto the application's LayerStack.
		 * 
		 * @param layer The Layer to push.
		 */
		void PushLayer(Layer* layer);
		/**
		 * @brief Pushes a overlay onto the application's LayerStack. AKA it will be drawn over any layers.
		 * 
		 * @param layer The Layer to push.
		 */
		void PushOverlay(Layer* layer);

		/**
		 * @brief Call to resize the Application. AKA the main window and renderer.
		 * 
		 * @param width The new width.
		 * @param height The new height.
		 */
		void OnResize(u16 width, u16 height);

		/**
		 * @brief Get's the Application instance.
		 * 
		 * @return Application* 
		 */
		OC_INLINE OC_STATIC Application* Get() { return s_Instance; }

		OC_FINLINE void* operator new(sizet size) {
			return oalloca(size, oSystemAllocator);
		}

		OC_FINLINE void operator delete(void* ptr) {
			ofree(ptr, oSystemAllocator);
		}

	protected:
		friend int ::main(int argc, char** argv);

		/* --- */

		/**
		 * @brief The primary runtime of the application.
		 */
		void Run();

		/**
		 * @brief Run's any runtime tests if available.
		 */
		void TestRuntime();

		/**
		 * @brief Update function for fixed timestep processes. AKA physics or state-machines.
		 * 
		 * @param delta The time since the last update, expected to be consistent by ~0.001 second.
		 */
		void FixedUpdate(Timestep delta);
		/**
		 * @brief Update function for variable timestep processes. AKA layer and runtime updates.
		 * 
		 * @param delta The time since the last update.
		 */
		void VariableUpdate(Timestep delta);
		
		/**
		 * @brief Runs at the beginning of each frame.
		 */
		void FrameBegin();
		/**
		 * @brief Runs after VariableUpdate is completed (at least while single threaded).
		 * 
		 * @param interpolation The interpolation since the last frame.
		 */
		void Render(f32 interpolation);
		/**
		 * @brief Runs at the end of each frame.
		 */
		void FrameEnd();

		/* --- */

		OC_STATIC_INLINE Application* s_Instance = nullptr; /** @brief The instance of the Application, makes sure there is only one instance running. */
		Scope<Window> m_Window; /** @brief The main window of the application. */

		LayerStack m_LayerStack; /** @brief The LayerStack of the application. */

		Timestep m_LastFrameTime; /** @brief A Timestep of the last frame's runtime. */
		Timestep m_Accumulator; /** @brief A Timestep accumulating time until above the fixed timestep threshold. */

		b8 m_Running; /** @brief A b8 to record if the application is in runtime or not. */

	private:
		OC_NO_COPY(Application);

	};	// Application

	/**
	 * @brief Create an Application object defined by the user.
	 * 
	 * @return Application* 
	 */
	Application* CreateApplication(int argc, char** argv);

}	// Ocean
