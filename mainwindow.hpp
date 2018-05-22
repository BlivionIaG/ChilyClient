#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QWidget>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QTimer>

#include "qt_genericclient.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(int width = 800, int height = 600, QString title = "Chily Con Carne Client", QWidget *parent = 0);
    ~MainWindow();
public slots:
    void connectToServer();
    void update();

private:
    QWidget *mainWidget;
    QHBoxLayout *serverConnectLayout;

    QTextEdit *serverAddress;
    QSpinBox *serverPort;
    QPushButton *serverConnect;

    QTimer *timer;
    QT_GenericClient *client;

    const int refreshDelay = 20;
};

#endif // MAINWINDOW_HPP
