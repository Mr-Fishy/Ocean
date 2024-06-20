#include "GameBoard.hpp"

#include <random>

static ui32 surandi(ui32 high, ui32 low) {
	return static_cast<ui32>(rand()) % (high - low);
}

GameBoard::GameBoard() {
	srand(time(NULL));

	ui16 usedRows = 0;
	ui16* usedCol = new ui16[10]{ 0 };

	m_Ships.emplace_back(new Ship( CARRIER, { surandi(10, 0), surandi(10, 0) }, /*static_cast<Direction>(surandi(3, 0))*/ UP ));
	


	delete[] usedCol;
}

GameBoard::~GameBoard() {
}

void GameBoard::Draw() {

	for (const Ship& ship : m_Ships) {
		switch (ship.ShipDirection) {
			case UP:
				Ocean::Renderer2D::DrawQuad({ -1.0f + (0.1f * ship.Origin.x),  -0.05f * ship.Origin.y }, { 1.75f, 0.875f }, { 0.3f, 0.3f, 0.3f, 1.0f });
				break;

			case DOWN:
				break;

			case LEFT:
				break;

			case RIGHT:
				break;
		}
	}

	/* Radar Board */
	for (ui8 i = 1; i < 21; i += 2)
		Ocean::Renderer2D::DrawQuad({ 0.0f, 0.05f * i }, { 4.0f / 2.1f, 0.01f }, { 0.1f, 0.1f, 0.1f, 1.0f });
	for (ui8 i = 1; i < 21; i += 2)
		Ocean::Renderer2D::DrawQuad({ 0.0f - 1.0f + (0.1f * i), 0.5f }, { 0.02f, 2.0f / 2.1f }, { 0.1f, 0.1f, 0.1f, 1.0f });

	/* Player Board */
	for (ui8 i = 1; i < 21; i += 2)
		Ocean::Renderer2D::DrawQuad({ 0.0f,  -0.05f * i }, { 4.0f / 2.1f, 0.01f }, { 0.1f, 0.1f, 0.1f, 1.0f });
	for (ui8 i = 1; i < 21; i += 2)
		Ocean::Renderer2D::DrawQuad({ -1.0f + (0.1f * i), -0.5f }, { 0.02f, 2.0f / 2.1f }, { 0.1f, 0.1f, 0.1f, 1.0f });
}

void GameBoard::OnEvent(Ocean::Event& e) {
}
