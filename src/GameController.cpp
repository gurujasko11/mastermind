#include "GameController.h"
#include "Common.h"

void GameController::startGame() {
    game = new Game(nRounds, nColors, nGuesses, isBlankAllowed, isP1Start);
}

void GameController::endGame() {
    if (game) {
        delete game;
        game = nullptr;
    }
}

void GameController::setView(View* newView) {
    view = newView;
}

void GameController::play() {
    status = game->getGameStatus();
    while (status.state != GameStatus::GAME_STATE::FINISHED) {
        switch (status.state) {
            case GameStatus::GAME_STATE::ROUND_STARTING: {
                view->handleRoundStart(status);
                CodeArray code;
                bool codeSetSuccesfully = false;
                while(codeSetSuccesfully == false) {
                    try {
                        getCode(code);
                        status = game->setCode(code);
                        codeSetSuccesfully = true;
                    }
                    catch (const std::runtime_error& error) {
                        view->info(error.what());
                    }
                }
                break;
            };
            case GameStatus::GAME_STATE::ROUND_ONGOING: {
                CodeArray guess;
                FeedbackArray feedback;
                bool guessForwardedSuccesfully = false;
                while(guessForwardedSuccesfully == false) {
                    try {
                        getGuess(guess);
                        status = game->makeGuess(guess, feedback);
                        guessForwardedSuccesfully = true;
                    }
                    catch (const std::runtime_error& error) {
                        view->info(error.what());
                    }
                }
                view->handleFeedback(feedback);
                if (status.state != GameStatus::GAME_STATE::ROUND_ONGOING) {
                    view->handleRoundOver(status, feedback);
                }
                break;
            };
            case GameStatus::GAME_STATE::FINISHED: {
                view->handleGameEnd(status);
                return;
            };
            default:
                throw std::runtime_error("Unexpected game state");
        }
    }
    view->handleGameEnd(status);
}

void GameController::getCode(CodeArray& code) {
    if (status.codeMaker == GameStatus::PLAYER::PLAYER_1) {
        requestCodeFromPlayer1(code);
    }
    else {
        requestCodeFromPlayer2(code);
    }
}

void GameController::getGuess(CodeArray& guess) {
    if (status.codeMaker == GameStatus::PLAYER::PLAYER_1) {
        requestGuessFromPlayer2(guess);
    }
    else {
        requestGuessFromPlayer1(guess);
    }
}

void GameController::requestCodeFromPlayer1(CodeArray& code) {
    view->info("[P1]: enter code");
    view->requestCode(code);
}

void GameController::requestGuessFromPlayer1(CodeArray& code) {
    view->info("[P1]: enter guess");
    view->requestCode(code);
}

void GameController::requestCodeFromHuman(CodeArray& code, std::string message) {
    view->info(message);
    view->requestCode(code);
}