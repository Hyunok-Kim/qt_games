#include "gamescene.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QPainter>
#include <QDir>

GameScene::GameScene(QObject *parent)
    : QGraphicsScene(parent), m_game(), m_click(0), m_isSwap(false), m_isMoving(false), m_tmpScore(0)
    , m_deltaTime(0.f), m_animationTime(0.f), m_animationSpeed(20.f)
{
    loadPixmap();
    setSceneRect(0, 0, Game::RESOLUTION.width(), Game::RESOLUTION.height());
    setBackgroundBrush(QBrush(QColor(153, 255, 51)));
    init();
    draw();
    connect(&m_timer, &QTimer::timeout, this, &GameScene::loop);
    m_timer.start(Game::ITERATION_VALUE);
    m_elapsedTimer.start();
}

void GameScene::loop()
{
    m_deltaTime = m_elapsedTimer.elapsed();
    m_elapsedTimer.restart();

    m_animationTime += m_deltaTime;

    if (m_animationTime > m_animationSpeed) {
        m_animationTime -= m_animationSpeed;
        mouseClick();
        matchFinding();
        movingAnimation();
        removeAnimation();
        updateScore();
        swapBackIfNotMatch();
        updateGrid();
        draw();
        drawScore();
    }
}

void GameScene::loadPixmap()
{
    if (m_BgPixmap.load(Game::PATH_TO_BG_PIXMAP)) {
        qDebug() << "BgPixmap is loaded successfully";
    } else {
        qDebug() << "BgPixmap is not loaded successfully";
    }

    if (m_GemsPixmap.load(Game::PATH_TO_GEMS_PIXMAP)) {
        qDebug() << "GemsPixmap is loaded successfully";
    } else {
        qDebug() << "GemsPixmap is not loaded successfully";
    }

    if (m_framePixmap.load(Game::PATH_TO_FRAME_PIXMAP)) {
        qDebug() << "FramePixmap is loaded successfully";
    } else {
        qDebug() << "FramePixmap is not loaded successfully";
    }

    if (m_scorePixmap.load(Game::PATH_TO_SCORE_PIXMAP)) {
        qDebug() << "ScorePixmap is loaded successfully";
    } else {
        qDebug() << "ScorePixmap is not loaded successfully";
    }

    if (m_numbersPixmap.load(Game::PATH_TO_NUMBERS_PIXMAP)) {
        qDebug() << "NumbersPixmap is loaded successfully";
    } else {
        qDebug() << "NumbersPixmap is not loaded successfully";
    }
}

void GameScene::init()
{
    clear();
    QGraphicsPixmapItem *bgItem = new QGraphicsPixmapItem(m_BgPixmap.scaled(Game::RESOLUTION.width(), Game::RESOLUTION.height()));
    addItem(bgItem);

    QGraphicsPixmapItem *frameItem = new QGraphicsPixmapItem(m_framePixmap);
    frameItem->setPos(Game::OFFSET.x() - 2, Game::OFFSET.y());
    addItem(frameItem);

    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            addItem(&m_pixmapItems[i][j]);
        }
    }

    QGraphicsPixmapItem *scoreItem = new QGraphicsPixmapItem(m_scorePixmap);
    scoreItem->setPos(Game::RESOLUTION.width() - scoreItem->boundingRect().width() - 85, 100);

    unityPartScoreItem.moveBy(Game::RESOLUTION.width()-32, 100);
    addItem(&unityPartScoreItem);

    decimalPartScoreItem.moveBy(Game::RESOLUTION.width()-2*32, 100);
    addItem(&decimalPartScoreItem);

    hundredthPartScoreItem.moveBy(Game::RESOLUTION.width()-3*32, 100);
    addItem(&hundredthPartScoreItem);
}

void GameScene::draw()
{
    for (int i=1; i<=8; i++) {
        for (int j=1; j<=8; j++) {
            Piece p = m_game.m_grid[i][j];
            QImage image = m_GemsPixmap.copy(p.kind*49, 0, 49, 49).toImage().convertToFormat(QImage::Format_ARGB32);
            m_pixmapItems[i][j].setPixmap(QPixmap::fromImage(image));
            m_pixmapItems[i][j].setPos(p.x, p.y);
            m_pixmapItems[i][j].moveBy(Game::OFFSET.x() - Game::TILE_SIZE, Game::OFFSET.y() - Game::TILE_SIZE);
        }
    }
}

void GameScene::mouseClick()
{
    if (m_click == 1) {
        m_x0 = m_pos.x() / Game::TILE_SIZE+1;
        m_y0 = m_pos.y() / Game::TILE_SIZE+1;
    }
    if (m_click == 2) {
        m_x = m_pos.x() / Game::TILE_SIZE+1;
        m_y = m_pos.y() / Game::TILE_SIZE+1;
        if (abs(m_x-m_x0)+abs(m_y-m_y0)==1) {
            m_game.swap(m_game.m_grid[m_y0][m_x0], m_game.m_grid[m_y][m_x]);
            m_isSwap = true;
            m_click = 0;
        } else {
            m_click = 1;
        }
    }
}

void GameScene::matchFinding()
{
    for (int i=1; i<=8; i++) {
        for (int j=1; j<=8; j++) {
            if (m_game.m_grid[i][j].kind == m_game.m_grid[i+1][j].kind) {
                if (m_game.m_grid[i][j].kind == m_game.m_grid[i-1][j].kind) {
                    for (int n=-1; n<=1; n++)  {
                        m_game.m_grid[i+n][j].match += 1;
                    }
                }
            }

            if (m_game.m_grid[i][j].kind == m_game.m_grid[i][j+1].kind) {
                if (m_game.m_grid[i][j].kind == m_game.m_grid[i][j-1].kind) {
                    for (int n=-1; n<=1; n++) {
                        m_game.m_grid[i][j+n].match += 1;
                    }
                }
            }
        }
    }
}

void GameScene::movingAnimation()
{
    m_isMoving = false;
    for (int i=1; i<=8; i++) {
        for (int j=1; j<=8; j++) {
            Piece &p = m_game.m_grid[i][j];
            int dx,dy;
            for (int n=0; n<4; n++) {
                dx = p.x - p.col * Game::TILE_SIZE;
                dy = p.y - p.row * Game::TILE_SIZE;
                if (dx) {
                    p.x -= dx/abs(dx);
                }
                if (dy) {
                    p.y -= dy/abs(dy);
                }
            }
            if (dx||dy) {
                m_isMoving = true;
            }
        }
    }
}

void GameScene::removeAnimation()
{
    if (!m_isMoving) {
        for (int i=1; i<=8; i++) {
            for (int j=1; j<=8; j++) {
                if (m_game.m_grid[i][j].match) {
                    if (m_game.m_grid[i][j].alpha>10) {
                        m_game.m_grid[i][j].alpha -= 10;
                        m_isMoving = true;
                    }
                }
            }
        }
    }
}

void GameScene::updateScore()
{
    m_tmpScore = 0;
    for (int i=1; i<=8; i++) {
        for (int j=1; j<=8; j++) {
            m_tmpScore += m_game.m_grid[i][j].match;
        }
    }
}

void GameScene::swapBackIfNotMatch()
{
    if (m_isSwap && !m_isMoving) {
        if (!m_tmpScore) {
            m_game.swap(m_game.m_grid[m_y0][m_x0], m_game.m_grid[m_y][m_x]);
        }
        m_isSwap = false;
    }
}

void GameScene::updateGrid()
{
    if (!m_isMoving) {
        for (int i=8; i>0; i--) {
            for (int j=1; j<=8; j++) {
                if (m_game.m_grid[i][j].match) {
                    for (int n=i; n>0; n--) {
                        if (!m_game.m_grid[n][j].match) {
                            m_game.swap(m_game.m_grid[n][j], m_game.m_grid[i][j]);
                            m_game.m_score++;
                            break;
                        }
                    }
                }
            }
        }

        for (int j=1; j<=8; j++) {
            for (int i=8,n=0; i>0; i--) {
                if (m_game.m_grid[i][j].match) {
                    m_game.m_grid[i][j].kind = rand()%7;
                    m_game.m_grid[i][j].y = -Game::TILE_SIZE*n++;
                    m_game.m_grid[i][j].match = 0;
                    m_game.m_grid[i][j].alpha = 255;
                }
            }
        }
    }
}

void GameScene::drawScore()
{
    QString scoreText = QString::number(m_game.m_score);
    int unityPartVal = 0;
    int decimalPartValue = 0;
    int hendredthPartValue = 0;

    if (scoreText.length() == 1) {
        unityPartVal = scoreText.toInt();
        decimalPartValue = 0;
        hendredthPartValue = 0;
    } else if (scoreText.length() == 2) {
        unityPartVal = scoreText.last(1).toInt();
        decimalPartValue = scoreText.first(1).toInt();
        hendredthPartValue = 0;
    } else if (scoreText.length() == 3) {
        unityPartVal = scoreText.last(1).toInt();
        hendredthPartValue = scoreText.first(1).toInt();
        QString copyVal = scoreText;
        copyVal.chop(1);
        decimalPartValue = copyVal.last(1).toInt();
    }

    unityPartScoreItem.setPixmap(m_numbersPixmap.copy(unityPartVal*32, 0, 32, 32));
    decimalPartScoreItem.setPixmap(m_numbersPixmap.copy(decimalPartValue*32, 0, 32, 32));
    hundredthPartScoreItem.setPixmap(m_numbersPixmap.copy(hendredthPartValue*32, 0, 32, 32));
}

void GameScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (!m_isSwap && !m_isMoving) {
            m_click++;
        }
        m_pos = QPoint(event->scenePos().x(), event->scenePos().y()) - Game::OFFSET;
    }
}
