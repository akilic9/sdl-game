#include "GameState.h"

GameState::GameState(const std::string& Name, const int& ID, SceneManager* SceneManager)
    : SceneBase(Name, ID, SceneManager)
{
}

GameState::~GameState()
{
}

void GameState::OnCreate()
{
}

void GameState::OnEnter()
{
}

void GameState::Update(double DeltaTime)
{
}

void GameState::FixedUpdate()
{
}

void GameState::Render()
{
}

void GameState::OnExit()
{
}

void GameState::OnDelete()
{
}
