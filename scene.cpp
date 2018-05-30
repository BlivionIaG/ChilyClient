#include "scene.hpp"

Scene::Scene(int width, int height, QObject *parent):  QGraphicsScene(parent), width{width}, height{height}, world{nullptr}
{
    elmSize = 0;
    QPixmap tmp;
    tmp.load(QString::fromStdString(imgBG));

    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(tmp.scaled(width, height));
    item->setPos(0,0);
    this->addItem(item);

    QMediaPlayer *player = new QMediaPlayer;
    player->setMedia(QUrl::fromLocalFile(QFileInfo("./res/a.mp3").absoluteFilePath()));
    player->setVolume(20);
    player->play();
}

void Scene::createEnvironnement(int maxX, int maxY){
    world = std::make_shared<SC_Environnement>(maxX, maxY);
    elmSize = width / maxX;
}

void Scene::addAnimal(std::string type, int id, int x, int y, int hp){
    if(world != nullptr){
        world->addAnimal(type, id, x,y,hp);

        if(!type.compare("Lion")){
            addLionImg(id, x, y);
        }else if(!type.compare(("Gazelle"))){
            addGazelleImg(id, x, y);
        }
    }
}

void Scene::attack(std::string attackerType, int attackerId, std::string victimType, int victimId){
    if(world != nullptr) {
        world->attack(attackerType, attackerId, victimType, victimId);
    }
}

void Scene::move(std::string type, int id, std::string direction, int pas){
    if(world != nullptr) {
        world->move(type, id, direction, pas);


        auto x = images[id]->x();
        auto y = images[id]->y();

        if(!direction.compare("LEFT")){
            images[id]->setPos(x-pas*elmSize,y);
        }else if(!direction.compare("RIGHT")){
            images[id]->setPos(x+pas*elmSize,y);
        }else  if(!direction.compare("UP")){
            images[id]->setPos(x,y-pas*elmSize);
        }else if(!direction.compare("DOWN")){
            images[id]->setPos(x,y+pas*elmSize);
        }
    }
}

void Scene::damage(std::string type, int id, int value){
    if(world != nullptr ) {
        world->damage(type, id, value);
    }
}

void Scene::addLionImg(int id, int x, int y){
    QPixmap tmp;
    tmp.load(QString::fromStdString(imgLionPath));

    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(tmp.scaled(elmSize, elmSize));
    item->setPos(x*elmSize,y*elmSize);
    this->addItem(item);

    images.insert(id, item);
}
void Scene::addGazelleImg(int id, int x, int y){
    QPixmap tmp;
    tmp.load(QString::fromStdString(imgGazellePath));

    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(tmp.scaled(elmSize, elmSize));
    item->setPos(x*elmSize,y*elmSize);
    this->addItem(item);

    images.insert(id, item);
}
