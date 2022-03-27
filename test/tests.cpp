#include <gtest/gtest.h>
#include "../src/Game.h"

CodeArray zeroArray;

template <unsigned int A, unsigned int B, unsigned int C, unsigned int D> 
CodeArray codeArray{CODE_COLOR(A), CODE_COLOR(B), CODE_COLOR(C), CODE_COLOR(D)};

template <unsigned int A, unsigned int B, unsigned int C, unsigned int D> 
FeedbackArray feedbackArray{FEEDBACK_COLOR(A), FEEDBACK_COLOR(B), FEEDBACK_COLOR(C), FEEDBACK_COLOR(D)};

template<typename T>
void compareArrays(T& x, T& y) {
    ASSERT_EQ(x.size(), y.size()) << "Arrays are of unequal length";

    for (unsigned int i = 0; i < x.size(); ++i) {
        ASSERT_EQ(x[i], y[i]) << "Arrays differ at index " << i;
    }
}

TEST(GameTest, CheckGameDefaultValues) {
    Game game;
    auto status = game.getGameStatus();
    ASSERT_EQ(status.state, GameStatus::GAME_STATE::ROUND_STARTING);
    ASSERT_EQ(status.p1Score, 0);
    ASSERT_EQ(status.p2Score, 0);
    ASSERT_EQ(status.roundCounter, 0);
    ASSERT_EQ(status.guessCounter, 0);
    ASSERT_EQ(status.codeMaker, GameStatus::PLAYER::PLAYER_1);
    compareArrays(status.code, zeroArray);
}
 
TEST(GameTest, SetCode) {
    Game game;
    auto status = game.getGameStatus();
    compareArrays(status.code, zeroArray);
    status = game.setCode(codeArray<1,2,3,4>);
    compareArrays(status.code, codeArray<1,2,3,4>);
    ASSERT_EQ(status.state, GameStatus::GAME_STATE::ROUND_ONGOING);
    ASSERT_EQ(status.p1Score, 0);
    ASSERT_EQ(status.p2Score, 0);
    ASSERT_EQ(status.roundCounter, 0);
    ASSERT_EQ(status.guessCounter, 0);
    ASSERT_EQ(status.codeMaker, GameStatus::PLAYER::PLAYER_1);
}

TEST(GameTest, GuessAtFirstTry) {
    Game game;
    game.setCode(codeArray<1,2,3,4>);
    FeedbackArray feedback;
    auto status = game.makeGuess(codeArray<1,2,3,4>, feedback);
    compareArrays(feedback, feedbackArray<2,2,2,2>);
    ASSERT_EQ(status.state, GameStatus::GAME_STATE::ROUND_STARTING);
    ASSERT_EQ(status.p1Score, 0);
    ASSERT_EQ(status.p2Score, 0);
    ASSERT_EQ(status.roundCounter, 1);
    ASSERT_EQ(status.guessCounter, 0);
    ASSERT_EQ(status.codeMaker, GameStatus::PLAYER::PLAYER_2);
}

TEST(GameTest, GuessAtSecondTry) {
    Game game;
    game.setCode(codeArray<1,2,3,4>);
    FeedbackArray feedback;
    auto status = game.makeGuess(codeArray<3,4,5,6>, feedback);
    compareArrays(feedback, feedbackArray<1,1,0,0>);
    ASSERT_EQ(status.state, GameStatus::GAME_STATE::ROUND_ONGOING);
    ASSERT_EQ(status.p1Score, 1);
    ASSERT_EQ(status.p2Score, 0);
    ASSERT_EQ(status.roundCounter, 0);
    ASSERT_EQ(status.guessCounter, 1);
    ASSERT_EQ(status.codeMaker, GameStatus::PLAYER::PLAYER_1);
    status = game.makeGuess(codeArray<1,2,3,4>, feedback);
    ASSERT_EQ(status.state, GameStatus::GAME_STATE::ROUND_STARTING);
    ASSERT_EQ(status.p1Score, 1);
    ASSERT_EQ(status.p2Score, 0);
    ASSERT_EQ(status.roundCounter, 1);
    ASSERT_EQ(status.guessCounter, 0);
    ASSERT_EQ(status.codeMaker, GameStatus::PLAYER::PLAYER_2);
}

TEST(GameTest, FeedbackTest) {
    Game game;
    game.setCode(codeArray<1,1,1,1>);
    FeedbackArray feedback;
    auto status = game.makeGuess(codeArray<2,2,2,2>, feedback);
    compareArrays(feedback, feedbackArray<0,0,0,0>);
    status = game.makeGuess(codeArray<3,4,5,6>, feedback);
    compareArrays(feedback, feedbackArray<0,0,0,0>);
    status = game.makeGuess(codeArray<4,3,2,1>, feedback);
    compareArrays(feedback, feedbackArray<0,0,0,2>);
    status = game.makeGuess(codeArray<5,1,2,1>, feedback);
    compareArrays(feedback, feedbackArray<0,2,0,2>);
    status = game.makeGuess(codeArray<1,1,1,2>, feedback);
    compareArrays(feedback, feedbackArray<2,2,2,0>);
    ASSERT_EQ(status.guessCounter, 5);
    status = game.makeGuess(codeArray<1,1,1,1>, feedback);
    compareArrays(feedback, feedbackArray<2,2,2,2>);
    ASSERT_EQ(status.state, GameStatus::GAME_STATE::ROUND_STARTING);
    ASSERT_EQ(status.p1Score, 5);
    ASSERT_EQ(status.p2Score, 0);
    ASSERT_EQ(status.roundCounter, 1);
    ASSERT_EQ(status.guessCounter, 0);
    ASSERT_EQ(status.codeMaker, GameStatus::PLAYER::PLAYER_2);
}

TEST(GameTest, CustomGameParameters) {
    Game game(2, 2, 1);
    game.setCode(codeArray<1,1,1,1>);
    FeedbackArray feedback;
    game.makeGuess(codeArray<2,2,2,2>, feedback);
    game.setCode(codeArray<2,2,2,2>);
    auto status = game.makeGuess(codeArray<2,2,2,2>, feedback);
    ASSERT_EQ(status.state, GameStatus::GAME_STATE::FINISHED);
    ASSERT_EQ(status.p1Score, 2);
    ASSERT_EQ(status.p2Score, 0);
}

TEST(GameTest, SetCodeDuringOngoingRound) {
    Game game;
    game.setCode(codeArray<1,1,1,1>);

    EXPECT_THROW({
        try
        {
            game.setCode(codeArray<1,2,2,2>);
        }
        catch( const std::runtime_error& e )
        {
            // and this tests that it has the correct message
            EXPECT_STREQ( "Failed to set code, game expects to input guess", e.what() );
            throw;
        }
    }, std::runtime_error );
    
    FeedbackArray feedback;
    auto status = game.makeGuess(codeArray<1,1,1,1>, feedback);
    compareArrays(feedback, feedbackArray<2,2,2,2>);
    ASSERT_EQ(status.state, GameStatus::GAME_STATE::ROUND_STARTING);
    ASSERT_EQ(status.p1Score, 0);
    ASSERT_EQ(status.p2Score, 0);
    ASSERT_EQ(status.roundCounter, 1);
    ASSERT_EQ(status.guessCounter, 0);
    ASSERT_EQ(status.codeMaker, GameStatus::PLAYER::PLAYER_2);
}


TEST(GameTest, CodeWithEmptyColorWhenItsBanned) {
    Game game(2, 2, 1, false);
    EXPECT_THROW({
        try
        {
            game.setCode(codeArray<1,1,1,0>);
        }
        catch( const std::runtime_error& e )
        {
            // and this tests that it has the correct message
            EXPECT_STREQ( "Invalid code", e.what() );
            throw;
        }
    }, std::runtime_error );
    game.setCode(codeArray<1,1,1,1>);
}

TEST(GameTest, InvalidCode) {
    Game game;
    EXPECT_THROW({
        try
        {
            game.setCode(codeArray<9,1,1,1>);
        }
        catch( const std::runtime_error& e )
        {
            // and this tests that it has the correct message
            EXPECT_STREQ( "Invalid code", e.what() );
            throw;
        }
    }, std::runtime_error );
}

TEST(GameTest, InvalidGuessCode) {
    Game game(4,6);
    game.setCode(codeArray<1,1,1,1>);
    FeedbackArray feedback;
    EXPECT_THROW({
        try
        {
            game.makeGuess(codeArray<7,1,1,1>, feedback);
        }
        catch( const std::runtime_error& e )
        {
            // and this tests that it has the correct message
            EXPECT_STREQ( "Invalid guess code", e.what() );
            throw;
        }
    }, std::runtime_error );
}

TEST(GameTest, OddRoundNumber) {
    EXPECT_THROW({
        try
        {
            Game game(3);
        }
        catch( const std::runtime_error& e )
        {
            // and this tests that it has the correct message
            EXPECT_STREQ( "Number of rounds has to be even", e.what() );
            throw;
        }
    }, std::runtime_error );
}

TEST(GameTest, ColorsNumberTooLow) {
    EXPECT_THROW({
        try
        {
            Game game(2,1);
        }
        catch( const std::runtime_error& e )
        {
            // and this tests that it has the correct message
            EXPECT_STREQ( "Number of colors has to be in range 2-9", e.what() );
            throw;
        }
    }, std::runtime_error );
}

TEST(GameTest, ColorsNumberTooHigh) {
    EXPECT_THROW({
        try
        {
            Game game(2,11);
        }
        catch( const std::runtime_error& e )
        {
            // and this tests that it has the correct message
            EXPECT_STREQ( "Number of colors has to be in range 2-9", e.what() );
            throw;
        }
    }, std::runtime_error );
}

TEST(GameTest, DataTest1) {
    Game game;
    for (unsigned int i = 0; i < 6; i++) {
        game.setCode(codeArray<1,2,3,4>);
        FeedbackArray feedback;
        game.makeGuess(codeArray<2,2,2,2>, feedback);
        game.makeGuess(codeArray<1,2,3,4>, feedback);
        game.setCode(codeArray<1,2,3,4>);
        game.makeGuess(codeArray<1,2,3,4>, feedback);
    }
    auto status = game.getGameStatus();
    ASSERT_EQ(status.state, GameStatus::GAME_STATE::FINISHED);
    ASSERT_EQ(status.p1Score, 6);
    ASSERT_EQ(status.p2Score, 0);
    ASSERT_EQ(status.roundCounter, 12);
    ASSERT_EQ(status.guessCounter, 0);
}

TEST(GameTest, DataTest2) {
    Game game;
    for (unsigned int i = 0; i < 6; i++) {
        game.setCode(codeArray<2,2,2,2>);
        FeedbackArray feedback;
        game.makeGuess(codeArray<2,2,2,2>, feedback);
        game.setCode(codeArray<1,2,3,4>);
        game.makeGuess(codeArray<2,2,2,2>, feedback);
        game.makeGuess(codeArray<2,2,2,2>, feedback);
        game.makeGuess(codeArray<2,2,2,2>, feedback);
        game.makeGuess(codeArray<1,2,3,4>, feedback);
    }
    auto status = game.getGameStatus();
    ASSERT_EQ(status.state, GameStatus::GAME_STATE::FINISHED);
    ASSERT_EQ(status.p1Score, 0);
    ASSERT_EQ(status.p2Score, 18);
}

TEST(GameTest, DataTest3) {
    Game game(12, 6, 8, true, false);
    for (unsigned int i = 0; i < 6; i++) {
        game.setCode(codeArray<2,2,2,2>);
        FeedbackArray feedback;
        for (unsigned int j = 0; j < 8; j++) {
            game.makeGuess(codeArray<2,2,0,1>, feedback);
        }
        game.setCode(codeArray<1,2,3,4>);
        game.makeGuess(codeArray<2,2,2,2>, feedback);
        game.makeGuess(codeArray<2,2,2,2>, feedback);
        game.makeGuess(codeArray<2,2,2,2>, feedback);
        game.makeGuess(codeArray<1,2,3,4>, feedback);
    }
    auto status = game.getGameStatus();
    ASSERT_EQ(status.state, GameStatus::GAME_STATE::FINISHED);
    ASSERT_EQ(status.p1Score, 18);
    ASSERT_EQ(status.p2Score, 54);
}

TEST(GameTest, DataTest4) {
    Game game(32);
    for (unsigned int i = 0; i < 16; i++) {
        game.setCode(codeArray<2,2,2,2>);
        FeedbackArray feedback;
        game.makeGuess(codeArray<2,2,2,2>, feedback);
        game.setCode(codeArray<1,2,3,4>);
        game.makeGuess(codeArray<0,2,2,2>, feedback);
        game.makeGuess(codeArray<1,2,3,4>, feedback);
    }
    auto status = game.getGameStatus();
    ASSERT_EQ(status.state, GameStatus::GAME_STATE::FINISHED);
    ASSERT_EQ(status.p1Score, 0);
    ASSERT_EQ(status.p2Score, 16);
    EXPECT_THROW({
        try
        {
            game.setCode(codeArray<1,2,2,2>);
        }
        catch( const std::runtime_error& e )
        {
            // and this tests that it has the correct message
            EXPECT_STREQ( "Failed to set code, game is finished", e.what() );
            throw;
        }
    }, std::runtime_error );
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}