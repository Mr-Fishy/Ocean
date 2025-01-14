#include <Ocean/Ocean.hpp>

#include <Ocean/Core/EntryPoint.hpp>

class Coral : public Ocean::Application {
public:
	Coral(const Ocean::ApplicationConfig& config);
	~Coral();

};	// Sandbox



Ocean::Application* Ocean::CreateApplication(int argc, char** argv) {
	Ocean::ApplicationConfig config("Coral Editor", 1200, 800);

	return new Coral(config);
}



Coral::Coral(const Ocean::ApplicationConfig& config) : Ocean::Application(config) {
	oprint(CONSOLE_TEXT_CYAN("Constructing Coral Application!\n"));
}

Coral::~Coral() {
	oprint(CONSOLE_TEXT_CYAN("Deconstructing Coral Application!\n"));	
}
