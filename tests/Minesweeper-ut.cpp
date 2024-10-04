#include "../src/Minesweeper.h"
#include "gtest/gtest.h"
#include <gmock/gmock.h>
#include <iostream>
#include <utility>

using ::testing::Return;

class MockRandomEngine : public IRandomEngine
{
public:
    MOCK_METHOD(MinePositions, RandomizeMinePlacement, (Cells, int), (override));
};

class MockUserInput : public IUserInput
{
public:
    MOCK_METHOD(bool, Init, (const Cells &, const MinePositions &), (override));
    MOCK_METHOD(void, OnResultEmpty, (int, int, int), (override));
    MOCK_METHOD(void, OnResultMine, (int, int), (override));
    MOCK_METHOD(void, OnMarkCell, (int, int), (override));
    MOCK_METHOD(Action, PollEvent, (), (override));
    MOCK_METHOD(void, Delay, (int), (const override));

};

class MinesweeperTests : public ::testing::Test
{
protected:
    void SetUp() override
    {
        userInputMock_ = new MockUserInput();
        randomEngineMock_ = new MockRandomEngine();
    }

    void TearDown() override
    {
        delete userInputMock_;
        delete randomEngineMock_;
    }

    Action CreateCheckCellAction(Position userPosition)
    {
        Action action;
        action.actionType_ = ActionType::CheckCell;
        action.playerPos_ = userPosition;
        return action;
    }

    Action CreateNoneAction(Position userPosition)
    {
        Action action;
        action.actionType_ = ActionType::None;
        action.playerPos_ = userPosition;
        return action;
    }

    Minesweeper CreateSut()
    {
        return Minesweeper(userInputMock_, randomEngineMock_);
    }

    MockUserInput *userInputMock_;
    MockRandomEngine *randomEngineMock_;
};

struct MinesweeperTestParameters
{
    MinePositions minePositions;
    Position userPosition;
    bool result;
};

class MinesweeperParametricTests : public ::testing::TestWithParam<MinesweeperTestParameters>
{
};

TEST_F(MinesweeperTests, ShouldCallOnResultMineWhenUserPositionIsEqualToMinePosition)
{
    MinePositions minePositions{{0, 1}};
    Position userPosition{0, 1};

    Action action = CreateCheckCellAction(userPosition);

    EXPECT_CALL(*randomEngineMock_, RandomizeMinePlacement(testing::_, testing::_)).WillOnce(Return(minePositions));
    EXPECT_CALL(*userInputMock_, OnResultMine(userPosition.first, userPosition.second)).Times(1);
    EXPECT_CALL(*userInputMock_, Init(testing::_, testing::_));
    EXPECT_CALL(*userInputMock_, PollEvent()).WillOnce(Return(action));

    Minesweeper ms = CreateSut();
    const bool runResult = ms.Run();

    ASSERT_FALSE(runResult);
}

TEST_F(MinesweeperTests, ShouldCallOnResultEmptyWhenUserPositionIsDifferentThanMinePosition)
{
    MinePositions minePositions{{1, 1}};
    Position userPosition{0, 1};

    Action action = CreateCheckCellAction(userPosition);

    EXPECT_CALL(*randomEngineMock_, RandomizeMinePlacement(testing::_, testing::_)).WillOnce(Return(minePositions));
    EXPECT_CALL(*userInputMock_, OnResultEmpty(userPosition.first, userPosition.second, testing::_)).Times(1);
    EXPECT_CALL(*userInputMock_, Init(testing::_, testing::_));
    EXPECT_CALL(*userInputMock_, PollEvent()).WillRepeatedly(Return(action));

    Minesweeper ms = CreateSut();
    const bool runResult = ms.Run();

    ASSERT_TRUE(runResult);
}

TEST_F(MinesweeperTests, ShouldThrowExceptionWhenUserPositionIsIncorrect)
{
    Position userPosition{-741398, 91430};

    Action action = CreateCheckCellAction(userPosition);

    EXPECT_CALL(*randomEngineMock_, RandomizeMinePlacement(testing::_, testing::_));
    EXPECT_CALL(*userInputMock_, Init(testing::_, testing::_));
    EXPECT_CALL(*userInputMock_, PollEvent()).WillRepeatedly(Return(action));

    Minesweeper ms = CreateSut();

    EXPECT_ANY_THROW(ms.Run());
}

TEST_F(MinesweeperTests, ShouldCallDelayAndReturnTrueWhenUserPositionIsNotGiven)
{
    Position dummyUserPosition{ 0, 0 };

    Action action = CreateNoneAction(dummyUserPosition);

    EXPECT_CALL(*randomEngineMock_, RandomizeMinePlacement(testing::_, testing::_));
    EXPECT_CALL(*userInputMock_, Init(testing::_, testing::_));
    EXPECT_CALL(*userInputMock_, Delay(testing::_));
    EXPECT_CALL(*userInputMock_, PollEvent()).WillRepeatedly(Return(action));

    Minesweeper ms = CreateSut();

    EXPECT_TRUE(ms.Run());
}

TEST_P(MinesweeperParametricTests, GivenMinePositionsAndUserPositionWhenRunIsCalledThenPropertyResultShouldBeReturn)
{
    MinesweeperTestParameters params = GetParam();

    MinePositions minePositions = params.minePositions;
    Position userPosition = params.userPosition;
    bool result = params.result;

    MockUserInput *userInputMock_;
    MockRandomEngine *randomEngineMock_;

    userInputMock_ = new MockUserInput();
    randomEngineMock_ = new MockRandomEngine();

    Action action;
    action.playerPos_ = userPosition;
    action.actionType_ = ActionType::CheckCell;

    EXPECT_CALL(*userInputMock_, Init(testing::_, testing::_));
    EXPECT_CALL(*randomEngineMock_, RandomizeMinePlacement(testing::_, testing::_)).WillOnce(Return(minePositions));
    EXPECT_CALL(*userInputMock_, PollEvent()).WillRepeatedly(Return(action));
    if (result)
        EXPECT_CALL(*userInputMock_, OnResultEmpty(userPosition.first, userPosition.second, testing::_));
    else
    {
        EXPECT_CALL(*userInputMock_, OnResultMine(userPosition.first, userPosition.second));
        EXPECT_CALL(*userInputMock_, Delay(testing::_));
    }
    Minesweeper ms(userInputMock_, randomEngineMock_);

    EXPECT_EQ(ms.Run(), result);

    delete userInputMock_;
    delete randomEngineMock_;
}

INSTANTIATE_TEST_SUITE_P(GivenMinePositionsAndUserPositionWhenRunIsCalledThenPropertyResultShouldBeReturn,
                         MinesweeperParametricTests,
                         ::testing::Values(
                             MinesweeperTestParameters{{{0, 1}}, {1, 0}, true},
                             MinesweeperTestParameters{{{0, 1}}, {0, 1}, false}));

struct CheckAroundCellTestParameters
{
    MinePositions minePositions;
    Position userPosition;
    const int result;
};

class CheckAroundCellParametricTests : public ::testing::TestWithParam<CheckAroundCellTestParameters>
{
};

TEST_P(CheckAroundCellParametricTests, ShouldCallOnResultEmptyWithSpecificMineAroundAmountWhenMineNotHit)
{
    CheckAroundCellTestParameters params = GetParam();

    MinePositions minePositions = params.minePositions;
    Position userPosition = params.userPosition;
    const int expectedMines = params.result;

    MockUserInput* userInputMock_;
    MockRandomEngine* randomEngineMock_;

    userInputMock_ = new MockUserInput();
    randomEngineMock_ = new MockRandomEngine();

    Action action;
    action.playerPos_ = userPosition;
    action.actionType_ = ActionType::CheckCell;

    EXPECT_CALL(*randomEngineMock_, RandomizeMinePlacement(testing::_, testing::_)).WillOnce(Return(minePositions));
    EXPECT_CALL(*userInputMock_, OnResultEmpty(userPosition.first, userPosition.second, expectedMines)).Times(1);
    EXPECT_CALL(*userInputMock_, Init(testing::_, testing::_));
    EXPECT_CALL(*userInputMock_, PollEvent()).WillRepeatedly(Return(action));

    Minesweeper ms(userInputMock_, randomEngineMock_);
    const bool runResult = ms.Run();

    delete userInputMock_;
    delete randomEngineMock_;

    ASSERT_TRUE(runResult);
}

INSTANTIATE_TEST_SUITE_P(ShouldCallOnResultEmptyWithSpecificMineAroundAmountWhenMineNotHit,
    CheckAroundCellParametricTests,
    ::testing::Values(
        // Lewy górny róg (x=0, y=0)
        CheckAroundCellTestParameters{ {{1, 0}, {1, 1}, {0, 1}}, {0, 0}, 3 },
        CheckAroundCellTestParameters{ {{1, 1}, {0, 1}}, {0, 0}, 2 },

        // Prawy górny róg (x=9, y=0)
        CheckAroundCellTestParameters{ {{8, 0}, {8, 1}, {9, 1}}, {9, 0}, 3 },
        CheckAroundCellTestParameters{ {{8, 0}, {9, 1}}, {9, 0}, 2 },

        // Lewy dolny róg (x=0, y=7)
        CheckAroundCellTestParameters{ {{0, 6}, {1, 6}, {1, 7}}, {0, 7}, 3 },
        CheckAroundCellTestParameters{ {{1, 7}, {1, 6}}, {0, 7}, 2 },

        // Prawy dolny róg (x=9, y=7)
        CheckAroundCellTestParameters{ {{8, 7}, {8, 6}, {9, 6}}, {9, 7}, 3 },
        CheckAroundCellTestParameters{ {{8, 7}, {9, 6}}, {9, 7}, 2 },

        // Górny brzeg (œrodkowy punkt, x=5, y=0)
        CheckAroundCellTestParameters{ {{4, 0}, {4, 1}, {5, 1}, {6, 0}, {6, 1}}, {5, 0}, 5 },
        CheckAroundCellTestParameters{ {{4, 1}, {5, 1}}, {5, 0}, 2 },

        // Dolny brzeg (œrodkowy punkt, x=5, y=7)
        CheckAroundCellTestParameters{ {{4, 7}, {4, 6}, {5, 6}, {6, 6}, {6, 7}}, {5, 7}, 5 },
        CheckAroundCellTestParameters{ {{4, 7}, {5, 6}}, {5, 7}, 2 },

        // Lewy brzeg (œrodkowy punkt, x=0, y=4)
        CheckAroundCellTestParameters{ {{0, 3}, {1, 3}, {1, 4}, {0, 5}, {1, 5}}, {0, 4}, 5 },
        CheckAroundCellTestParameters{ {{1, 3}, {1, 4}}, {0, 4}, 2 },

        // Prawy brzeg (œrodkowy punkt, x=9, y=4)
        CheckAroundCellTestParameters{ {{8, 3}, {8, 4}, {9, 3}, {8, 5}, {9, 5}}, {9, 4}, 5 },
        CheckAroundCellTestParameters{ {{8, 4}, {9, 5}}, {9, 4}, 2 },

        // Wnêtrze planszy (punkt œrodkowy, x=5, y=4)
        CheckAroundCellTestParameters{ {{4, 3}, {4, 4}, {4, 5}, {5, 3}, {5, 5}, {6, 3}, {6, 4}, {6, 5}}, {5, 4}, 8 },
        CheckAroundCellTestParameters{ {{4, 4}, {5, 3}, {6, 4}}, {5, 4}, 3 }));