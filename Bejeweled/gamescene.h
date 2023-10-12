#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QElapsedTimer>

#include "game.h"

class GameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GameScene(QObject *parent = nullptr);

private slots:
    void loop();
private:
    void loadPixmap();
    void init();
    void draw();
    void mouseClick();
    void matchFinding();
    void movingAnimation();
    void removeAnimation();
    void updateScore();
    void swapBackIfNotMatch();
    void updateGrid();
    void drawScore();
    Game m_game;
    QPixmap m_BgPixmap;
    QPixmap m_GemsPixmap;
    QPixmap m_framePixmap;
    QPixmap m_scorePixmap;
    QPixmap m_numbersPixmap;

    int m_x0, m_y0, m_x, m_y;
    int m_click;
    QPoint m_pos;
    bool m_isSwap, m_isMoving;
    QGraphicsPixmapItem m_pixmapItems[10][10];
    QGraphicsPixmapItem unityPartScoreItem, decimalPartScoreItem, hundredthPartScoreItem;
    int m_tmpScore;
    QElapsedTimer m_elapsedTimer;
    QTimer m_timer;
    float m_deltaTime;
    float m_animationTime;
    const float m_animationSpeed;
protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // GAMESCENE_H
