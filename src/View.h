#ifndef VIEW_H
#define VIEW_H

#include "Common.h"

class View {
public:
    virtual void requestCode(CodeArray& code) = 0;
    virtual void info(std::string text) = 0;
    virtual void handleFeedback(FeedbackArray& feedback) = 0;
    virtual void handleRoundStart(GameStatus& status) = 0;
    virtual void handleRoundOver(GameStatus& status, FeedbackArray& feedback) = 0;
    virtual void handleGameEnd(GameStatus& status) = 0;

protected:
    void codeNumberToCode(unsigned int& codeNumber, CodeArray& code);
    bool wasGuessRight(FeedbackArray& feedback);
};

#endif // VIEW_H