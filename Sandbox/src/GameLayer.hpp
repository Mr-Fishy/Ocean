#pragma once

#include <Ocean/Ocean.hpp>

#include "Camera/CameraController.hpp"

#include "Game/State.hpp"

#include "Game/MainMenu.hpp"
#include "Game/GameBoard.hpp"


class Battleship : public Ocean::Layer {
public:
	Battleship();
	virtual ~Battleship();

	virtual void OnAttach() override final;
	virtual void OnDetach() override final;

	virtual void OnUpdate(Ocean::Timestep ts) override final;
	virtual void OnEvent(Ocean::Event& e) override final;

private:
	AppCamera m_Camera;

	GameState m_State;

	Ocean::Ref<Ocean::Texture2D> m_CheckboardTexture;
	Ocean::Ref<Ocean::Texture2D> m_BackgroundTexture;

	MainMenu m_Menu;
	GameBoard m_GameBoard;

	const vec2 ZeroPoint = { 0.0f, 0.0f };
	const vec2 WindowSize = { 4.0f, 2.0f };
};
