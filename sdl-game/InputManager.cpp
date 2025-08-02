#include "InputManager.h"
#include <SDL3/SDL_log.h>
#include <fstream>
#include <string>

InputBinding::InputBinding(const std::string& name, std::pair<InputType, int> map)
    : mName(name), mInputMap(map) {}

InputManager::InputManager()
	: mHasFocus(true)
    , mCurrentSceneID(-2)
{
	LoadBindings();
}

InputManager::~InputManager()
{
    for (auto& binding : mInputBindings)
    {
        for (auto& input : binding.second)
        {
            delete input;
            input = nullptr;
        }
        binding.second.clear();
    }
}

SDL_AppResult InputManager::HandleInput(SDL_Event* event)
{
    if (!mHasFocus)
    {
        return SDL_APP_CONTINUE; // Ignore input if the window is not focused.
	}

	SDL_AppResult result = SDL_APP_CONTINUE;

    for (auto& binding : mInputBindings)
    {
        for (auto& input : binding.second)
        {
            auto inputType = input->mInputMap.first;
            if (inputType == (InputType)event->type)
            {
                auto sceneCallbacks = mCallbacks.find(mCurrentSceneID);
                if (sceneCallbacks != mCallbacks.end())
                {
                    auto bindingName = binding.first;
                    auto callback = sceneCallbacks->second.find(bindingName);
                    if (callback != sceneCallbacks->second.end())
                    {
                        result = callback->second(input);
                    }
                }

                // -1 is callbacks that are scene independent. Check if this binding is in there.
                auto universalCallbacks = mCallbacks.find(-1); 
                if (universalCallbacks != mCallbacks.end())
                {
                    auto callback = universalCallbacks->second.find(binding.first);
                    if (callback != universalCallbacks->second.end())
                    {
                        result = callback->second(input);
                    }
                }
            }
        }
    }

    return result;
}

void InputManager::Update()
{
}

bool InputManager::AddBinding(const std::string& name, std::pair<InputType, int> map)
{
    InputBinding* binding = new InputBinding(name, map);
    mInputBindings[name].push_back(binding);
    return true;
}

bool InputManager::RemoveBinding(const char* bindingName)
{
    auto binding = mInputBindings.find(bindingName);
    if (binding == mInputBindings.end())
    {
        return false;
    }
    else
    {
        // Delete the binding pointers in vector.
        for (auto& inputs : binding->second)
        { 
            delete inputs;
            inputs = nullptr;
        }

        binding->second.clear(); // Clear vector.
        mInputBindings.erase(binding); // Erase map element.
        return true;
    }
}

void InputManager::RemoveCallback(const int sceneID, const std::string& callbackName)
{
    // Check if callbacks for this scene exist.
    auto scene = mCallbacks.find(sceneID);
    if (scene == mCallbacks.end())
    {
        return;
    }

    // Check if a keybind by this name exist in this scene's callbacks.
    auto callback = scene->second.find(callbackName);
    if (callback == scene->second.end())
    {
        return;
    }

    // Delete the binding callback you found.
    scene->second.erase(callbackName);
}

SDL_FPoint InputManager::GetMousePosition() const
{
	SDL_FPoint mousePos;
    SDL_GetMouseState(&mousePos.x, &mousePos.y);
    return mousePos;
}

void InputManager::SetHasFocus(const bool bIsFocused)
{
    if (bIsFocused == mHasFocus)
    {
        return;
    }

    mHasFocus = bIsFocused;
}

void InputManager::SetCurrentSceneId(const int id)
{
    if (mCurrentSceneID == id)
    {
        return;
    }

    mCurrentSceneID = id;
}

void InputManager::LoadBindings()
{
    std::ifstream bindingsStream;
    std::string filePath = "Data/InputBindings.cfg";
    bindingsStream.open(filePath);

    // Can't open file.
    if (!bindingsStream.is_open())
    {
        SDL_LogError(SDL_LOG_CATEGORY_INPUT, "Cannot open InputBindings.cfg!");
        return;
    }

    std::string line;
    while (std::getline(bindingsStream, line))
    {
        std::string equalSeparator = "=";
        int separatorIndex = line.find(equalSeparator);
        if (separatorIndex != std::string::npos)
        {
            std::string actionName = line.substr(0, separatorIndex);
            std::string actionCodes = line.substr(separatorIndex + equalSeparator.length(), line.length());

            std::string actionSeparator = "/";
            int actionSeparatorIndex = actionCodes.find(actionSeparator);

            while (actionSeparatorIndex != std::string::npos)
            {
                std::string actionEvent = actionCodes.substr(0, actionSeparatorIndex);
                std::string keySeparator = ":";

                int eventSeparatorIndex = actionEvent.find(keySeparator);
                if (eventSeparatorIndex != std::string::npos)
                {
                    int eventType = stoi(actionEvent.substr(0, eventSeparatorIndex));
                    int eventKey = stoi(actionEvent.substr(eventSeparatorIndex + keySeparator.length(), actionEvent.length()));

                    AddBinding(actionName, std::make_pair((InputType)eventType, eventKey));
                }

                actionCodes = actionCodes.substr(actionSeparatorIndex + actionSeparator.length(), actionCodes.length());
                actionSeparatorIndex = actionCodes.find(actionSeparator);
            }
        }
    }
    bindingsStream.close();
}
