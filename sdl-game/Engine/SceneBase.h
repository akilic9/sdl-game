#pragma once
#include <string>

class SceneManager;

class SceneBase
{
public:
    SceneBase(const std::string& Name, const int& ID, SceneManager* SceneManager)
        : mName(Name)
        , mID(ID)
        , mSceneManager(SceneManager)
    {}

    virtual ~SceneBase() {}

    virtual void OnCreate() = 0;
    virtual void OnEnter() = 0;
    virtual void Update(double DeltaTime) = 0;
    virtual void FixedUpdate() = 0;
    virtual void Render() = 0;
    virtual void OnExit() = 0;
    virtual void OnDelete() = 0;

    const std::string& GetName() const { return mName; }

protected:
    const std::string mName;
    int mID;
    SceneManager* mSceneManager;
};

