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
	mInputManager.AddCallback(-1, "window_close", &Window::OnClose, &mWindow);
	return mWindow.Init();
}

SDL_AppResult Game::HandleInput(SDL_Event* event)
{
	return mInputManager.HandleInput(event);
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
