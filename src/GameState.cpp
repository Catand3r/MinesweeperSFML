#include "GameState.h"
#include "Minesweeper.h"
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

void GameStateManager::RunCurrentState(Minesweeper *ms, const Action &action)
{
    GameStates::iterator arrayIt =
        *currentState_;          // WskaŸnik na element tablicy (którym jest GameState*) aka GameState**
    GameState *state = *arrayIt; // Konkretny wskaŸnik na GameState
    if (state->Run(ms, action) == RunResult::SwitchToNext)
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

RunResult RunningFirstCheck::Run(Minesweeper *ms, const Action &action)
{
    (*ms).CreateEmptyBoard((*ms).boardSizeX_, (*ms).boardSizeY_);
    (*ms).CreateEmptyMinePositions();
    (*ms).flagAmount_ = (*ms).mineAmount_;
    (*ms).firstCheck_ = true;
    (*ms).userInput_->Init((*ms).cells_, (*ms).minePostitions_, (*ms).flagAmount_);
    (*ms).userInput_->DrawGameNotRunning();
    auto [x, y] = action.playerPos_;
    if (action.actionType_ == ActionType::CheckCell)
    {
        (*ms).firstCheck_ = false;
        (*ms).minePostitions_ = (*ms).randomEngine_->RandomizeMinePlacement((*ms).cells_, (*ms).mineAmount_, x, y);
        (*ms).PlaceMines();
    }
    if (!(*ms).firstCheck_)
        return RunResult::SwitchToNext;
    return RunResult::Stay;
}

void Running::OnEntry()
{
}

RunResult Running::Run(Minesweeper *ms, const Action &action)
{
    (*ms).userInput_->DrawGameRunning();
    if ((*ms).gameWon_ && !(*ms).gameLost_)
    {
        PutNextState(wonState_);
        return RunResult::SwitchToNext;
    }
    else if ((*ms).gameLost_ && !(*ms).gameWon_)
    {
        PutNextState(lostState_);
        return RunResult::SwitchToNext;
    }
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

RunResult Lost::Run(Minesweeper *ms, const Action &action)
{
    (*ms).UncoverAllMines();
    (*ms).userInput_->DrawGameNotRunning();
    if (action.actionType_ == ActionType::Restart)
        return RunResult::SwitchToNext;
    return RunResult::Stay;
}

void Won::OnEntry()
{
}

RunResult Won::Run(Minesweeper *ms, const Action &action)
{
    (*ms).userInput_->DrawGameNotRunning();
    if (action.actionType_ == ActionType::Restart)
        return RunResult::SwitchToNext;
    return RunResult::Stay;
}
