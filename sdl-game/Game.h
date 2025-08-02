#pragma once
#include "Window.h"
#include "InputManager.h"

class Game
{
public:
	Game();
	~Game();

	SDL_AppResult Init();

	SDL_AppResult HandleInput(SDL_Event* event);
	void Update(double deltaTime);
	void FixedUpdate();
	void Render();


private:
	Window mWindow;
	InputManager mInputManager;
	float mDeltaTime;
};

