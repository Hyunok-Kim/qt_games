#ifndef GAME_H
#define GAME_H

#include <QSize>
#include <QString>

class Game
{
public:
    Game();
    const QSize RESOLUTION;
    const QString PATH_TO_BACKGROUND_PIXMAP;
    const QString PATH_TO_PADDLE_PIXMAP;
    const QString PATH_TO_BALL_PIXMAP;
    const QString PATH_TO_BLOCKS_PIXMAP[5];
    const QString PATH_TO_GAME_OVER_BG_PIXMAP;
    const QString PATH_TO_PAUSE_BG_PIXMAP;
    const QString PATH_TO_VICTORY_BG_PIXMAP;
    const QString PATH_TO_ALL_NUMBERS_PIXMAP;
    static const size_t COUNT_OF_KIND_OF_BLOCKS;
    static const int DEATH_Y_LEVEL;
    static const QSize NUMBER_SIZE;
    const float ITERATION_VALUE;
    const int PADDLE_SPEED;

    bool m_isBallStucked;
    int m_deltaX;
    int m_deltaY;

    enum class State {
        Active, Pause, GameOver, Win
    };

    State m_state;
    int m_score;
};

#endif // GAME_H
