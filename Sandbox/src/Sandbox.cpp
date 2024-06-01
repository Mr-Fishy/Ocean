
// Ocean
#include <Ocean/Ocean.hpp>
#include <Ocean/Core/EntryPoint.hpp>

// Layer Declarations

 /*
  * TODO Required Functionality:
  * 
  * 1) Alpha blending
  * 2) Multi texture support
  * 3) Render command API (ie draw(x, y, texture/color) from Sandbox)
  * 4) Hit-testing for UI interaction
  * 5) Layer setup with disabling
  * 
  * TODO Battleship:
  * 
  * 1) Main Menu
  *     a) Start Game
  *     b) Exit Game
  *     c) 2 Player vs Bot?
  * 2) Gameplay Loop
  *     a) Place Ships
  *     b) Primary Gameplay
  *         i) Select Valid Position to Fire
  *        ii) Hit or Not, if Ship has No Health, Destroy it
  *       iii) Oponent's Turn (Bot or Player)
  *     c) Once All Ships on Either Side are Destroyed, Declare Winner
  *     d) Return to Main Menu
  * 3) Bot Oponent?
 */

class Sandbox : public Ocean::Application {
public:
	Sandbox() = default;
	virtual ~Sandbox() = default;
};

Ocean::Application* Ocean::CreateApplication() {
	return new Sandbox();
}
