#include "scene.hpp"

Scene::Scene(int width, int height, int elmSize, QObject *parent):  QGraphicsScene(parent), width{width}, height{height}, elmSize{elmSize}, world{world}
{
    this->setSceneRect(0,0,width, height);

    imgLion = new QPixmap(imgLionPath.c_str());
    imgLion->scaled(elmSize, elmSize, Qt::KeepAspectRatio);
    imgGazelle = new QPixmap(imgGazellePath.c_str());
    imgGazelle->scaled(elmSize, elmSize, Qt::KeepAspectRatio);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(displayRefreshRate);
}

void Scene::update(){

}

void Scene::createEnvironnement(int maxX, int maxY){
    if(world == nullptr){
        world = std::make_shared<SC_Environnement>(maxX, maxY);
    }
}

void Scene::addAnimal(std::string type, int id, int x, int y, int hp){
    if(world != nullptr){
        world->addAnimal(type, id, x,y,hp);

        QPixmap *tmp = nullptr;
        if(!type.compare("Lion")){
            tmp = imgLion->copy(x,y,imgLion->width(), imgLion->height());
        }else if(!type.compare(("Gazelle"))){
            tmp = imgGazelle->copy(x,y,imgGazelle->width(), imgGazelle->height());
        }

        imgData tmpid = {id, tmp};
        images[QString::fromStdString(type)].append(tmpid);
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

        auto image = findImage(images[QString::fromStdString(type)], id);
        auto animal = world->findAnimal(world->getAnimals()[type], id);

        image->image.rect().setX(animal->get()->getX());
        image->image.rect().setY(animal->get()->getY());
    }
}

void Scene::damage(std::string type, int id, int value){
    if(world != nullptr ) {
        world->damage(type, id, value);
    }
}
