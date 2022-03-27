#ifndef GAME_H
#define GAME_H

#include "Common.h"
#include <unordered_set>

class Game {
public:
    Game(unsigned int nRounds = DEFAULT_ROUND_COUNT,
         unsigned int nColors = DEFAULT_COLOR_COUNT,
         unsigned int nGuesses = DEFAULT_GUESS_COUNT,
         bool isBlankAllowed = DEFAULT_BLANK_ALLOWED,
         bool isP1Start = DEFAULT_P1_STARTS);
    const GameStatus setCode(CodeArray& newCode);
    const GameStatus makeGuess(CodeArray& guess, FeedbackArray& feedback);
    const GameStatus getGameStatus();

private:
    void flushMatchingPositions(CodeArray& guess, FeedbackArray& feedback);
    void flushNotMatchingPositions(CodeArray& guess, FeedbackArray& feedback);
    void prepareGuessContainers(FeedbackArray& feedback);
    void checkResult(FeedbackArray& feedback);
    void grantPoint();
    void switchCodeMaker();
    bool validateCode(CodeArray& code);

    GameStatus gameStatus;
    std::unordered_multiset<CODE_COLOR> notMatchedColors;
    bool blankAllowed;
    const unsigned int numberOfRounds;
    const unsigned int numberOfColors;
    const unsigned int numberOfGuesses;

};

#endif // GAME_H