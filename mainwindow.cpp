#include "mainwindow.hpp"

MainWindow::MainWindow(int width, int height, QString title, QWidget *parent)
    : QMainWindow(parent), mainWidget{new QWidget}, mainLayout{new QVBoxLayout}, serverConnectLayout{new QHBoxLayout}, simulationActionLayout{new QHBoxLayout}, client{nullptr}, world{nullptr}
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

    simulationStart = new QPushButton("START");
    simulationStart->setMaximumHeight(this->height()/24);
    simulationActionLayout->addWidget(simulationStart);
    simulationStart->hide();

    simulationPause = new QPushButton("PAUSE");
    simulationPause->setMinimumHeight(this->height()/24);
    simulationPause->setCheckable(true);
    simulationActionLayout->addWidget(simulationPause);
    simulationPause->hide();

    mainLayout->addLayout(serverConnectLayout);
    mainLayout->addLayout(simulationActionLayout);
    mainWidget->setLayout(mainLayout);

    this->setCentralWidget(mainWidget);

    timer = new QTimer(this);

    // Action Connexion au Serveur
    connect(serverConnect, SIGNAL(clicked()), this, SLOT(connectToServer()));
    //Action démarrage simulation
    connect(simulationStart, SIGNAL(clicked()), this, SLOT(sendSimulationStart()));
    //Action Toggle Pause
    connect(simulationPause, SIGNAL(toggled(bool)), this, SLOT(sendSimulationPause(bool)));
}

MainWindow::~MainWindow()
{

}

void MainWindow::connectToServer(){
    client = new QT_GenericClient(serverAddress->toPlainText(), serverPort->value());

    simulationStart->show();
    simulationPause->show();

    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(refreshDelay);
}

void MainWindow::sendSimulationStart(){
    if(client != nullptr){
        client->send("SERVER@simulation:1 start");
    }
}

void MainWindow::sendSimulationPause(bool value){
    if(client != nullptr){
        if(value){
            client->send("SERVER@pause:1 on");
        }else{
            client->send("SERVER@pause:1 off");
        }
    }
}

void MainWindow::update(){
    auto tmp = client->receive();

    if(tmp.size() > 0){
        for(auto &i : tmp){
            auto cmd = cmdFormat::parseCommand(i);

            if(!cmd.command.compare("createEnvironnement")){
                if(cmd.args.size() < 2){
                    client->send("SERVER@createEnvironnement:2 error invalid parameters");
                }else if(world == nullptr){
                    world = std::make_shared<SC_Environnement>(std::atoi(cmd.args[0].c_str()), std::atoi(cmd.args[1].c_str()));
                }else{
                    client->send("SERVER@createEnvironnement:2 error world already created");
                }
            }else if(!cmd.command.compare("addAnimal")){
                if(cmd.args.size() < 5){
                    client->send("SERVER@addAnimal:2 error invalid parameters");
                }else if(world!=nullptr){
                    world->addAnimal(cmd.args[0], std::atoi(cmd.args[1].c_str()), std::atoi(cmd.args[2].c_str()),std::atoi(cmd.args[3].c_str()),std::atoi(cmd.args[4].c_str()));
                }
            }else if(!cmd.command.compare("attack")){
                if(cmd.args.size() < 4){
                    client->send("SERVER@attack:2 error invalid parameters");
                }else if(world!=nullptr){
                    world->attack(cmd.args[0], std::atoi(cmd.args[1].c_str()), cmd.args[2], std::atoi(cmd.args[3].c_str()));
                }
            }else if(!cmd.command.compare("move")){
                if(cmd.args.size() < 4){
                    client->send("SERVER@move:2 error invalid parameters");
                }else if(world != nullptr){
                    world->move(cmd.args[0], std::atoi(cmd.args[1].c_str()), cmd.args[2], std::atoi(cmd.args[3].c_str()));
                }
            }else if(!cmd.command.compare("damage")){

            }
        }
    }
}
