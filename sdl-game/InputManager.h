#pragma once
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <unordered_map>
#include <functional>
#include <string>

enum class InputType {
	KeyDown = SDL_EVENT_KEY_DOWN,
	KeyUp = SDL_EVENT_KEY_UP,
	MButtonDown = SDL_EVENT_MOUSE_BUTTON_DOWN,
	MButtonUp = SDL_EVENT_MOUSE_BUTTON_UP,
	MouseWheel = SDL_EVENT_MOUSE_WHEEL,
	WindowResized = SDL_EVENT_WINDOW_RESIZED,
	GainedFocus = SDL_EVENT_WINDOW_FOCUS_GAINED,
	LostFocus = SDL_EVENT_WINDOW_FOCUS_LOST,
	MouseEntered = SDL_EVENT_WINDOW_MOUSE_ENTER,
	MouseLeft = SDL_EVENT_WINDOW_MOUSE_LEAVE,
	Closed = SDL_EVENT_QUIT
};

// Input type -> key scancode (if any)
struct InputBinding {
    InputBinding(const std::string& name, std::pair<InputType, int> map);

    std::pair<InputType, int> mInputMap;
    std::string mName;
};

// Name -> binding
using InputBindings = std::unordered_map<std::string, std::vector<InputBinding*>>;

// Input name -> target function
using Callback = std::unordered_map<std::string, std::function<SDL_AppResult(InputBinding*)>>;

// Scene id -> scene's callbacks
using SceneCallbacks = std::unordered_map<int, Callback>;

class InputManager
{
public:
    InputManager();
    ~InputManager();

    SDL_AppResult HandleInput(SDL_Event* event);

    bool AddBinding(const std::string& name, std::pair<InputType, int> map);
    bool RemoveBinding(const char* bindingName);

    // Add a callback - i.e. register to a keybind trigger event on a scene basis.
    template<class T>
    bool AddCallback(const int sceneID, const std::string& callbackName, SDL_AppResult(T::* func)(InputBinding*), T* instance)
    {
        // Create a [sceneId, default constructor callback] element
        auto Itr = mCallbacks.emplace(sceneID, Callback()).first;

        // Black magic https://en.cppreference.com/w/cpp/utility/functional/placeholders
        auto Temp = std::bind(func, instance, std::placeholders::_1);

        // Store the bound function and callback name into the previously default callback
        return Itr->second.emplace(callbackName, Temp).second;
    }

    // Remove a callback ie unregister from a keybind on a scene basis.
    void RemoveCallback(const int sceneID, const std::string& callbackName);

    
    // Return the mouse position relative to window's top left corner.
    SDL_FPoint GetMousePosition() const;


    // Set window focus flag.
    void SetHasFocus(const bool bIsFocused);
    void SetCurrentSceneId(const int id);

private:
    bool mHasFocus;
    InputBindings mInputBindings;
    SceneCallbacks mCallbacks;
    int mCurrentSceneID;

    // Load keybinds from cfg file.
    void LoadBindings();
};