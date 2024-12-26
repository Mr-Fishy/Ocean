#include "Application.hpp"

#include "Ocean/Core/Primitives/Assert.hpp"
#include "Ocean/Core/Primitives/Log.hpp"
#include "Ocean/Core/Primitives/Macros.hpp"
#include "Ocean/Core/Primitives/ServiceManager.hpp"
#include "Ocean/Core/Primitives/Time.hpp"
#include "Ocean/Core/Primitives/Memory.hpp"

#include "Renderer/Renderer.hpp"

namespace Ocean {

	Application::Application(OC_UNUSED const ApplicationConfig& config) : p_ServiceManager(&ServiceManager::Instance()), m_Windows() {
		OASSERTM(s_Instance != nullptr, "Application already exists!");
		s_Instance = this;

		MemoryService::Instance().Init(nullptr);

		oTimeServiceInit();

		p_ServiceManager->Init();

		Shrimp::Renderer::Init();
	}

	void Application::Close() {
		Shrimp::Renderer::Shutdown();

		Ocean::ServiceManager::Shutdown();

		Ocean::oTimeServiceShutdown();

		Ocean::MemoryService::Shutdown();
	}

	void Application::Run() {
	#ifdef OC_DEBUG

		TestApp();

	#endif

		// TODO: Application error reporting.
		if (!MainLoop())
			oprint("Application Runtime Error: (ERROR)");
	}

	void Application::TestApp() {

	}

	b8 Application::MainLoop() {
		Timestep currentTime(Ocean::oTimeRealiSec(Ocean::oTimeNow()));
		Timestep accumulator(0.0f);

		Timestep dt(0.02f);

		u32 accumulatorCounter = 0;
		Timestep time(0.0f);
		u32 frameCount = 0;

		// TODO: Remove lifetime dependency on the main window as an application can run without a window.
		while (!this->m_Windows[0].RequestedExit()) {
			Timestep t(Ocean::oTimeRealiSec(Ocean::oTimeNow()));
			Timestep frameTime(t - currentTime);
			currentTime = t;

			accumulator += frameTime;
			time += frameTime;
			
			if (time.GetSeconds() >= 5.0f ) {
				time -= 5.0f;
				oprint("Frames per 5 seconds: %i (%f fps)\n", frameCount, frameCount / 5.0f);
				oprint("Fixed Updates per 5 seconds: %i (%f ups)\n", accumulatorCounter, accumulatorCounter / 5.0f);
				frameCount = accumulatorCounter = 0;
			}

			if (!this->m_Windows[0].Minimized()) {
				// p_Renderer->BeginFrame();
			}

			FrameBegin();

			this->m_Windows[0].PollEvents();

			if (this->m_Windows[0].Resized()) {
				OnResize(this->m_Windows[0].Width(), this->m_Windows[0].Height());

				this->m_Windows[0].ResizeHandled();
			}

			// Fixed Update
			while (accumulator.GetSeconds() >= dt.GetSeconds()) {
				// TODO: Interpolation (For Physics Engine and Renderer)
				FixedUpdate(dt);

				accumulatorCounter++;

				accumulator -= dt;
			}

			// Variable Update
			VariableUpdate(frameTime);

			if (!this->m_Windows[0].Minimized()) {
				// TODO: Interpolation
				Render(f32());

				// p_Renderer->EndFrame();
			}

			// Prepare for the next frame if needed.
			FrameEnd();

			frameCount++;
		}

		// p_Renderer->CleanUp();

		Close();

		return true;
	}

	void Application::Render(OC_UNUSED f32 interpolation) {

	}

	void Application::OnResize(u16 width, u16 height) {
		Shrimp::Renderer::OnWindowResize(width, height);
	}

}	// Ocean
