#pragma once

#include <Ocean/Ocean.hpp>

class GameState {
public:
	enum State {
		None = 0,
		MainMenu,
		Game,
		Exit
	};

	GameState() = default;
	~GameState() = default;

	State GetState() const { return m_State; }
	void SetState(const State state) { m_State = state; }

private:
	State m_State = State::MainMenu;
};
