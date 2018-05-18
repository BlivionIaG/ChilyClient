#include "mainwindow.hpp"

MainWindow::MainWindow(int width, int height, QString title, QWidget *parent)
    : QMainWindow(parent), mainWidget{new QWidget}, hlayout{new QHBoxLayout}
{
    this->setWindowTitle(title);
    this->setMinimumHeight(height);
    this->setMinimumWidth(width);

    serverAddress = new QTextEdit;
    serverAddress->setMaximumHeight(this->height()/24);
    serverAddress->setPlainText("localhost");
    hlayout->addWidget(serverAddress);

    serverPort = new QSpinBox;
    serverPort->setMaximumWidth(this->width()/8);
    serverPort->setMaximumHeight(this->height()/24);
    serverPort->setMinimum(0);
    serverPort->setMaximum(65535);
    serverPort->setValue(42080);
    hlayout->addWidget(serverPort);

    serverConnect = new QPushButton("CONNECT");
    serverConnect->setMinimumHeight(this->height()/24);
    hlayout->addWidget(serverConnect);

    mainWidget->setLayout(hlayout);
    this->setCentralWidget(mainWidget);

    connect(serverConnect, SIGNAL(clicked()), this, SLOT(connectToServer()));
}

MainWindow::~MainWindow()
{

}

void MainWindow::connectToServer(){

}
