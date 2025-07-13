#include "Game.h"
#include <SDL3/SDL_log.h>
#include <string>
#include <SDL3/SDL_timer.h>

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

void Game::Update(double deltaTime)
{
	mWindow.Update(deltaTime);
}

void Game::FixedUpdate()
{
}

void Game::Render()
{
	mWindow.BeginDraw();

	mWindow.EndDraw();
}
