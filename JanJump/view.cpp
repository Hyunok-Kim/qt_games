#include "view.h"
#include "gamescene.h"
#include <QApplication>
#include <QKeyEvent>

View::View()
    : QGraphicsView(), m_gameScene(new GameScene(this))
{
    setScene(m_gameScene);
    resize(Game::RESOLUTION.width()+2, Game::RESOLUTION.height()+2);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void View::keyPressEvent(QKeyEvent *event)
{
    if (!event->isAutoRepeat()) {
        switch (event->key()) {
        case Qt::Key_Escape:
            QApplication::instance()->quit();
            break;
        }
    }
    QGraphicsView::keyPressEvent(event);
}
