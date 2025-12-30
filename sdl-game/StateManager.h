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

    void Update(float DeltaTime);
    void Render();

    // Register a state to create a state.
    template<class T>
    void RegisterState(const std::string& StateName)
    {
        const int StateId = mStateCount++;
        mNameToIdMap[StateName] = StateId;
        mFactory[StateId] = [StateName, StateId, this]() -> StateBase*
            { 
                return new T(StateName, StateId, this);
            };
    }

    virtual void SwitchState(const int StateId);
    virtual void SwitchState(const std::string& StateName);

    int GetIdFromName(const std::string& Name);

    void QueueForRemoval(const int StateId);
    void ProcessRemovals();


protected:
    StateList mStates;
    StateFactory mFactory;
    NameToId mNameToIdMap;
    int mActiveState;
    int mStateCount = 0;
    std::vector<int> mRemovalQueue;

    bool CreateState(const int StateId);
    void RemoveState(const int StateId);
};

