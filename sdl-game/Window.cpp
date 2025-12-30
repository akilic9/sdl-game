#include "Window.h"
#include <SDL3/SDL_log.h>

const int DEFAULT_WIDTH = 1600;
const int DEFAULT_HEIGHT = 900;
const char* DEFAULT_TITLE = "SDL Window";

Window::Window() :
	mWindow(nullptr),
	mRenderer(nullptr),
	mWidth(DEFAULT_WIDTH),
	mHeight(DEFAULT_HEIGHT),
	mTitle(DEFAULT_TITLE) {}

Window::Window(const char* Title) :
	mWindow(nullptr),
	mRenderer(nullptr),
	mWidth(DEFAULT_WIDTH),
	mHeight(DEFAULT_HEIGHT),
	mTitle(Title) {}

Window::Window(const char* Title, const int Width, const int Height) :
	mWindow(nullptr),
	mRenderer(nullptr),
	mWidth(Width),
	mHeight(Height),
	mTitle(Title) {}

Window::~Window() {}

SDL_AppResult Window::Init()
{
	return CreateWindowAndRenderer(mTitle, mWidth, mHeight);
}

void Window::BeginDraw() const
{
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mRenderer);
}

void Window::EndDraw() const
{
	SDL_RenderPresent(mRenderer);
}

void Window::Update(double DeltaTime)
{
}

SDL_AppResult Window::CreateWindowAndRenderer(const char* Title, const int Width, const int Height)
{
	if(!SDL_CreateWindowAndRenderer(Title, Width, Height, SDL_WINDOW_RESIZABLE, &mWindow, &mRenderer) != 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Couldn't create window and renderer: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	return SDL_APP_CONTINUE;
}

SDL_AppResult Window::OnClose(InputBinding* Binding)
{
	return SDL_APP_SUCCESS;
}
