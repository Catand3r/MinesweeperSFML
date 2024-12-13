
#pragma once
#include <array>
#include <string>
#include <vector>

enum class RunResult
{
    SwitchToNext = 0,
    Stay
};

class GameState;
class Minesweeper;
struct Action;

class GameStateManager
{
    friend GameState;

  public:
    using GameStates = std::array<GameState *, 4>;

  private:
    class GameStatesIterator
    {
      public:
        GameStatesIterator(GameStates::iterator arrayIt);
        GameStatesIterator operator++(int);
        GameStates::iterator operator*();

      private:
        GameStates::iterator arrayIt_;
    };

  public:
    GameStateManager();
    ~GameStateManager();
    void RunCurrentState(Minesweeper &, const Action &);

  private:
    GameStates gameStates_;
    GameStatesIterator currentState_;
};

using GameStateArrayIt = GameStateManager::GameStates::iterator;

class GameState
{
  protected:
    using GameStateIterator = GameStateManager::GameStatesIterator;

  public:
    RunResult OnRun(Minesweeper &, const Action &);
    virtual RunResult Run(Minesweeper &, const Action &) = 0;
    virtual GameStateArrayIt GetNextState(GameStateArrayIt currentState)
    {
        currentState++;
        return currentState;
    }
    void OnExit()
    {
        firstRun_ = true;
    }

    virtual void PutNextState(GameStateArrayIt);

    void PutFirstState(GameStateArrayIt firstState)
    {
        firstState_ = firstState;
    }

  protected:
    bool firstRun_ = true;

    GameStateArrayIt nextState_;

    GameStateArrayIt firstState_;

    std::string name_;

    virtual RunResult OnEntry(Minesweeper &, const Action &) = 0;

    bool TryGameRestart(const Action &);
};

class RunningFirstCheck : public GameState
{
  public:
    RunResult Run(Minesweeper &, const Action &) override;

  private:
    RunResult OnEntry(Minesweeper &, const Action &) override;
};

class Running : public GameState
{
  public:
    RunResult Run(Minesweeper &, const Action &) override;
    void PutLostState(GameStateArrayIt);
    void PutWonState(GameStateArrayIt);
    GameStateArrayIt GetNextState(GameStateArrayIt) override
    {
        return nextState_;
    }

  private:
    RunResult OnEntry(Minesweeper &, const Action &) override;

    GameStateArrayIt lostState_;
    GameStateArrayIt wonState_;
};

class Lost : public GameState
{
  public:
    RunResult Run(Minesweeper &, const Action &) override;
    GameStateArrayIt GetNextState(GameStateArrayIt) override
    {
        return nextState_;
    }

  private:
    RunResult OnEntry(Minesweeper &, const Action &) override;
};

class Won : public GameState
{
  public:
    RunResult Run(Minesweeper &, const Action &) override;
    GameStateArrayIt GetNextState(GameStateArrayIt) override
    {
        return nextState_;
    }

  private:
    RunResult OnEntry(Minesweeper &, const Action &) override;
};
