#ifndef SCENE_HPP
#define SCENE_HPP

#include <QGraphicsScene>
#include <QTimer>
#include <QMap>
#include <QFileInfo>
#include <QVector>
#include <QDebug>
#include <QString>
#include <QGraphicsPixmapItem>
#include <QMediaPlayer>

#include <memory>

#include "SC_Environnement.hpp"

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    Scene(int width, int height, QObject *parent = 0);
    ~Scene() noexcept {}

    void createEnvironnement(int maxX, int maxY);
    void addAnimal(std::string type, int id, int x, int y, int hp);
    void attack(std::string attackerType, int attackerId, std::string victimType, int victimId);
    void move(std::string type, int id, std::string direction, int pas);
    void damage(std::string type, int id, int value);

    void addLionImg(int id, int x, int y);
    void addGazelleImg(int id, int x, int y);
public slots:
private:
    int width, height, elmSize;

    QTimer *timer;
    QMap<int,QGraphicsPixmapItem*> images;
    QGraphicsPixmapItem *bg;

    std::shared_ptr<SC_Environnement> world;

    const std::string imgBG = "./res/bg.jpg";
    const std::string imgLionPath = "./res/lion.png";
    const std::string imgGazellePath = "./res/gazelle.png";
    const std::string musbgPath = "./res/a.mp3";

    const int displayRefreshRate = 20;
};

#endif // SCENE_HPP
