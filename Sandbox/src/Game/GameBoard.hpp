#pragma once

#include <Ocean/Ocean.hpp>

class GameBoard {
public:
	enum ShipType {
		NONE      = 0,
		SCOUT     = 2,
		DESTROYER = 3,
		CARRIER   = 5,
	};

	enum Direction {
		UP = 0,
		DOWN,
		LEFT,
		RIGHT
	};

	struct Ship {
		ShipType Type;

		uvec2 Origin;
		Direction ShipDirection;

		Ship(const ShipType type, const uvec2& origin, const Direction direction) : Type(type), Origin(origin), ShipDirection(direction) { }
	};

	GameBoard();
	~GameBoard();

	void Draw();

	void OnEvent(Ocean::Event& e);

private:
	std::vector<Ship> m_Ships;

	/* Game Piece Textures */

};
