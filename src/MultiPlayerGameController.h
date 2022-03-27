#ifndef MULTIPLAYERGAMECONTROLLER_H
#define MULTIPLAYERGAMECONTROLLER_H

#include "GameController.h"

class MultiPlayerGameController : public GameController{
public:
    void requestCodeFromPlayer2(CodeArray& guess) override;
    void requestGuessFromPlayer2(CodeArray& guess) override;
};

#endif // MULTIPLAYERGAMECONTROLLER_H