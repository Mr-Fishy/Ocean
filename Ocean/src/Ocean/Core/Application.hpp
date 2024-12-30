#pragma once

#include "Ocean/Core/Types/Bool.hpp"
#include "Ocean/Core/Types/Integers.hpp"
#include "Ocean/Core/Types/Strings.hpp"
#include "Ocean/Core/Types/Timestep.hpp"
#include "Ocean/Core/Types/UniquePtr.hpp"

#include "Ocean/Core/Primitives/Macros.hpp"

int main(/* int argc, char** argv */);

namespace Ocean {

	class ServiceManager;

	/**
	 * @brief The configuration of the application at startup.
	 */
	struct ApplicationConfig {

		cstring name = nullptr; /** @brief The name of the application. */

		u32 width  = 900; /** @brief The width of the application window. */
		u32 height = 600; /** @brief The height of the application window. */

		b8 fullscreen = false; /** @brief If the application is fullscreen. */

		// TODO: Service Enabling / Disabling (i.e. ability to disable audio if not needed).

		ApplicationConfig(cstring name, u32 w, u32 h, b8 fullscreen = false) : name(name), width(w), height(h), fullscreen(fullscreen) { }

	};	// ApplicationConfig

	/**
	 * @brief The base Ocean application class.
	 */
	class Application {
	public:
		Application(const ApplicationConfig& config);
		virtual ~Application();

		/**
		 * @brief Closes the application.
		 */
		void Close();

	protected:
		friend int ::main(/* int argc, char** argv */);

		/* --- */

		/**
		 * @brief Runs the primary runtime of the application.
		 */
		void Run();

		/**
		 * @brief Run's any unit tests if available.
		 */
		void TestApp();

		/**
		 * @brief The runtime loop of the application.
		 * 
		 * @return b8 True if successful runtime. False otherwise.
		 */
		b8 MainLoop();

		virtual void FixedUpdate(Timestep delta) = 0;
		virtual void VariableUpdate(Timestep delta) = 0;
		
		void Render(f32 interpolation);

		virtual void FrameBegin() = 0;
		virtual void FrameEnd() = 0;

		void OnResize(u16 width, u16 height);

		/* --- */

		inline static Application* s_Instance = nullptr;

		UniquePtr<ServiceManager> p_ServiceManager;

		b8 m_Running;

	private:
		OC_NO_COPY(Application);
	};

	Application* CreateApplication();

}	// Ocean
