#include "game.h"
#include <QDebug>
#include <cstdlib>

//change to increate or decrease speed of game.
const float Game::SPEED = 500.0f; //in millisecond
const float Game::SPEED_UP = 100.0f; //in millisecond

const int Game::COUNT_OF_COLORS = 10;
const int Game::COUNT_OF_FIGURES = 7;
const int Game::COUNT_OF_BLOCKS = 4;

QSize Game::BLOCK_SIZE = QSize(18, 18);
QSize Game::RESOLUTION = QSize(320, 480);

Game::Game()
    : BOARD_HEIGHT(20), BOARD_WIDTH(10), m_dx(0), m_rotate(false),
    m_colorNum(1), m_timer(0), m_delay(SPEED), m_state(State::Active), m_score(0), m_gameOver(false)
{
    for (int i=0; i<BOARD_HEIGHT; ++i) {
        for (int j=0; j<BOARD_WIDTH; ++j) {
            m_field[i][j] = 0;
        }
    }
    //set zero for points
    for (int i=0; i<4; ++i) {
        m_a[i].x = 0;
        m_a[i].y = 0;
        m_b[i].x = 0;
        m_b[i].y = 0;
    }

    //I
    m_figures[0][0] = 1;
    m_figures[0][1] = 3;
    m_figures[0][2] = 5;
    m_figures[0][3] = 7;

    //S
    m_figures[1][0] = 2;
    m_figures[1][1] = 4;
    m_figures[1][2] = 5;
    m_figures[1][3] = 7;

    //Z
    m_figures[2][0] = 3;
    m_figures[2][1] = 5;
    m_figures[2][2] = 4;
    m_figures[2][3] = 6;

    //T
    m_figures[3][0] = 3;
    m_figures[3][1] = 5;
    m_figures[3][2] = 4;
    m_figures[3][3] = 7;

    //L
    m_figures[4][0] = 2;
    m_figures[4][1] = 3;
    m_figures[4][2] = 5;
    m_figures[4][3] = 7;

    //J
    m_figures[5][0] = 3;
    m_figures[5][1] = 5;
    m_figures[5][2] = 7;
    m_figures[5][3] = 6;

    //O
    m_figures[6][0] = 2;
    m_figures[6][1] = 3;
    m_figures[6][2] = 4;
    m_figures[6][3] = 5;

    //set init figure
    srand(time(0));
    m_colorNum = (rand() % (Game::COUNT_OF_COLORS - 1)) + 1;
    int n = rand() % Game::COUNT_OF_FIGURES;
    for (int i=0; i<Game::COUNT_OF_BLOCKS; i++) {
        m_a[i].x = (m_figures[n][i] % 2) + BOARD_WIDTH/2 - 1;
        m_a[i].y = m_figures[n][i] / 2;
    }

    //load textures
    if ( !m_background.load(":/images/bg_beach.png") ) {
        qDebug() << "Failed to load: " << ":/images/bg_beach.png";
    }
    if ( !m_frame.load(":/images/frame.png") ) {
        qDebug() << "Failed to load: " << ":/images/frame.png";
    }
    if ( !m_tile.load(":images/tiles.png") ) {
        qDebug() << "Failed to load: " << ":/images/tiles.png";
    }
    if ( !m_pauseBackground.load(":/images/bg_pause.png") ) {
        qDebug() << "Failed to load: " << ":/images/bg_pause.png";
    }
    if ( !m_scorePixmap.load(":/images/score.png") ) {
        qDebug() << "Failed to load: " << ":/images/score.png";
    }
    if ( !m_numbersPixmap.load(":/images/numbers.png") ) {
        qDebug() << "Failed to load: " << ":/images/numbers.png";
    }
    if ( !m_gameOverBackground.load(":/images/bg_gameover.png") ) {
        qDebug() << "Failed to load: " << ":/images/bg_gameover.png";
    }
    if ( !m_restartTextPixmap.load(":/images/restart_text.png") ) {
        qDebug() << "Failed to load: " << ":/images/restart_text.png";
    }
}

bool Game::check()
{
    for (int i=0; i<4; i++) {
        if (m_a[i].x<0 || m_a[i].x>=BOARD_WIDTH || m_a[i].y>=BOARD_HEIGHT) {
            return false;
        }
        else if (m_field[m_a[i].y][m_a[i].x]) {
            return false;
        }
    }
    return true;
}

void Game::addScore(int val)
{
    m_score += val;
}

void Game::reset()
{
    m_score = 0;
    m_gameOver = false;

    for (int i=0; i<BOARD_HEIGHT; ++i) {
        for (int j=0; j<BOARD_WIDTH; ++j) {
            m_field[i][j] = 0;
        }
    }
    //set zero for points
    for (int i=0; i<4; ++i) {
        m_a[i].x = 0;
        m_a[i].y = 0;
        m_b[i].x = 0;
        m_b[i].y = 0;
    }

    //set init figure
    srand(time(0));
    m_colorNum = (rand() % (Game::COUNT_OF_COLORS - 1)) + 1;
    int n = rand() % Game::COUNT_OF_FIGURES;
    for (int i = 0; i < Game::COUNT_OF_BLOCKS; i++) {
        m_a[i].x = (m_figures[n][i] % 2) + BOARD_WIDTH/2-1;
        m_a[i].y = m_figures[n][i] / 2;
    }

    m_timer = 0.0f;
    m_state = State::Active;
}
