#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QTimer>
#include <QGraphicsView>

#include "qt_genericclient.hpp"
#include "scene.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(int width = 800, int height = 600, QString title = "Chily Con Carne Client", QWidget *parent = 0);
    ~MainWindow();

    void initScene();
public slots:
    void connectToServer();
    void sendSimulationStart();
    void sendSimulationPause(bool value);
    void update();

private:
    QWidget *mainWidget;
    QVBoxLayout *mainLayout;
    QHBoxLayout *serverConnectLayout;
    QHBoxLayout *simulationActionLayout;

    QTextEdit *serverAddress;
    QSpinBox *serverPort;
    QPushButton *serverConnect;

    QPushButton *simulationStart;
    QPushButton *simulationPause;

    QTimer *timer;
    QT_GenericClient *client;

    Scene *scene;
    QGraphicsView *sceneView;

    const int refreshDelay = 20;
};

#endif // MAINWINDOW_HPP
