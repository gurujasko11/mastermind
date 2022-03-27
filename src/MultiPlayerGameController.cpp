#include "MultiPlayerGameController.h"

void MultiPlayerGameController::requestCodeFromPlayer2(CodeArray& code) {
    requestCodeFromHuman(code, "[P2]: enter code");
}

void MultiPlayerGameController::requestGuessFromPlayer2(CodeArray& code) {
    requestCodeFromHuman(code, "[P2]: enter guess");
}
