#include "TextView.h"
#include "iostream"
#include <limits>

void TextView::requestCode(CodeArray& code) {
    bool codeValid = false;
    do {
        try {
            unsigned int codeNumber;
            std::cin >> codeNumber;
            codeNumberToCode(codeNumber, code);
            codeValid = true;
        }
        catch (const std::runtime_error& error) {
            std::cout << error.what() << std::endl;
            std::cin.clear(); //Clears the error flag on cin. 
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            codeValid = false;
        }
    }
    while (codeValid == false);
}

void TextView::info(std::string text) {
    std::cout << text << std::endl; 
}

void TextView::handleFeedback(FeedbackArray& feedback) {
    std::cout << "Guess gave following feedback:" << std::endl;
    printFeedback(feedback);
}

void TextView::printFeedback(FeedbackArray& feedback) {
    for (auto& feedbackColor : feedback) {
        char feedbackChar;
        switch (feedbackColor) {
            case FEEDBACK_COLOR::BLACK : {
                feedbackChar = '2';
                break;
            }
            case FEEDBACK_COLOR::WHITE : {
                feedbackChar = '1';
                break;
            }
            default : {
                feedbackChar = '0';
                break;
            }
        }
        std::cout << feedbackChar;
    }
    std::cout << std::endl;
}

void TextView::handleRoundStart(GameStatus& status) {
    std::cout << "Starting round " << status.roundCounter + 1<< std::endl;
    char playerChar;
    if (status.codeMaker == GameStatus::PLAYER::PLAYER_1) {
        playerChar = '1';
    }
    else {
        playerChar = '2';
    }
    std::cout << "Player [" << playerChar << "] is code maker in this round" << std::endl;

}

void TextView::printPoints(GameStatus& status) {
    std::cout << "[POINTS] P1: " << status.p1Score <<
        ", P2: " << status.p2Score << std::endl;
}

void TextView::handleRoundOver(GameStatus& status, FeedbackArray& feedback) {
    std::cout << "Round " << status.roundCounter  << " over" << std::endl;
    if (wasGuessRight(feedback)) {
        std::cout << "Code guessed corectly" << std::endl;
    }
    else {
        std::cout << "Code NOT guessed corectly" << std::endl;
    }
    printPoints(status);
}

void TextView::handleGameEnd(GameStatus& status) {
    std::cout << "Game ended, ";
    if (status.p1Score > status.p2Score) {
        std::cout << "Player 1 wins!" << std::endl;
    }
    else if (status.p1Score < status.p2Score) {
        std::cout << "Player 2 wins!" << std::endl;
    }
    else {
        std::cout << "it's draw!" << std::endl;
    }
    printPoints(status);
    std::cout << "Press enter to exit game:" << std::endl;
    std::string dummy;
    std::cin >> dummy;
}