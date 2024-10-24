#pragma once

#include "Ocean/Core/Base.hpp"

#include "Ocean/Core/Types/ValueTypes.hpp"
#include "Ocean/Core/Types/Strings.hpp"

int main(int argc, char** argv);

namespace Ocean {

	class ServiceManager;

	/**
	 * @brief The configuration of the application at startup.
	 */
	struct ApplicationConfig {

		u32 width  = 900; /** @brief The width of the application window. */
		u32 height = 600; /** @brief The height of the application window. */

		b8 fullscreen = false; /** @brief If the application is fullscreen. */

		b8 initBaseServices = true;

		cstring name = nullptr; /** @brief The name of the application. */

		ApplicationConfig& w(u32 value) { width = value; return *this; }
		ApplicationConfig& h(u32 value) { height = value; return *this; }
		ApplicationConfig& name(cstring value) { name = value; return *this; }

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
		friend int ::main(int argc, char** argv);

		/* --- */

		/**
		 * @brief Runs the primary runtime of the application.
		 */
		void Run();

		/**
		 * @brief Run's any unit tests if available.
		 */
		void TestApp();

		virtual b8 MainLoop();

		virtual void FixedUpdate(f32 delta);
		virtual void VariableUpdate(f32 delta);
		
		virtual void Render(f32 interpolation);

		virtual void FrameBegin();
		virtual void FrameEnd();

		/* --- */

		static Application* s_Instance;

		ServiceManager* p_ServiceManager = nullptr;

	private:
		Application(const Application&) { }
		Application operator = (const Application&) { }
	};

	Application* CreateApplication();

}	// Ocean
