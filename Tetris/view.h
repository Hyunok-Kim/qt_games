#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsView>

class GameScene;
class MenuScene;
class SettingsScene;
class QSoundEffect;

class View : public QGraphicsView
{
    Q_OBJECT
public:
    explicit View(QWidget *parent = nullptr);

private:
    void createConnection();

    GameScene *m_gameScene;
    MenuScene *m_menuScene;
    SettingsScene *m_settingsScene;
    QCursor m_cursor;

    QSoundEffect *m_backgroundMusic;

private slots:
    void start();
    void options();
    void quit();
    void menu();

    // QWidget interface
protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
};

#endif // VIEW_H
