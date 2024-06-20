#pragma once

#include <Ocean/Ocean.hpp>

class MainMenu {
public:
	MainMenu();
	~MainMenu();

	void Draw();

	b8 IsPlay(vec2& pos);
	b8 IsExit(vec2& pos);

	void OnEvent(Ocean::Event& e);

private:
	Ocean::Ref<Ocean::Texture2D> m_PlayButtonTexture;
	Ocean::Ref<Ocean::Texture2D> m_ExitButtonTexture;
};
