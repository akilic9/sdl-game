#include "Game.h"
#include <SDL3/SDL_log.h>
#include <string>
#include <SDL3/SDL_timer.h>

Game::Game() : mWindow("idk yet") {}

Game::~Game() {}

SDL_AppResult Game::Init()
{
	mInputManager.AddCallback(-1, "window_close", &Window::OnClose, &mWindow);
	return mWindow.Init();
}

SDL_AppResult Game::HandleInput(SDL_Event* Event)
{
	return mInputManager.HandleInput(Event);
}

void Game::Update(double DeltaTime)
{
	mWindow.Update(DeltaTime);
}

void Game::FixedUpdate()
{
}

void Game::Render()
{
	mWindow.BeginDraw();

	mWindow.EndDraw();
}