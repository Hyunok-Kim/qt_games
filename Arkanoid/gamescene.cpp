#include "gamescene.h"
#include <QTimer>
#include <QKeyEvent>
#include <QGraphicsPixmapItem>
#include <QDir>
#include <QPainter>

GameScene::GameScene(QObject *parent)
    : QGraphicsScene(parent), m_game(), m_level(), m_timer(new QTimer(this)), m_paddleXpos(300), m_paddleYpos(440),
    m_ballXpos(300), m_ballYpos(428), m_moveRight(false), m_moveLeft(false)
{
    loadPixmap();
    loadLevel();

    setSceneRect(0, 0, m_game.RESOLUTION.width(), m_game.RESOLUTION.height());
    connect(m_timer, &QTimer::timeout, [this](){
        update();
    });
    m_timer->start(m_game.ITERATION_VALUE);
}

void GameScene::update()
{
    clear();
    m_blockItems.clear();
    m_blockItems.resize(m_level.m_levelData->size());
    QGraphicsPixmapItem *bgItem = new QGraphicsPixmapItem(m_bgPixmap.scaled(m_game.RESOLUTION.width(), m_game.RESOLUTION.height()));
    addItem(bgItem);

    QGraphicsPixmapItem *paddleItem = new QGraphicsPixmapItem(m_paddlePixmap);
    addItem(paddleItem);
    paddleItem->setPos(m_paddleXpos, m_paddleYpos);

    QGraphicsPixmapItem *ballItem = new QGraphicsPixmapItem(m_ballPixmap);
    addItem(ballItem);

    for (int i=0; i<m_level.m_levelData->size(); ++i) {
        QGraphicsPixmapItem *block = new QGraphicsPixmapItem(m_blocksPixmap[m_level.m_levelData->at(i).colorValue]);
        block->setPos(m_level.m_levelData->at(i).position.x(), m_level.m_levelData->at(i).position.y());
        m_blockItems[i] = block;
        addItem(block);
    }

    if (m_moveLeft) {
        m_paddleXpos -= m_game.PADDLE_SPEED;
    } else if (m_moveRight) {
        m_paddleXpos += m_game.PADDLE_SPEED;
    }
    clampPaddle();

    if (!m_game.m_isBallStucked && m_game.m_state == Game::State::Active) {
        m_ballXpos += m_game.m_deltaX;
        if (m_ballXpos < 0 || m_ballXpos > m_game.RESOLUTION.width() - m_ballPixmap.width()) {
            m_game.m_deltaX = -m_game.m_deltaX;
        }
        for (int idx=0; idx<m_level.m_levelData->count(); ++idx) {
            if (!m_blockItems[idx]) {
                continue;
            }
            if (QRectF(m_ballXpos, m_ballYpos, m_ballPixmap.width(), m_ballPixmap.height()).intersects(
                    QRectF(m_blockItems[idx]->pos().x(), m_blockItems[idx]->pos().y(), m_blockItems[idx]->boundingRect().width(), m_blockItems[idx]->boundingRect().height()))) {
                m_game.m_deltaX = -m_game.m_deltaX;
                m_game.m_score += 2;
                m_blockItems[idx]->hide();
                LevelData data = m_level.m_levelData->at(idx);
                data.isDeleted = true;
                m_level.m_levelData->removeAt(idx);
                m_level.m_levelData->insert(idx, data);
            }
        }

        m_ballYpos += m_game.m_deltaY;
        if (m_ballYpos < 0) {
            m_game.m_deltaY = -m_game.m_deltaY;
        } else if (m_ballYpos > Game::DEATH_Y_LEVEL) {
            m_game.m_state = Game::State::GameOver;
        }

        m_blockItems.resize(m_level.m_levelData->count());
        for (int idx=0; idx<m_level.m_levelData->count(); ++idx) {
            if (!m_blockItems[idx]) {
                continue;
            }
            if (QRectF(m_ballXpos, m_ballYpos, m_ballPixmap.width(), m_ballPixmap.height()).intersects(
                    QRectF(m_blockItems[idx]->pos().x(), m_blockItems[idx]->pos().y(), m_blockItems[idx]->boundingRect().width(), m_blockItems[idx]->boundingRect().height()))) {
                m_game.m_deltaY = -m_game.m_deltaY;
                m_game.m_score += 2;
                m_blockItems[idx]->hide();
                LevelData data = m_level.m_levelData->at(idx);
                data.isDeleted = true;
                m_level.m_levelData->removeAt(idx);
                m_level.m_levelData->insert(idx, data);
            }
        }

        for (int idx=0; idx<m_level.m_levelData->size(); ++idx) {
            if (m_level.m_levelData->at(idx).isDeleted)
                m_level.m_levelData->removeAt(idx);
        }
        checkVictory();

        if (m_ballXpos >= m_paddleXpos &&
            m_ballXpos + m_ballPixmap.width() <= m_paddleXpos + m_paddlePixmap.width() &&
            m_ballYpos + m_ballPixmap.height() >= m_paddleYpos &&
            m_paddleYpos + m_paddlePixmap.height()/2 >= m_ballYpos ) {
            m_ballYpos = paddleItem->y() - m_ballPixmap.height();
            m_game.m_deltaY = -(rand()%5 + 2);
        }
    }
    ballItem->setPos(m_ballXpos, m_ballYpos);

    drawScore();
    if (m_game.m_state == Game::State::GameOver) {
        QGraphicsPixmapItem *goBgItem = new QGraphicsPixmapItem(m_game_over_bgPixmap.scaled(m_game.RESOLUTION.width(), m_game.RESOLUTION.height()));
        addItem(goBgItem);
    } else if (m_game.m_state == Game::State::Pause) {
        QGraphicsPixmapItem *pauseBgItem = new QGraphicsPixmapItem(m_pause_bgPixmap.scaled(m_game.RESOLUTION.width(), m_game.RESOLUTION.height()));
        addItem(pauseBgItem);
    } else if (m_game.m_state == Game::State::Win) {
        QGraphicsPixmapItem *victoryBgItem = new QGraphicsPixmapItem(m_victoryPixmap.scaled(m_game.RESOLUTION.width(), m_game.RESOLUTION.height()));
        addItem(victoryBgItem);
    }
}

void GameScene::loadPixmap()
{
    if (m_paddlePixmap.load(m_game.PATH_TO_PADDLE_PIXMAP)) {
        qDebug() << "PaddlePixmap is loaded successfully";
    } else {
        qDebug() << "PaddlePixmap is not loaded successfully";
    }
    for (size_t i=0; i<m_game.COUNT_OF_KIND_OF_BLOCKS; ++i) {
        if (m_blocksPixmap[i].load(m_game.PATH_TO_BLOCKS_PIXMAP[i])) {
            qDebug() << "BlockPixmap["<< i <<"] is loaded successfully";
        } else {
            qDebug() << "BlockPixmap["<< i <<"] is not loaded successfully";
        }
    }
    if (m_bgPixmap.load(m_game.PATH_TO_BACKGROUND_PIXMAP)) {
        qDebug() << "BgPixmap is loaded successfully";
    } else {
        qDebug() << "BgPixmap is not loaded successfully";
    }
    if (m_ballPixmap.load(m_game.PATH_TO_BALL_PIXMAP)) {
        qDebug() << "BallPixmap is loaded successfully";
    } else {
        qDebug() << "BallPixmap is not loaded successfully";
    }
    if (m_game_over_bgPixmap.load(m_game.PATH_TO_GAME_OVER_BG_PIXMAP)) {
        qDebug() << "GameOverBgPixmap is loaded successfully";
    } else {
        qDebug() << "GameOverBgPixmap is not loaded successfully";
    }
    if (m_pause_bgPixmap.load(m_game.PATH_TO_PAUSE_BG_PIXMAP)) {
        qDebug() << "PauseBgPixmap is loaded successfully";
    } else {
        qDebug() << "PauseBgPixmap is not loaded successfully";
    }
    if (m_victoryPixmap.load(m_game.PATH_TO_VICTORY_BG_PIXMAP)) {
        qDebug() << "VictoryBgPixmap is loaded successfully";
    } else {
        qDebug() << "VictoryBgPixmap is not loaded successfully";
    }
    if (m_allNumbersPixmap.load(m_game.PATH_TO_ALL_NUMBERS_PIXMAP)) {
        qDebug() << "AllNumbersPixmap is loaded successfully";
    } else {
        qDebug() << "AllNumbersPixmap is not loaded successfully";
    }
}

void GameScene::loadLevel(const QString &pathFile)
{
    m_level.loadLevel(pathFile);
    m_blockItems.clear();
}

void GameScene::clampPaddle()
{
    if (m_paddleXpos < 0) {
        m_paddleXpos = 0;
    } else if (m_paddleXpos + m_paddlePixmap.width() > m_game.RESOLUTION.width()) {
        m_paddleXpos = m_game.RESOLUTION.width() - m_paddlePixmap.width();
    }
}

void GameScene::stuckBall()
{
    if (m_game.m_isBallStucked)  {
        m_ballXpos = m_paddleXpos + m_paddlePixmap.width()/2 - m_ballPixmap.width()/2;
    }
}

void GameScene::checkVictory()
{
    if (m_level.m_levelData->isEmpty()) {
        m_game.m_state = Game::State::Win;
    }
}

void GameScene::drawScore()
{
    QString scoreText = QString::number(m_game.m_score);
    int unityPartVal = 0;
    int decimalPartValue = 0;
    int hundredthPartValue = 0;

    if (scoreText.length() == 1) {
        unityPartVal = scoreText.toInt();
        decimalPartValue = 0;
        hundredthPartValue = 0;
    } else if (scoreText.length() == 2) {
        unityPartVal = scoreText.last(1).toInt();
        decimalPartValue = scoreText.first(1).toInt();
        hundredthPartValue = 0;
    } else if (scoreText.length() == 3) {
        unityPartVal = scoreText.last(1).toInt();
        hundredthPartValue = scoreText.first(1).toInt();
        QString copyVal = scoreText;
        copyVal.chop(1);
        decimalPartValue = copyVal.last(1).toInt();
    }

    QGraphicsPixmapItem *unityPartScoreItem = new QGraphicsPixmapItem(m_allNumbersPixmap.copy(unityPartVal * Game::NUMBER_SIZE.width(), 0, Game::NUMBER_SIZE.width(), Game::NUMBER_SIZE.width()));
    unityPartScoreItem->moveBy(m_game.RESOLUTION.width() - Game::NUMBER_SIZE.width(), 0);
    addItem(unityPartScoreItem);

    QGraphicsPixmapItem *decimalPartScoreItem = new QGraphicsPixmapItem(m_allNumbersPixmap.copy(decimalPartValue * Game::NUMBER_SIZE.width(), 0, Game::NUMBER_SIZE.width(), Game::NUMBER_SIZE.width()));
    decimalPartScoreItem->moveBy(m_game.RESOLUTION.width() - 2 * Game::NUMBER_SIZE.width(), 0);
    addItem(decimalPartScoreItem);

    QGraphicsPixmapItem *hundredthPartScoreItem = new QGraphicsPixmapItem(m_allNumbersPixmap.copy(hundredthPartValue * Game::NUMBER_SIZE.width(), 0, Game::NUMBER_SIZE.width(), Game::NUMBER_SIZE.width()));
    hundredthPartScoreItem->moveBy(m_game.RESOLUTION.width() - 3 * Game::NUMBER_SIZE.width(), 0);
    addItem(hundredthPartScoreItem);
}

void GameScene::reset()
{
    m_paddleXpos = 300;
    m_paddleYpos = 440;
    m_moveRight = false;
    m_moveLeft = false;
    m_ballXpos = 300;
    m_ballYpos = 428;

    m_game.m_score = 0;
    m_game.m_state = Game::State::Active;
    m_game.m_isBallStucked = true;
    m_game.m_deltaX = 5;
    m_game.m_deltaY = -6;

    loadLevel();
}

void GameScene::renderScene()
{
    static int index = 0;
    QString fileName = QDir::currentPath() + QDir::separator() + "screen" + QString::number(index++) + ".png";
    QRect rect = sceneRect().toAlignedRect();
    QImage image(rect.size(), QImage::Format_ARGB32);
    image.fill(Qt::transparent);
    QPainter painter(&image);
    render(&painter);
    image.save(fileName);
    qDebug() << "saved " << fileName;
}

void GameScene::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Left:
    case Qt::Key_A:
    {
        if (m_game.m_state == Game::State::Active) {
            m_moveLeft = true;
        }
    }
    break;
    case Qt::Key_Right:
    case Qt::Key_D:
    {
        if (m_game.m_state == Game::State::Active) {
            m_moveRight = true;
        }
    }
    break;
    case Qt::Key_Space:
    {
        if (m_game.m_state == Game::State::Active) {
            if (m_game.m_isBallStucked) {
                m_game.m_isBallStucked = false;
            }
        }
    }
    break;
    case Qt::Key_P:
    {
        if (m_game.m_state == Game::State::Active) {
            m_game.m_state = Game::State::Pause;
        } else if (m_game.m_state == Game::State::Pause) {
            m_game.m_state = Game::State::Active;
        }
    }
    break;
    case Qt::Key_R:
    {
        if (m_game.m_state == Game::State::GameOver || m_game.m_state == Game::State::Win) {
            reset();
        }
    }
    break;
    case Qt::Key_Z:
    {
        renderScene();
    }
    break;
    }
    QGraphicsScene::keyPressEvent(event);
}

void GameScene::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Left:
    case Qt::Key_A:
    {
        m_moveLeft = false;
    }
    break;
    case Qt::Key_Right:
    case Qt::Key_D:
    {
        m_moveRight = false;
    }
    }
    QGraphicsScene::keyReleaseEvent(event);
}
