#pragma once
#include "../Engine/SceneBase.h"

class SceneManager;

class GameState : public SceneBase
{
public:
    GameState(const std::string& Name, const int& ID, SceneManager* SceneManager);
    ~GameState();

    void OnCreate() override;
    void OnEnter() override;
    void Update(double DeltaTime) override;
    void FixedUpdate() override;
    void Render() override;
    void OnExit() override;
    void OnDelete() override;

private:
    //GameMap* m_map;
};
