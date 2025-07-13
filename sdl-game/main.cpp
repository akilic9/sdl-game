#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "Game.h"

const float FIXED_FRAME_TIME = 1000.f / 60.f;
float elapsedTime = 0.f;
float accumulatedTime = 0.f;

Game game;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
	accumulatedTime = SDL_GetTicks();
	elapsedTime = SDL_GetTicks();
	return game.Init();
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
	return game.HandleEvents(event);
}

/* This function runs once per frame. */
SDL_AppResult SDL_AppIterate(void* appstate)
{
    float deltaTime = SDL_GetTicks() - elapsedTime;
    elapsedTime = SDL_GetTicks();

    game.Update(deltaTime);

    accumulatedTime += deltaTime;
    while (accumulatedTime >= FIXED_FRAME_TIME)
    {
        game.FixedUpdate();
        accumulatedTime -= FIXED_FRAME_TIME;
    }

    game.Render();

    return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
}