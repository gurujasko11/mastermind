#include "Game.h"
#include <stdexcept>

Game::Game(unsigned int nRounds, unsigned int nColors, unsigned int nGuesses, bool isBlankAllowed, bool isP1Start)
    : numberOfRounds(nRounds),
      numberOfColors(nColors),
      numberOfGuesses(nGuesses),
      blankAllowed(isBlankAllowed)
{
    if (numberOfRounds % 2 != 0) {
        throw std::runtime_error("Number of rounds has to be even");
    }
    if (numberOfColors < MIN_COLOR_NUMBER || 
        numberOfColors > MAX_COLOR_NUMBER) {
            std::string exceptionMessage = "Number of colors has to be in range ";
            exceptionMessage += std::to_string(MIN_COLOR_NUMBER) + "-" + std::to_string(MAX_COLOR_NUMBER);
        throw std::runtime_error(exceptionMessage);
    }
    if (isP1Start) {
        gameStatus.codeMaker = GameStatus::PLAYER::PLAYER_1;
    }
    else {
        gameStatus.codeMaker = GameStatus::PLAYER::PLAYER_2;
    }
}

const GameStatus Game::setCode(CodeArray& newCode) {
    if (gameStatus.state == GameStatus::GAME_STATE::ROUND_ONGOING) {
        throw std::runtime_error("Failed to set code, game expects to input guess");
    }
    if (gameStatus.state == GameStatus::GAME_STATE::FINISHED) {
        throw std::runtime_error("Failed to set code, game is finished");
    }
    if (validateCode(newCode) == false) {
        throw std::runtime_error("Invalid code");
    }
    gameStatus.code = newCode;
    gameStatus.state = GameStatus::GAME_STATE::ROUND_ONGOING;
    return gameStatus;
}

const GameStatus Game::makeGuess(CodeArray& guess, FeedbackArray& feedback) {
    if (validateCode(guess) == false) {
        throw std::runtime_error("Invalid guess code");
    }
    prepareGuessContainers(feedback);
    flushMatchingPositions(guess, feedback);
    flushNotMatchingPositions(guess, feedback);
    checkResult(feedback);
    return gameStatus;
}

const GameStatus Game::getGameStatus() {
    return gameStatus;
}

void Game::prepareGuessContainers(FeedbackArray& feedback) {
    feedback.fill(FEEDBACK_COLOR::BLANK);
    notMatchedColors.clear();
}

void Game::flushMatchingPositions(CodeArray& guess, FeedbackArray& feedback) {
    for (int i=0; i<CODE_LENGTH; i++) {
        if (gameStatus.code.at(i) == guess.at(i)) {
            feedback.at(i) = FEEDBACK_COLOR::BLACK;
        }
        else {
            notMatchedColors.insert(gameStatus.code.at(i));
        }
    }
}

void Game::flushNotMatchingPositions(CodeArray& guess, FeedbackArray& feedback) {
    for (int i=0; i<CODE_LENGTH; i++) {
        if (feedback.at(i) == FEEDBACK_COLOR::BLANK) {
            auto notMatchedColorIterator = notMatchedColors.find(guess.at(i));
            if (notMatchedColorIterator != notMatchedColors.end()) {
                notMatchedColors.erase(notMatchedColorIterator);
                feedback.at(i) = FEEDBACK_COLOR::WHITE;
            }
        }
    }
}

void Game::checkResult(FeedbackArray& feedback) {
    bool codeSolved = true;
    bool roundEnded = false;
    for (int i=0; i<CODE_LENGTH; i++) {
        if (feedback.at(i) != FEEDBACK_COLOR::BLACK) {
            codeSolved = false;
            break;
        }
    }
    if (codeSolved) {
        roundEnded = true;
    }
    else {
        grantPoint();
        gameStatus.guessCounter++;
        if(gameStatus.guessCounter == numberOfGuesses) {
            grantPoint();
            roundEnded = true;
        }
    }
    if (roundEnded) {
        gameStatus.roundCounter++;
        gameStatus.guessCounter = 0;
        if (gameStatus.roundCounter == numberOfRounds) {
            gameStatus.state = GameStatus::GAME_STATE::FINISHED;
        }
        else {
            switchCodeMaker();
            gameStatus.state = GameStatus::GAME_STATE::ROUND_STARTING;
        }
    }
}

void Game::grantPoint() {
    if (gameStatus.codeMaker == GameStatus::PLAYER::PLAYER_1) {
        gameStatus.p1Score++;
    }
    else {
        gameStatus.p2Score++;
    }
}

void Game::switchCodeMaker() {
    if (gameStatus.codeMaker == GameStatus::PLAYER::PLAYER_1) {
        gameStatus.codeMaker = GameStatus::PLAYER::PLAYER_2;
    }
    else {
        gameStatus.codeMaker = GameStatus::PLAYER::PLAYER_1;
    }
}

bool Game::validateCode(CodeArray& code) {
    for (auto& value : code) {
        if (blankAllowed == false && value == CODE_COLOR::BLANK) {
            return false;
        }
        if (static_cast<unsigned int>(value) > numberOfColors) {
            return false;
        }
    }
    return true;
}