#ifndef SCENE_HPP
#define SCENE_HPP

#include <QGraphicsScene>
#include <QTimer>

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    Scene(int width, int height, QObject *parent = 0);
    ~Scene() noexcept {}
public slots:
    void update();
private:
    QTimer *timer;
};

#endif // SCENE_HPP
