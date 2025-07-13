#include "Game.h"
#include <SDL3/SDL_log.h>
#include <string>

Game::Game() : 
	mWindow("idk yet"),
	mDeltaTime(0.f) {}

Game::~Game() {}

SDL_AppResult Game::Init()
{
	return mWindow.Init();
}

SDL_AppResult Game::HandleEvents(SDL_Event* event)
{
	if (event->type == SDL_EVENT_QUIT) {
		return SDL_APP_SUCCESS; //end the program
	}
	return SDL_APP_CONTINUE;
}

void Game::Update(float deltaTime)
{
	mWindow.Update(deltaTime);
	mDeltaTime = deltaTime;
}

void Game::FixedUpdate()
{
}

void Game::Render()
{
	mWindow.BeginDraw();

	std::string message = std::to_string(1000.f / mDeltaTime);
	int w = 0, h = 0;
	float x, y;
	const float scale = 4.0f;

	/* Center the message and scale it up */
	SDL_GetRenderOutputSize(mWindow.GetRenderer(), &w, &h);
	SDL_SetRenderScale(mWindow.GetRenderer(), scale, scale);
	x = ((w / scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * SDL_strlen(message.c_str())) / 2;
	y = ((h / scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE) / 2;

	SDL_SetRenderDrawColor(mWindow.GetRenderer(), 255, 255, 255, 255);
	SDL_RenderDebugText(mWindow.GetRenderer(), x, y, message.c_str());
	
	SDL_Log("FPS: %f", 1000.f / mDeltaTime);

	mWindow.EndDraw();
}
