#include "mainwindow.hpp"

MainWindow::MainWindow(int width, int height, QString title, QWidget *parent)
    : QMainWindow(parent), mainWidget{new QWidget}, mainLayout{new QVBoxLayout}, serverConnectLayout{new QHBoxLayout}, simulationActionLayout{new QHBoxLayout}, simConfigLayout{new QHBoxLayout},simLayout{new QHBoxLayout}, client{nullptr}, scene{nullptr}, sceneView{nullptr}
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

    fullScrean = new QPushButton("FULLSCREEN");
    fullScrean->setMinimumHeight(this->height()/24);
    fullScrean->setCheckable(true);
    simulationActionLayout->addWidget(fullScrean);
    fullScrean->hide();

    simulationStart = new QPushButton("START");
    simulationStart->setMaximumHeight(this->height()/24);
    simulationActionLayout->addWidget(simulationStart);
    simulationStart->hide();

    simulationPause = new QPushButton("PAUSE");
    simulationPause->setMinimumHeight(this->height()/24);
    simulationPause->setCheckable(true);
    simulationActionLayout->addWidget(simulationPause);
    simulationPause->hide();

    quitButton = new QPushButton("QUIT");
    quitButton->setMinimumHeight(this->height()/24);
    simulationActionLayout->addWidget(quitButton);
    quitButton->hide();

    simW = new QSpinBox;
    simW->setMaximumHeight(this->height()/24);
    simW->setMinimum(0);
    simW->setMaximum(300);
    simW->setValue(30);
    simConfigLayout->addWidget(simW);
    simW->hide();

    simH = new QSpinBox;
    simH->setMaximumHeight(this->height()/24);
    simH->setMinimum(0);
    simH->setMaximum(150);
    simH->setValue(15);
    simConfigLayout->addWidget(simH);
    simH->hide();

    nbLions = new QSpinBox;
    nbLions->setMaximumHeight(this->height()/24);
    nbLions->setMinimum(0);
    nbLions->setMaximum(1000);
    nbLions->setValue(4);
    simConfigLayout->addWidget(nbLions);
    nbLions->hide();

    nbGazelles = new QSpinBox;
    nbGazelles->setMaximumHeight(this->height()/24);
    nbGazelles->setMinimum(0);
    nbGazelles->setMaximum(4000);
    nbGazelles->setValue(20);
    simConfigLayout->addWidget(nbGazelles);
    nbGazelles->hide();

    speed = new QSlider(Qt::Horizontal);
    speed->setMinimumWidth(this->width()/3);
    speed->setMaximumHeight(this->height()/24);
    speed->setMinimum(0);
    speed->setMaximum(1000);
    speed->setValue(20);
    simConfigLayout->addWidget(speed);
    speed->hide();

    mainLayout->addLayout(serverConnectLayout);
    mainLayout->addLayout(simulationActionLayout);
    mainLayout->addLayout(simConfigLayout);
    mainLayout->addLayout(simLayout);
    mainWidget->setLayout(mainLayout);

    this->setCentralWidget(mainWidget);

    timer = new QTimer(this);

    //Toggle fullscreen
    connect(fullScrean, SIGNAL(toggled(bool)), this, SLOT(toggleFS(bool)));
    // Action Connexion au Serveur
    connect(serverConnect, SIGNAL(clicked()), this, SLOT(connectToServer()));
    //Action démarrage simulation
    connect(simulationStart, SIGNAL(clicked()), this, SLOT(sendSimulationStart()));
    //Action Toggle Pause
    connect(simulationPause, SIGNAL(toggled(bool)), this, SLOT(sendSimulationPause(bool)));
    //Action Quit
    connect(quitButton, SIGNAL(clicked()), this, SLOT(quit()));
    //Slider
    connect(speed, SIGNAL(sliderReleased()), this, SLOT(speedChanged()));
}

MainWindow::~MainWindow()
{

}

void MainWindow::speedChanged(){
    client->send("SERVER@delay:1 "+std::to_string(speed->value()));
}

void MainWindow::connectToServer(){
    client = new QT_GenericClient(serverAddress->toPlainText(), serverPort->value());

    fullScrean->show();
    simulationStart->show();
    simulationPause->show();
    quitButton->show();
    speed->show();
    simW->show();
    simH->show();
    nbLions->show();
    nbGazelles->show();

    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(refreshDelay);
}

void MainWindow::sendSimulationStart(){
    if(client != nullptr){
        std::string sval("SERVER@simulation:5 start ");
        sval += std::to_string(simW->value());
        sval += " ";
        sval += std::to_string(simH->value());
        sval += " ";
        sval +=  std::to_string(nbLions->value());
        sval += " ";
        sval +=std::to_string(nbGazelles->value());

        client->send(sval);
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

void MainWindow::toggleFS(bool value){
    if(value){
        this->showFullScreen();
    }else{
        this->showNormal();
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
                }else if(scene == nullptr){
                    scene = new Scene(this->width(),this->height() , this);
                    sceneView = new QGraphicsView();
                    sceneView->setScene(scene);
                    simLayout->addWidget(sceneView);
                    scene->createEnvironnement(std::atoi(cmd.args[0].c_str()), std::atoi(cmd.args[1].c_str()));

                    qDebug() << "Earth is Flat !";
                }else{
                    client->send("SERVER@createEnvironnement:2 error world already created");
                }
            }else if(!cmd.command.compare("addAnimal")){
                if(cmd.args.size() < 5){
                    client->send("SERVER@addAnimal:2 error invalid parameters");
                }else if(scene!=nullptr){
                    scene->addAnimal(cmd.args[0], std::atoi(cmd.args[1].c_str()), std::atoi(cmd.args[2].c_str()),std::atoi(cmd.args[3].c_str()),std::atoi(cmd.args[4].c_str()));
                }
            }else if(!cmd.command.compare("attack")){
                if(cmd.args.size() < 4){
                    client->send("SERVER@attack:2 error invalid parameters");
                }else if(scene!=nullptr){
                    scene->attack(cmd.args[0], std::atoi(cmd.args[1].c_str()), cmd.args[2], std::atoi(cmd.args[3].c_str()));
                }
            }else if(!cmd.command.compare("move")){
                if(cmd.args.size() < 4){
                    client->send("SERVER@move:2 error invalid parameters");
                }else if(scene != nullptr){
                    scene->move(cmd.args[0], std::atoi(cmd.args[1].c_str()), cmd.args[2], std::atoi(cmd.args[3].c_str()));
                }
            }else if(!cmd.command.compare("damage")){
                if(cmd.args.size() < 3){
                    client->send("SERVER@damage:2 error invalid parameters");
                }else if(scene != nullptr){
                    scene->damage(cmd.args[0], std::atoi(cmd.args[1].c_str()), std::atoi(cmd.args[2].c_str()));
                }
            }
        }
    }
}

void MainWindow::quit(){
    client->send("SERVER@quit:0");

    exit(0);
}
