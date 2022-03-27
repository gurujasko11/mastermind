#include "View.h"
#include <cmath>

void View::codeNumberToCode(unsigned int& codeNumber, CodeArray& code)
{
    unsigned int digitCount = int(log10(codeNumber) + 1);
    if (digitCount != CODE_LENGTH) {
        throw std::runtime_error("Invalid input");
    }
    for (unsigned int i=CODE_LENGTH; i>0; i--) {
        code.at(i-1) = CODE_COLOR(codeNumber % 10);
        codeNumber /= 10;
    }
}

bool View::wasGuessRight(FeedbackArray& feedback) {
    for (auto& feedbackColor : feedback) {
        if (feedbackColor != FEEDBACK_COLOR::BLACK) {
            return false;
        }
    }
    return true;
}