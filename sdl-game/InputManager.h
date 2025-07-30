#pragma once
#include <SDL3/SDL_events.h>
#include <unordered_map>
#include <functional>

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

// Input type -> key code (if any)
struct InputBinding {
    InputBinding(const char* Name, std::pair<InputType, int> Map);

    std::pair<InputType, int> mInputMap;
    const char* mName;
};

// Name -> binding
using InputBindings = std::unordered_map<const char*, std::vector<InputBinding*>>;

// Input name -> target function
using Callback = std::unordered_map<const char*, std::function<void(InputBinding*)>>;

// Scene id -> scene's callbacks
using SceneCallbacks = std::unordered_map<int, Callback>;

class InputManager
{
public:
    InputManager();
    ~InputManager();

    //void HandleInputs(sf::Event Event);
    void Update();

    bool AddBinding(const char* Name, std::pair<InputType, int> Map);
    bool RemoveBinding(const char* BindingName);

    // Add a callback - i.e. register to a keybind trigger event on a scene basis.
    template<class T>
    bool AddCallback(const int SceneID, const char* CallbackName, void(T::* Func)(InputBinding*), T* Instance)
    {
        // Create a [sceneId, default constructor callback] element
        auto Itr = mCallbacks.emplace(SceneID, Callback()).first;
        // Black magic https://en.cppreference.com/w/cpp/utility/functional/placeholders
        auto Temp = std::bind(Func, Instance, std::placeholders::_1);
        // Store the bound function and callback name into the previously default callback
        return Itr->second.emplace(CallbackName, Temp).second;
    }

    // Remove a callback ie unregister from a keybind on a scene basis.
    void RemoveCallback(const int SceneID, const char* CallbackName);

    
    // If not passed a window, this function will return the mouse position relative to desktop origin.
	// window must be passed in to get the position within the window.    
    //sf::Vector2i GetMousePos(sf::RenderWindow* window = nullptr) const { return (window ? sf::Mouse::getPosition(*window) : sf::Mouse::getPosition()); }

    // Set window focus flag.
    void SetHasFocus(const bool bIsFocused);
    void SetCurrentSceneId(const int ID);

private:
    bool mHasFocus;
    InputBindings mInputBindings;
    SceneCallbacks mCallbacks;
    int mCurrentSceneID;

    // Load keybinds from cfg file.
    void LoadBindings();
};