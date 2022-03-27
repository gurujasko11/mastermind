#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "Game.h"
#include "View.h"
#include "TextView.h"

class GameController {
public:
    void play();
    void setView(View* newView);
    void startGame();
    void endGame();
    void getCode(CodeArray& code);
    void getGuess(CodeArray& guess);
    void requestCodeFromPlayer1(CodeArray& code);
    void requestGuessFromPlayer1(CodeArray& code);
    void virtual requestCodeFromPlayer2(CodeArray& guess) = 0;
    void virtual requestGuessFromPlayer2(CodeArray& guess) = 0;

    unsigned int nRounds;
    unsigned int nColors;
    unsigned int nGuesses = 8;
    bool isBlankAllowed = true;
    bool isP1Start = true;

protected:
    void requestCodeFromHuman(CodeArray& code, std::string message);
    View* view;

private:
    GameStatus status;
    Game* game;
};

#endif // GAMECONTROLLER_H