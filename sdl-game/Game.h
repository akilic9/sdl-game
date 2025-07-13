#pragma once
#include "Window.h"

class Game
{
public:
	Game();
	~Game();

	SDL_AppResult Init();

	SDL_AppResult HandleEvents(SDL_Event* event);
	void Update(float deltaTime);
	void FixedUpdate();
	void Render();


private:
	Window mWindow;
	float mDeltaTime;
};

