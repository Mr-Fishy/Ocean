#include <Ocean.hpp>

class Sandbox : public Ocean::Application {
public:
	Sandbox() {

	}

	~Sandbox() {

	}
};

Ocean::Application* Ocean::CreateApplication() {
	return new Sandbox();
}