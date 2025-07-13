#include "Game.h"

Game::Game() : 
	mWindow("idk yet") {}

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
}

void Game::Render()
{
	mWindow.BeginDraw();

	mWindow.EndDraw();
}
