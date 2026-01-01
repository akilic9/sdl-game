#pragma once
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_render.h>
#include "InputManager.h"

class Window
{
public:
	Window();
	Window(const char* Title);
	Window(const char* Title, const int Width, const int Height);
	~Window();

	SDL_AppResult Init();
	void BeginDraw() const;
	void EndDraw() const;

	void Update(double DeltaTime);

	SDL_AppResult OnClose(InputBinding* Binding);

	SDL_Renderer* GetRenderer() const { return mRenderer; }
	SDL_Window* GetWindow() const { return mWindow; }

private:
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;

	int mWidth;
	int mHeight;
	const char* mTitle;

	SDL_AppResult CreateWindowAndRenderer(const char* Title, const int Width, const int Height);
};

