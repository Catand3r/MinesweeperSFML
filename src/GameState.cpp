#include "GameState.h"
#include <array>
#include <string>

GameStateManager::GameStateManager()
    : gameStates_{new RunningFirstCheck(), new Running(), new Lost(), new Won()}, currentState_{gameStates_.begin()}
{
    GameState *first = gameStates_[0];
    GameState *running = gameStates_[1];
    GameState *lost = gameStates_[2];
    GameState *won = gameStates_[3];

    first->PutNextState(gameStates_.begin() + 1);
    if (Running *run = dynamic_cast<Running *>(running))
    {
        run->PutLostState(gameStates_.begin() + 2);
        run->PutWonState(gameStates_.begin() + 3);
    }
    lost->PutNextState(gameStates_.begin());
    won->PutNextState(gameStates_.begin());
}

GameStateManager::~GameStateManager()
{
    for (auto gameState : gameStates_)
    {
        delete gameState;
    }
}

void GameStateManager::RunCurrentState()
{
    GameStates::iterator arrayIt =
        *currentState_;          // WskaŸnik na element tablicy (którym jest GameState*) aka GameState**
    GameState *state = *arrayIt; // Konkretny wskaŸnik na GameState
    if (state->Run() == RunResult::SwitchToNext)
        currentState_++;
}

GameStateManager::GameStatesIterator::GameStatesIterator(GameStates::iterator arrayIt) : arrayIt_{arrayIt}
{
}

GameStateManager::GameStatesIterator GameStateManager::GameStatesIterator::operator++(int)
{
    arrayIt_ = (*arrayIt_)->GetNextState(arrayIt_);

    return *this;
}

GameStateManager::GameStates::iterator GameStateManager::GameStatesIterator::operator*()
{
    return arrayIt_;
}

void GameState::PutNextState(GameStateArrayIt nextState)
{
    nextState_ = nextState;
}

void RunningFirstCheck::OnEntry()
{
}

RunResult RunningFirstCheck::Run()
{
    return RunResult::Stay;
}

void Running::OnEntry()
{
}

RunResult Running::Run()
{
    return RunResult::Stay;
}

void Running::PutLostState(GameStateArrayIt lostState)
{
    lostState_ = lostState;
}

void Running::PutWonState(GameStateArrayIt wonState)
{
    wonState_ = wonState;
}

void Lost::OnEntry()
{
}

RunResult Lost::Run()
{
    return RunResult::Stay;
}

void Won::OnEntry()
{
}

RunResult Won::Run()
{
    return RunResult::Stay;
}
