#include "StateManager.h"

StateManager::StateManager()
	: mActiveState(-1)
{
}

StateManager::~StateManager()
{
	for (auto& state : mStates)
	{
		state.second->OnDelete();
		delete state.second;
		state.second = nullptr;
	}
}

void StateManager::Update(float deltaTime)
{
	if (mStates.empty())
	{
		return;
	}

	mStates[mActiveState]->Update(deltaTime);
}

void StateManager::SwitchState(const int stateId)
{
}

void StateManager::SwitchState(const std::string& stateName)
{
}

int StateManager::GetIdFromName(const std::string& name)
{
	return 0;
}

void StateManager::QueueForRemoval(const int stateId)
{
}

void StateManager::ProcessRemovals()
{
}

void StateManager::CreateState(const int stateId)
{
}

void StateManager::RemoveState(const int stateId)
{
}
