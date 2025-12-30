#include "InputManager.h"
#include <SDL3/SDL_log.h>
#include <fstream>
#include <string>

InputBinding::InputBinding(const std::string& Name, const std::pair<InputType, int> Map)
    : mInputMap(Map), mName(Name) {}

InputManager::InputManager()
	: mHasFocus(true)
	, mCurrentSceneID(-2) // Scene IDs start from 0, -1 is for universal callbacks.
{
	LoadBindings();
}

InputManager::~InputManager()
{
    for (std::pair<const std::string, std::vector<InputBinding*>>& Binding : mInputBindings)
    {
        for (const InputBinding* Input : Binding.second)
        {
            delete Input;
            Input = nullptr;
        }
        Binding.second.clear();
    }
}

SDL_AppResult InputManager::HandleInput(const SDL_Event* Event)
{
    if (!mHasFocus)
    {
        return SDL_APP_CONTINUE; // Ignore input if the window is not focused.
	}

	SDL_AppResult Result = SDL_APP_CONTINUE;

    for (std::pair<const std::string, std::vector<InputBinding*>>& Binding : mInputBindings)
    {
        for (InputBinding* Input : Binding.second)
        {
            const InputType Type = Input->mInputMap.first;
			const int KeyScanCode = Input->mInputMap.second;

            // KeyScanCode 0 is SDL_SCANCODE_UNKNOWN, used for when there is no keycode needed. Like the window close button.
            if (Type == static_cast<InputType>(Event->type) && (KeyScanCode == 0 || KeyScanCode == Event->key.scancode))
            {
                auto SceneCallbacks = mCallbacks.find(mCurrentSceneID);
                if (SceneCallbacks != mCallbacks.end())
                {
                    const std::string& BindingName = Binding.first;
                    auto CallbackItr = SceneCallbacks->second.find(BindingName);
                    if (CallbackItr != SceneCallbacks->second.end())
                    {
                        Result = CallbackItr->second(Input);
                    }
                }

                // -1 is callbacks that are scene independent. Check if this binding is in there.
                auto UniversalCallbacksItr = mCallbacks.find(-1); 
                if (UniversalCallbacksItr != mCallbacks.end())
                {
                    auto CallbackItr = UniversalCallbacksItr->second.find(Binding.first);
                    if (CallbackItr != UniversalCallbacksItr->second.end())
                    {
                        Result = CallbackItr->second(Input);
                    }
                }
            }
        }
    }

    return Result;
}

bool InputManager::AddBinding(const std::string& Name, const std::pair<InputType, int>& Map)
{
    InputBinding* Binding = new InputBinding(Name, Map);
    mInputBindings[Name].push_back(Binding);
    return true;
}

bool InputManager::RemoveBinding(const char* BindingName)
{
    const auto BindingItr = mInputBindings.find(BindingName);
    if (BindingItr == mInputBindings.end())
    {
        return false;
    }
    else
    {
        // Delete the binding pointers in vector.
        for (InputBinding* Inputs : BindingItr->second)
        { 
            delete Inputs;
            Inputs = nullptr;
        }

        BindingItr->second.clear(); // Clear vector.
        mInputBindings.erase(BindingItr); // Erase map element.
        return true;
    }
}

void InputManager::RemoveCallback(const int SceneID, const std::string& CallbackName)
{
    // Check if callbacks for this scene exist.
    const auto Scene = mCallbacks.find(SceneID);
    if (Scene == mCallbacks.end())
    {
        return;
    }

    // Check if a keybind by this name exist in this scene's callbacks.
    const auto Callback = Scene->second.find(CallbackName);
    if (Callback == Scene->second.end())
    {
        return;
    }

    // Delete the binding callback you found.
    Scene->second.erase(CallbackName);
}

SDL_FPoint InputManager::GetMousePosition() const
{
	SDL_FPoint MousePos;
    SDL_GetMouseState(&MousePos.x, &MousePos.y);
    return MousePos;
}

void InputManager::SetHasFocus(const bool bIsFocused)
{
    if (bIsFocused == mHasFocus)
    {
        return;
    }

    mHasFocus = bIsFocused;
}

void InputManager::SetCurrentSceneId(const int ID)
{
    if (mCurrentSceneID == ID)
    {
        return;
    }

    mCurrentSceneID = ID;
}

void InputManager::LoadBindings()
{
    std::ifstream BindingsStream;
    std::string FilePath = "Data/InputBindings.cfg";
    BindingsStream.open(FilePath);

    // Can't open file.
    if (!BindingsStream.is_open())
    {
        SDL_LogError(SDL_LOG_CATEGORY_INPUT, "Cannot open InputBindings.cfg!");
        return;
    }

    std::string Line;
    while (std::getline(BindingsStream, Line))
    {
        std::string EqualSeparator = "=";
        int SeparatorIndex = Line.find(EqualSeparator);
        if (SeparatorIndex != std::string::npos)
        {
            std::string ActionName = Line.substr(0, SeparatorIndex);
            std::string ActionCodes = Line.substr(SeparatorIndex + EqualSeparator.length(), Line.length());

            std::string ActionSeparator = "/";
            int ActionSeparatorIndex = ActionCodes.find(ActionSeparator);

            while (ActionSeparatorIndex != std::string::npos)
            {
                std::string ActionEvent = ActionCodes.substr(0, ActionSeparatorIndex);
                std::string KeySeparator = ":";

                int EventSeparatorIndex = ActionEvent.find(KeySeparator);
                if (EventSeparatorIndex != std::string::npos)
                {
                    int EventType = stoi(ActionEvent.substr(0, EventSeparatorIndex));
                    int EventKey = stoi(ActionEvent.substr(EventSeparatorIndex + KeySeparator.length(), ActionEvent.length()));

                    AddBinding(ActionName, std::make_pair((InputType)EventType, EventKey));
                }

                ActionCodes = ActionCodes.substr(ActionSeparatorIndex + ActionSeparator.length(), ActionCodes.length());
                ActionSeparatorIndex = ActionCodes.find(ActionSeparator);
            }
        }
    }
    BindingsStream.close();
}
