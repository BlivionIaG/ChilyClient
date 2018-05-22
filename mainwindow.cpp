#include "mainwindow.hpp"

MainWindow::MainWindow(int width, int height, QString title, QWidget *parent)
    : QMainWindow(parent), mainWidget{new QWidget}, serverConnectLayout{new QHBoxLayout}, client{nullptr}
{
    this->setWindowTitle(title);
    this->setMinimumHeight(height);
    this->setMinimumWidth(width);

    serverAddress = new QTextEdit;
    serverAddress->setMaximumHeight(this->height()/24);
    serverAddress->setPlainText("localhost");
    serverConnectLayout->addWidget(serverAddress);

    serverPort = new QSpinBox;
    serverPort->setMaximumWidth(this->width()/8);
    serverPort->setMaximumHeight(this->height()/24);
    serverPort->setMinimum(0);
    serverPort->setMaximum(65535);
    serverPort->setValue(42080);
    serverConnectLayout->addWidget(serverPort);

    serverConnect = new QPushButton("CONNECT");
    serverConnect->setMinimumHeight(this->height()/24);
    serverConnectLayout->addWidget(serverConnect);

    mainWidget->setLayout(serverConnectLayout);
    this->setCentralWidget(mainWidget);

    timer = new QTimer(this);

    connect(serverConnect, SIGNAL(clicked()), this, SLOT(connectToServer()));
}

MainWindow::~MainWindow()
{

}

void MainWindow::connectToServer(){
    client = new QT_GenericClient(serverAddress->toPlainText(), serverPort->value());

    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(refreshDelay);
}

void MainWindow::update(){
    auto tmp = client->receive();

    for(auto &i : tmp){
        qDebug() << QString::fromStdString(i);
    }
}
