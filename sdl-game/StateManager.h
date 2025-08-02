#pragma once
#include <unordered_map>
#include <functional>
#include "StateBase.h"

using StateList = std::unordered_map<int, StateBase*>;

using NameToId = std::unordered_map<std::string, int>;

using StateFactory = std::unordered_map<int, std::function<StateBase* (void)>>;

class StateManager
{
public:
    StateManager();
    ~StateManager();

    void Update(float deltaTime);
    void Render();

    // Register a state to create the state object.
    template<class T>
    void RegisterState(const std::string& stateName)
    {
        int stateId = mStateCount++;
        mNameToIdMap[stateName] = stateId;
        mFactory[stateId] = [stateName, stateId, this]() -> StateBase*
            { 
                return new T(stateName, stateId, this);
            };
    }

    virtual void SwitchState(const int stateId);
    virtual void SwitchState(const std::string& stateName);

    int GetIdFromName(const std::string& name);

    void QueueForRemoval(const int stateId);
    void ProcessRemovals();


protected:
    StateList mStates;
    StateFactory mFactory;
    NameToId mNameToIdMap;
    int mActiveState;
    int mStateCount = 0;
    std::vector<int> mRemovalQueue;

    void CreateState(const int stateId);
    void RemoveState(const int stateId);
};

