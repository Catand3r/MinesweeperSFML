#include "../src/Minesweeper.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
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
    MOCK_METHOD(Position, GetPos, (), (override));
    MOCK_METHOD(void, OnResultEmpty, (int, int, int), (override));
    MOCK_METHOD(void, OnResultMine, (int, int), (override));
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

    EXPECT_CALL(*randomEngineMock_, RandomizeMinePlacement(testing::_, testing::_)).WillOnce(Return(minePositions));
    EXPECT_CALL(*userInputMock_, GetPos()).WillOnce(Return(userPosition));
    EXPECT_CALL(*userInputMock_, OnResultMine(userPosition.first, userPosition.second)).Times(1);

    Minesweeper ms = CreateSut();
    const bool runResult = ms.Run();

    ASSERT_FALSE(runResult);
}

TEST_F(MinesweeperTests, ShouldCallOnResultEmptyWhenUserPositionIsDifferentThanMinePosition)
{
    MinePositions minePositions{{1, 1}};
    Position userPosition{0, 1};

    EXPECT_CALL(*randomEngineMock_, RandomizeMinePlacement(testing::_, testing::_)).WillOnce(Return(minePositions));
    EXPECT_CALL(*userInputMock_, GetPos()).WillOnce(Return(userPosition));
    EXPECT_CALL(*userInputMock_, OnResultEmpty(userPosition.first, userPosition.second, testing::_)).Times(1);

    Minesweeper ms = CreateSut();
    const bool runResult = ms.Run();

    ASSERT_TRUE(runResult);
}

TEST_F(MinesweeperTests, ShouldThrowExceptionWhenUserPositionIsIncorrect)
{
    Position userPosition{-741398, 91430};

    EXPECT_CALL(*userInputMock_, GetPos()).WillOnce(Return(userPosition));
    EXPECT_CALL(*randomEngineMock_, RandomizeMinePlacement(testing::_, testing::_));

    Minesweeper ms = CreateSut();

    EXPECT_ANY_THROW(ms.Run());
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

    EXPECT_CALL(*userInputMock_, GetPos()).WillOnce(Return(userPosition));
    EXPECT_CALL(*randomEngineMock_, RandomizeMinePlacement(testing::_, testing::_)).WillOnce(Return(minePositions));
    if (result)
        EXPECT_CALL(*userInputMock_, OnResultEmpty(userPosition.first, userPosition.second, testing::_));
    else
        EXPECT_CALL(*userInputMock_, OnResultMine(userPosition.first, userPosition.second));

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