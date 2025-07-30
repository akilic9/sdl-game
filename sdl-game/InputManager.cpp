#include "InputManager.h"

InputBinding::InputBinding(const char* Name, std::pair<InputType, int> Map)
    : mName(Name), mInputMap(Map) {}

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

void InputManager::Update()
{
}

bool InputManager::AddBinding(const char* Name, std::pair<InputType, int> Map)
{
    return false;
}

bool InputManager::RemoveBinding(const char* BindingName)
{
    return false;
}

void InputManager::RemoveCallback(const int SceneID, const char* CallbackName)
{
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
}
