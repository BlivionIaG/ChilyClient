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
#include <QSlider>

#include <string>

#include "qt_genericclient.hpp"
#include "scene.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     *brief	Constructeur de la fenetre principale
     *\param	width   largeur de la fenetre
     *\param	height	hauteur de la fenetre
     *\param    title   titre de la fenetre
     */
    MainWindow(int width = 800, int height = 600, QString title = "Chily Con Carne Client", QWidget *parent = 0);
    ~MainWindow();

public slots:

    void toggleFS(bool value);
    void connectToServer();
    void sendSimulationStart();
    void sendSimulationPause(bool value);
    void update();
    void quit();
    void speedChanged();

private:
    QWidget *mainWidget;
    QVBoxLayout *mainLayout;
    QHBoxLayout *serverConnectLayout;
    QHBoxLayout *simulationActionLayout;
    QHBoxLayout *simConfigLayout;
    QHBoxLayout *simLayout;

    QTextEdit *serverAddress;
    QSpinBox *serverPort;
    QPushButton *serverConnect;

    QPushButton *fullScrean;
    QPushButton *simulationStart;
    QPushButton *simulationPause;
    QPushButton *quitButton;

    QSpinBox *simW;
    QSpinBox *simH;
    QSpinBox *nbLions;
    QSpinBox *nbGazelles;
    QSlider *speed;

    QTimer *timer;
    QT_GenericClient *client;

    Scene *scene;
    QGraphicsView *sceneView;

    const int refreshDelay = 20;
};

#endif // MAINWINDOW_HPP
