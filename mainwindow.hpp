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
#include <QPixmap>

#include <memory>

#include "qt_genericclient.hpp"
#include "SC_Environnement.hpp"

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

    std::shared_ptr<SC_Environnement> world;

    QPixmap *imgLion;
    QPixmap *imgGazelle;

    const int refreshDelay = 20;
    const std::string imgLionPath = "res/lion.png";
    const std::string imgGazellePath = "res/gazelle.png";
};

#endif // MAINWINDOW_HPP
