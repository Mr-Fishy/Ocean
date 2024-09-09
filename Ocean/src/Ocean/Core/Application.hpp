#pragma once

#include "Ocean/Core/Base.hpp"

#include "Ocean/Core/Types/ValueTypes.hpp"
#include "Ocean/Core/Types/Strings.hpp"

int main(int argc, char** argv);

namespace Ocean {

	class ServiceManager;

	struct ApplicationConfig {

		u32 Width  = 900;
		u32 Height = 600;

		cstring Name = nullptr;

		b8 InitBaseServices = false;

		ApplicationConfig& w(u32 value) { Width = value; return *this; }
		ApplicationConfig& h(u32 value) { Height = value; return *this; }
		ApplicationConfig& name(cstring value) { Name = value; return *this; }

	};	// ApplicationConfig

	class Application {
	public:
		Application(const ApplicationConfig& config);
		virtual ~Application();

		void Close();

	protected:
		friend int ::main(int argc, char** argv);

		/* --- */

		void Run();

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
	};

	Application* CreateApplication();

}	// Ocean
