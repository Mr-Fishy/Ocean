
// Ocean
#include <Ocean/Ocean.hpp>
#include <Ocean/Core/EntryPoint.hpp>

// Layer Declarations

class Coral : public Ocean::Application {
public:
	Coral() = default;
	virtual ~Coral() = default;
};

Ocean::Application* Ocean::CreateApplication() {
	return new Coral();
}
