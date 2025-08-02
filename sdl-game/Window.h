#pragma once
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_render.h>
#include "InputManager.h"

class Window
{
public:
	Window();
	Window(const char* title);
	Window(const char* title, int width, int height);
	~Window();

	SDL_AppResult Init();
	void BeginDraw();
	void EndDraw();

	void Update(double deltaTime);

	SDL_AppResult OnClose(InputBinding* binding);

	SDL_Renderer* GetRenderer() const { return mRenderer; }
	SDL_Window* GetWindow() const { return mWindow; }

private:
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;

	int mWidth;
	int mHeight;
	const char* mTitle;

	SDL_AppResult CreateWindowAndRenderer(const char* title, int width, int height);
};

