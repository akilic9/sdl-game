#pragma once
#include <string>

class StateBase
{
public:
    StateBase(const std::string& Name, const int& ID)
        : mName(Name)
        , mID(ID)
    {}

    virtual ~StateBase() {}

    virtual void OnCreate() = 0;
    virtual void OnEnter() = 0;
    virtual void Update(float DeltaTime) = 0;
    virtual void Render() = 0;
    virtual void OnExit() = 0;
    virtual void OnDelete() = 0;

    const std::string& GetName() const { return mName; }

protected:
    const std::string mName;
    int mID;
};

