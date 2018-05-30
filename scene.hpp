#ifndef SCENE_HPP
#define SCENE_HPP

#include <QGraphicsScene>
#include <QTimer>
#include <QMap>
#include <QVector>
#include <QString>

#include <memory>

#include "SC_Environnement.hpp"

struct imgData
{
    int id;
    QPixmap image;
};

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    Scene(int width, int height, int elmSize, QObject *parent = 0);
    ~Scene() noexcept {}



    QVector<imgData>::iterator findImage(QVector<imgData> qv, int id){ return std::find_if(qv.begin(), qv.end(), [id](imgData a) { return a.id == id; });}

    void createEnvironnement(int maxX, int maxY);
    void addAnimal(std::string type, int id, int x, int y, int hp);
    void attack(std::string attackerType, int attackerId, std::string victimType, int victimId);
    void move(std::string type, int id, std::string direction, int pas);
    void damage(std::string type, int id, int value);
public slots:
    void update();
private:
    int width, height, elmSize;

    QTimer *timer;
    QPixmap *imgLion, *imgGazelle;
    QMap<QString,QVector<imgData>> images;

    std::shared_ptr<SC_Environnement> world;

    const std::string imgLionPath = "res/lion.png";
    const std::string imgGazellePath = "res/gazelle.png";

    const int displayRefreshRate = 20;
};

#endif // SCENE_HPP
