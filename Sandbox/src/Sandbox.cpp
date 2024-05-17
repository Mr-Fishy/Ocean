
// Ocean
#include <Ocean/Ocean.hpp>
#include <Ocean/Core/EntryPoint.hpp>

// Layer Declarations

class Sandbox : public Ocean::Application {
public:
	Sandbox() = default;
	virtual ~Sandbox() = default;
};

Ocean::Application* Ocean::CreateApplication() {
	return new Sandbox();
}
