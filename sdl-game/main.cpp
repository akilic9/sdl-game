#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "Game.h"

const double FIXED_FRAME_TIME = 1000.f / 60.f;
double ElapsedTime = 0.f;
double AccumulatedTime = 0.f;

Game Game;

// This function runs once at startup.
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
	return Game.Init();
}

// This function runs when a new event (mouse input, keypresses, etc) occurs.
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
	//SDL_Log("Event type: %d, event key: %d", event->type, event->key.scancode);
	return Game.HandleInput(event);
}

// This function runs once per frame.
SDL_AppResult SDL_AppIterate(void* appstate)
{
    const double DeltaTime = SDL_GetTicks() - ElapsedTime;
    ElapsedTime = SDL_GetTicks();

    Game.Update(DeltaTime);

    AccumulatedTime += DeltaTime;
    while (AccumulatedTime >= FIXED_FRAME_TIME)
    {
        Game.FixedUpdate();
        AccumulatedTime -= FIXED_FRAME_TIME;
    }

    Game.Render();

    return SDL_APP_CONTINUE;
}

// This function runs once at shutdown.
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
}