#include "level.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "game.h"

Level::Level()
    : m_levelData(new QVector<LevelData>())
{

}

Level::~Level()
{
    delete m_levelData;
}

void Level::loadLevel(const QString &pathFile)
{
    QFile file(pathFile);

    if (file.open(QIODeviceBase::Text | QIODeviceBase::ReadOnly)) {
        qDebug() << "Level is loaded successfully";
    } else {
        qDebug() << "Level is not loaded successfully";
    }
    m_levelData->clear();
    QTextStream stream(&file);
    while (!stream.atEnd()) {
        QString line = stream.readLine();
        if (line.isEmpty() || line.startsWith("//")) {
            continue;
        }
        Q_ASSERT(line.split(";").count() == 3);
        LevelData levelData;
        int colorValue = line.split(";").at(0).toInt();
        if (colorValue < 0) {
            qDebug() << "Warning colorValue should be between " << 0 << " and " << Game::COUNT_OF_KIND_OF_BLOCKS-1 << ". Not " << colorValue;
            colorValue = 0;
        } else if (colorValue > (Game::COUNT_OF_KIND_OF_BLOCKS - 1)) {
            qDebug() << "Warning colorValue should be between " << 0 << " and " << Game::COUNT_OF_KIND_OF_BLOCKS-1 << ". Not " << colorValue;
            colorValue = Game::COUNT_OF_KIND_OF_BLOCKS-1;
        }
        levelData.colorValue = colorValue;

        int x = line.split(";").at(1).toInt();
        int y = line.split(";").at(2).toInt();

        QPoint position = QPoint(x, y);
        levelData.position = position;

        levelData.isDeleted = false;

        m_levelData->push_back(levelData);
    }

    file.close();
}
