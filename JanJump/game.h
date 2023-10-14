#ifndef GAME_H
#define GAME_H

#include <QSize>
#include <QString>

class Game
{
public:
    Game();
    static void init();
    static QSize RESOLUTION;
    QString PATH_TO_BACKGROUND_PIXMAP;
    QString PATH_TO_HERO_PIXMAP;
    QString PATH_TO_PLATFORM_PIXMAP;
    QString PATH_TO_ALL_NUMBERS_PIXMAP;
    QString PATH_TO_PAUSED_BG;
    QString PATH_TO_GAME_OVER_BG;
    static float DELAY;
    static const float JUMP_FORCE;
    //var for better jump
    static const int X_OFFSET;
    static const int Y_OFFSET;
    static const QSize HERO_SIZE;
    //
    static const float JUMP_SPEED;
    static const int DEAD_LEVEL;
    static const QSize PLATFORM_SIZE;
    static const QSize NUMBER_SIZE;
    int POINTS;

    enum class State {
        Active, Paused, Game_Over
    };

    State STATE;
};

#endif // GAME_H
