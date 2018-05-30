#include "mainwindow.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv); //Appli QT
    MainWindow w; //Fenetre principale
    w.show();

    return a.exec();
}
