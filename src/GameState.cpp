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

    for (auto gameState : gameStates_)
    {
        gameState->PutFirstState(gameStates_.begin());
    }

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

void GameStateManager::RunCurrentState(Minesweeper &ms, const Action &action)
{
    GameStates::iterator arrayIt =
        *currentState_;          // WskaŸnik na element tablicy (którym jest GameState*) aka GameState**
    GameState *state = *arrayIt; // Konkretny wskaŸnik na GameState
    if (state->OnRun(ms, action) == RunResult::SwitchToNext)
        currentState_++;
}

GameStateManager::GameStatesIterator::GameStatesIterator(GameStates::iterator arrayIt) : arrayIt_{arrayIt}
{
}

GameStateManager::GameStatesIterator GameStateManager::GameStatesIterator::operator++(int)
{
    (*arrayIt_)->OnExit();

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

RunResult GameState::OnRun(Minesweeper &ms, const Action &action)
{
    if (TryGameRestart(action))
    {
        return RunResult::SwitchToNext;
    }

    if (firstRun_)
    {
        firstRun_ = false;
        return OnEntry(ms, action);
    }
    else
        return Run(ms, action);
}

bool GameState::TryGameRestart(const Action &action)
{
    if (action.actionType_ == ActionType::Restart)
    {
        PutNextState(firstState_);
        return true;
    }
    return false;
}

RunResult RunningFirstCheck::OnEntry(Minesweeper &ms, const Action &action)
{
    ms.CreateEmptyBoard(ms.boardSizeX_, ms.boardSizeY_);
    ms.userInput_->Init(ms.cells_, ms.flagAmount_);
    ms.flagAmount_ = ms.mineAmount_;
    return RunResult::Stay;
}

RunResult RunningFirstCheck::Run(Minesweeper &ms, const Action &action)
{
    ms.userInput_->DrawGameNotRunning();
    auto [x, y] = action.playerPos_;
    if (action.actionType_ == ActionType::CheckCell)
    {
        auto minePositions = ms.randomEngine_->RandomizeMinePlacement(ms.cells_, ms.mineAmount_, x, y);
        ms.PlaceMines(minePositions);
        return RunResult::SwitchToNext;
    }
    return RunResult::Stay;
}

RunResult Running::OnEntry(Minesweeper &ms, const Action &action)
{
    return RunResult::Stay;
}

RunResult Running::Run(Minesweeper &ms, const Action &action)
{
    ms.userInput_->DrawGameRunning();

    bool gameLost = false;

    auto [x, y] = action.playerPos_;
    if (x < 0 || x > ms.cells_.size() || y < 0 || y > ms.cells_[0].size())
        throw std::out_of_range("User input is out of range\n");
    else if (action.actionType_ == ActionType::CheckCell)
        gameLost = !ms.ExecuteCheckCell(x, y);
    else if (action.actionType_ == ActionType::MarkCell)
        ms.ExecuteMarkCell(x, y);

    if (ms.IsGameWon())
    {
        PutNextState(wonState_);

        return RunResult::SwitchToNext;
    }
    else if (gameLost)
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

RunResult Lost::OnEntry(Minesweeper &ms, const Action &action)
{
    // ms.userInput_->OnResultMine(x, y);
    ms.UncoverAllMines();
    return RunResult::Stay;
}

RunResult Lost::Run(Minesweeper &ms, const Action &action)
{
    ms.userInput_->DrawGameNotRunning();
    return RunResult::Stay;
}

RunResult Won::OnEntry(Minesweeper &ms, const Action &action)
{
    ms.userInput_->OnGameWon();
    return RunResult::Stay;
}

RunResult Won::Run(Minesweeper &ms, const Action &action)
{
    ms.userInput_->DrawGameNotRunning();
    return RunResult::Stay;
}
