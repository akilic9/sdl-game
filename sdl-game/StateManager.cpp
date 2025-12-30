#include "StateManager.h"

StateManager::StateManager()
	: mActiveState(-1)
{
}

StateManager::~StateManager()
{
	for (auto& State : mStates)
	{
		State.second->OnDelete();
		delete State.second;
		State.second = nullptr;
	}
}

void StateManager::Update(float DeltaTime)
{
	if (mStates.empty())
	{
		return;
	}

	mStates[mActiveState]->Update(DeltaTime);
}

void StateManager::Render()
{
}

void StateManager::SwitchState(const int StateId)
{
}

void StateManager::SwitchState(const std::string& StateName)
{
}

int StateManager::GetIdFromName(const std::string& Name)
{
	return 0;
}

void StateManager::QueueForRemoval(const int StateId)
{
}

void StateManager::ProcessRemovals()
{
}

void StateManager::CreateState(const int StateId)
{
}

void StateManager::RemoveState(const int StateId)
{
}
