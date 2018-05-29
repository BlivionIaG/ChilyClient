#include "scene.hpp"

Scene::Scene(int width, int height, QObject *parent):  QGraphicsScene(parent)
{
    this->setSceneRect(0,0,width, height);
}

void Scene::update(){

}
