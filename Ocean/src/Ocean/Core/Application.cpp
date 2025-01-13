#include "Application.hpp"

#include "Ocean/Primitives/Assert.hpp"
#include "Ocean/Primitives/Log.hpp"
#include "Ocean/Primitives/Time.hpp"

#include "Ocean/Renderer/Renderer.hpp"

namespace Ocean {

	#define FixedTimestep 0.2f

	Application::Application(OC_UNUSED const ApplicationConfig& config) : m_Window(), m_LayerStack(), m_LastFrameTime(0.0f), m_Accumulator(0.0f), m_Running(false) {
		OASSERTM(!
		
		s_Instance, "Application already exists!");
		s_Instance = this;

		this->m_Window = Window::Create(config.width, config.height, config.name);
		this->m_Window->Init();

		Shrimp::Renderer::Init();
	}

	Application::~Application() {
		Shrimp::Renderer::Shutdown();
	}

	void Application::Close() {
		this->m_Running = false;
	}

	void Application::Run() {
	#ifdef OC_DEBUG

		TestRuntime();

	#endif

		this->m_Running = true;

		// Temporary
		Timestep time(0.0f);
		u32 accumulatorCounter = 0;
		u32 frameCount = 0;

		while (this->m_Running) {
			Timestep t(oTimeNow());
			Timestep timeStep(t - this->m_LastFrameTime);
			this->m_LastFrameTime = t;

			this->m_Accumulator += timeStep;
			time += timeStep;

			if (time.GetSeconds() >= 5.0f ) {
				time -= 5.0f;
				oprint("Frames per 5 seconds: %i (%f fps)\n", frameCount, frameCount / 5.0f);
				oprint("Fixed Updates per 5 seconds: %i (%f ups)\n", accumulatorCounter, accumulatorCounter / 5.0f);
				frameCount = accumulatorCounter = 0;
			}

			// if (!this->m_Window->Minimized()) {
			// 	// p_Renderer->BeginFrame();
			// }

			FrameBegin();

			while (this->m_Accumulator.GetSeconds() >= FixedTimestep) {
				// TODO: Interpolation (For Physics Engine and Renderer)
				FixedUpdate(FixedTimestep);

				accumulatorCounter++;

				this->m_Accumulator -= FixedTimestep;
			}

			VariableUpdate(timeStep);

			if (!this->m_Window->Minimized()) {
				// TODO: Interpolation
				Render(f32());
			}
			// p_Renderer->EndFrame();

			FrameEnd();

			this->m_Window->OnUpdate();

			frameCount++;

			if (this->m_Window->RequestedExit())
				Close();
		}
	}

	void Application::TestRuntime() {

	}

	void Application::FixedUpdate(OC_UNUSED Timestep delta) {

	}
	void Application::VariableUpdate(OC_UNUSED Timestep delta) {

	}
	
	void Application::FrameBegin() {
		
	}
	void Application::Render(OC_UNUSED f32 interpolation) {
		
	}
	void Application::FrameEnd() {
		
	}

	void Application::OnResize(u16 width, u16 height) {
		Shrimp::Renderer::OnWindowResize(width, height);
	}

}	// Ocean
