#include "Game.h"
#include <string>
#include "../Game/GameState.h"

Game::Game() : mWindow("idk yet") {}

Game::~Game() {}

SDL_AppResult Game::Init()
{	
	const SDL_AppResult& Result = mWindow.Init();
	if (Result == SDL_APP_FAILURE)
	{
		return Result;
	}
	
	// TODO: Move this to a better place.
	mInputManager.AddCallback(-1, "window_close", &Window::OnClose, &mWindow);
	
	// TODO: Move these to a better place.
	mSceneManager.RegisterState<GameState>("Game");
	mSceneManager.SwitchState("Game");
	
	return Result;
}

SDL_AppResult Game::HandleInput(SDL_Event* Event)
{
	return mInputManager.HandleInput(Event);
}

void Game::Update(double DeltaTime)
{
	mWindow.Update(DeltaTime);
	mSceneManager.Update(DeltaTime);
}

void Game::FixedUpdate()
{
	mSceneManager.FixedUpdate();
}

void Game::Render()
{
	mWindow.BeginDraw();
	
	// Do stuff
	mSceneManager.Render();

	mWindow.EndDraw();
}