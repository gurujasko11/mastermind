#ifndef TEXTVIEW_H
#define TEXTVIEW_H

#include "View.h"

class TextView : public View {
public:
    void requestCode(CodeArray& code) override;
    void info(std::string text) override;
    void handleFeedback(FeedbackArray& feedback) override;
    void handleRoundStart(GameStatus& status) override;
    void handleRoundOver(GameStatus& status, FeedbackArray& feedback) override;
    void handleGameEnd(GameStatus& status) override;
private:
    void printFeedback(FeedbackArray& feedback);
    void printPoints(GameStatus& status);
};

#endif // TEXTVIEW_H