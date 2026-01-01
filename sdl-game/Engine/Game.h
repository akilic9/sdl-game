#pragma once
#include "Window.h"
#include "InputManager.h"
#include "SceneManager.h"

class Game
{
public:
	Game();
	~Game();

	SDL_AppResult Init();

	SDL_AppResult HandleInput(SDL_Event* Event);
	void Update(double DeltaTime);
	void FixedUpdate();
	void Render();


private:
	Window mWindow;
	InputManager mInputManager;
	SceneManager mSceneManager;
};

