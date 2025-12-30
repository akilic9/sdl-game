#include "StateManager.h"

#include "SDL3/SDL_log.h"

StateManager::StateManager()
	: mActiveState(-1)
{
}

StateManager::~StateManager()
{
	for (std::pair<const int, StateBase*>& State : mStates)
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
	if (mStates.empty())
	{
		return;
	}
	
	mStates[mActiveState]->Render();
}

void StateManager::SwitchState(const int StateId)
{
	if (mActiveState > -1)
	{
		mStates[mActiveState]->OnExit();
	}
	
	const auto& StateItr = mStates.find(StateId);
	if (StateItr == mStates.end() && !CreateState(StateId))
	{
		return;
	}
	
	mActiveState = StateId;
	mStates[mActiveState]->OnEnter();
}

void StateManager::SwitchState(const std::string& StateName)
{
	const auto& StateItr = mNameToIdMap.find(StateName);
	if (StateItr != mNameToIdMap.end())
	{
		SwitchState(mNameToIdMap[StateName]);
	}
}

int StateManager::GetIdFromName(const std::string& Name)
{
	const auto& StateItr = mNameToIdMap.find(Name);
	
	return StateItr != mNameToIdMap.end() ? mNameToIdMap[Name] : -1;
}

void StateManager::QueueForRemoval(const int StateId)
{
	mRemovalQueue.push_back(StateId);
}

void StateManager::ProcessRemovals()
{
	while (mRemovalQueue.begin() != mRemovalQueue.end())
	{
		RemoveState(*mRemovalQueue.begin());
		mRemovalQueue.erase(mRemovalQueue.begin());
	}
}

bool StateManager::CreateState(const int StateId)
{
	const auto& StateInfo = mFactory.find(StateId);
	
	if (StateInfo == mFactory.end())
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not create new state with ID %d: No factory info found.", StateId);
		return false;
	}
	
	StateBase* NewState = StateInfo->second();
	mStates[StateId] = NewState;
	NewState->OnCreate();
	return true;
}

void StateManager::RemoveState(const int StateId)
{
	const auto& State = mStates.find(StateId);
	if (State == mStates.end())
	{
		return;
	}	
	
	State->second->OnDelete();
	delete State->second;
	State->second = nullptr;
	mStates.erase(State->first);
}
