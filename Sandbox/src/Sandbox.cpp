#include "Sandbox.hpp"

Ocean::Application* Ocean::CreateApplication() {
	Ocean::ApplicationConfig config("Ocean Sandbox", 1200, 800);

	return new Sandbox(config);
}



Sandbox::Sandbox(const Ocean::ApplicationConfig& config) : Application(config) {
	oprint(CONSOLE_TEXT_CYAN("Constructing Sandbox Application!\n"));

	// ---> Init Primitive Services
	Ocean::MemoryService::Instance().Init(nullptr);

	Ocean::oTimeServiceInit();

	p_ServiceManager = &Ocean::ServiceManager::Instance();
	p_ServiceManager->Init();

	// ---> Init Foundation

	// Window
	Ocean::WindowConfig winConfig {
		config.width,
		config.height,
		config.name,
	};
	// TODO: Figure out why below doesn't work.
	// p_Window = oallocat(Ocean::Window, 1, Ocean::MemoryService::Instance().SystemAllocator());
	p_Window = new Ocean::Window;
	p_Window->Init(&winConfig);

	// Input

	// Graphics

}

Sandbox::~Sandbox() {
	oprint(CONSOLE_TEXT_CYAN("Deconstructing Sandbox Application!\n"));

	// Input

	// Window
	p_Window->Shutdown();
	delete p_Window;

	// Primitives
	Ocean::ServiceManager::Shutdown();

	Ocean::oTimeServiceShutdown();

	Ocean::MemoryService::Shutdown();
}

// TODO: Fix Update Blocking When Window Is Grabbed (Moving)

b8 Sandbox::MainLoop() {
	Timestep currentTime(Ocean::oTimeRealiSec(Ocean::oTimeNow()));
	Timestep accumulator(0.0f);

	Timestep dt(0.02f);

	u32 accumulatorCounter = 0;
	Timestep time(0.0f);
	u32 frameCount = 0;

	while (!p_Window->RequestedExit()) {
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

		if (!p_Window->Minimized())
			// p_Renderer->BeginFrame();

		FrameBegin();

		p_Window->PollEvents();

		if (p_Window->Resized()) {
			OnResize(p_Window->Width(), p_Window->Height());

			p_Window->ResizeHandled();
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

		if (!p_Window->Minimized()) {
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



void Sandbox::FixedUpdate(Timestep delta) {
}

void Sandbox::VariableUpdate(Timestep delta) {
}



void Sandbox::Render(f32 interpolation) {
	// p_Renderer->RenderFrame();
}



void Sandbox::FrameBegin() {
}

void Sandbox::FrameEnd() {
}



void Sandbox::OnResize(u16 width, u16 height) {
	// p_Renderer->ResizeSwapchain(width, height);
}
