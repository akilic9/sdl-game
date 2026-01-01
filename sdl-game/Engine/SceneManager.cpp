#include "SceneManager.h"

#include "SDL3/SDL_log.h"

SceneManager::SceneManager()
	: mActiveState(-1)
{
}

SceneManager::~SceneManager()
{
	for (std::pair<const int, SceneBase*>& State : mStates)
	{
		State.second->OnDelete();
		delete State.second;
		State.second = nullptr;
	}
}

void SceneManager::Update(double DeltaTime)
{
	if (mStates.empty())
	{
		return;
	}

	mStates[mActiveState]->Update(DeltaTime);
}

void SceneManager::FixedUpdate()
{
	if (mStates.empty())
	{
		return;
	}

	mStates[mActiveState]->FixedUpdate();
}

void SceneManager::Render()
{
	if (mStates.empty())
	{
		return;
	}
	
	mStates[mActiveState]->Render();
}

void SceneManager::SwitchState(const int StateId)
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

void SceneManager::SwitchState(const std::string& StateName)
{
	const auto& StateItr = mNameToIdMap.find(StateName);
	if (StateItr != mNameToIdMap.end())
	{
		SwitchState(mNameToIdMap[StateName]);
	}
}

int SceneManager::GetIdFromName(const std::string& Name)
{
	const auto& StateItr = mNameToIdMap.find(Name);
	
	return StateItr != mNameToIdMap.end() ? mNameToIdMap[Name] : -1;
}

void SceneManager::QueueForRemoval(const int StateId)
{
	mRemovalQueue.push_back(StateId);
}

void SceneManager::ProcessRemovals()
{
	while (mRemovalQueue.begin() != mRemovalQueue.end())
	{
		RemoveState(*mRemovalQueue.begin());
		mRemovalQueue.erase(mRemovalQueue.begin());
	}
}

bool SceneManager::CreateState(const int StateId)
{
	const auto& StateInfo = mFactory.find(StateId);
	
	if (StateInfo == mFactory.end())
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not create new state with ID %d: No factory info found.", StateId);
		return false;
	}
	
	SceneBase* NewState = StateInfo->second();
	mStates[StateId] = NewState;
	NewState->OnCreate();
	return true;
}

void SceneManager::RemoveState(const int StateId)
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
