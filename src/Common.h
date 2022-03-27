#ifndef COMMON_H
#define COMMON_H

#include <array>

static const unsigned int CODE_LENGTH = 4;
static const unsigned int MIN_COLOR_NUMBER = 2;
static const unsigned int MAX_COLOR_NUMBER = 9;
static const unsigned int DEFAULT_ROUND_COUNT = 12;
static const unsigned int DEFAULT_COLOR_COUNT = 6;
static const unsigned int DEFAULT_GUESS_COUNT = 8;
static const bool DEFAULT_BLANK_ALLOWED = true;
static const bool DEFAULT_P1_STARTS = true;

enum class CODE_COLOR {
    BLANK = 0,
    WHITE,
    BLACK,
    RED,
    GREEN,
    BLUE,
    YELLOW,
    BROWN,
    ORANGE,
    PINK
};

enum class FEEDBACK_COLOR {
    BLANK = 0,
    WHITE,
    BLACK
};

typedef std::array<CODE_COLOR, CODE_LENGTH> CodeArray;
typedef std::array<FEEDBACK_COLOR, CODE_LENGTH> FeedbackArray;

class GameStatus {
public:
    unsigned int p1Score = 0;
    unsigned int p2Score = 0;
    unsigned int roundCounter = 0;
    unsigned int guessCounter = 0;
    CodeArray code{CODE_COLOR(0),CODE_COLOR(0),CODE_COLOR(0),CODE_COLOR(0)};
    enum GAME_STATE {
        ROUND_STARTING = 0,
        ROUND_ONGOING,
        FINISHED
    } state = ROUND_STARTING;
    enum PLAYER {
        PLAYER_1 = 0,
        PLAYER_2
    } codeMaker;
};

#endif // COMMON_H