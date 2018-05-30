#include "mainwindow.hpp"

MainWindow::MainWindow(int width, int height, QString title, QWidget *parent)
    : QMainWindow(parent), mainWidget{new QWidget}, mainLayout{new QVBoxLayout}, serverConnectLayout{new QHBoxLayout}, simulationActionLayout{new QHBoxLayout}, simConfigLayout{new QHBoxLayout},simLayout{new QHBoxLayout}, client{nullptr}, scene{nullptr}, sceneView{nullptr}
{
    this->setWindowTitle(title);
    this->setMinimumHeight(height);
    this->setMinimumWidth(width);

    /* Champ de Texte addresse du serveur */
    serverAddress = new QTextEdit;
    serverAddress->setMaximumHeight(this->height()/24);
    serverAddress->setPlainText("localhost");
    serverConnectLayout->addWidget(serverAddress);

    /* Champ de Texte port serveur */
    serverPort = new QSpinBox;
    serverPort->setMaximumWidth(this->width()/8);
    serverPort->setMaximumHeight(this->height()/24);
    serverPort->setMinimum(0);
    serverPort->setMaximum(65535);
    serverPort->setValue(42080);
    serverConnectLayout->addWidget(serverPort);

    /* Button Connexion */
    serverConnect = new QPushButton("CONNECT");
    serverConnect->setMinimumHeight(this->height()/24);
    serverConnectLayout->addWidget(serverConnect);

    /* Bouton fullscreen */
    fullScrean = new QPushButton("FULLSCREEN");
    fullScrean->setMinimumHeight(this->height()/24);
    fullScrean->setCheckable(true);
    simulationActionLayout->addWidget(fullScrean);
    fullScrean->hide();

    /* Bouton démarrage simulation */
    simulationStart = new QPushButton("START");
    simulationStart->setMaximumHeight(this->height()/24);
    simulationActionLayout->addWidget(simulationStart);
    simulationStart->hide();

    /* Bouton Pause simulation */
    simulationPause = new QPushButton("PAUSE");
    simulationPause->setMinimumHeight(this->height()/24);
    simulationPause->setCheckable(true);
    simulationActionLayout->addWidget(simulationPause);
    simulationPause->hide();

    /* Bouton Quitter */
    quitButton = new QPushButton("QUIT");
    quitButton->setMinimumHeight(this->height()/24);
    simulationActionLayout->addWidget(quitButton);
    quitButton->hide();

    /* Champ de texte Largeur de la simulation */
    simW = new QSpinBox;
    simW->setMaximumHeight(this->height()/24);
    simW->setMinimum(0);
    simW->setMaximum(300);
    simW->setValue(30);
    simConfigLayout->addWidget(simW);
    simW->hide();

    /* Champ de texte Hauteur de la simulation */
    simH = new QSpinBox;
    simH->setMaximumHeight(this->height()/24);
    simH->setMinimum(0);
    simH->setMaximum(150);
    simH->setValue(15);
    simConfigLayout->addWidget(simH);
    simH->hide();

    /* Champ de texte Nb de lions */
    nbLions = new QSpinBox;
    nbLions->setMaximumHeight(this->height()/24);
    nbLions->setMinimum(0);
    nbLions->setMaximum(1000);
    nbLions->setValue(4);
    simConfigLayout->addWidget(nbLions);
    nbLions->hide();

    /* Champ de texte nbGazelles */
    nbGazelles = new QSpinBox;
    nbGazelles->setMaximumHeight(this->height()/24);
    nbGazelles->setMinimum(0);
    nbGazelles->setMaximum(4000);
    nbGazelles->setValue(20);
    simConfigLayout->addWidget(nbGazelles);
    nbGazelles->hide();

    /*  Slider vitesse */
    speed = new QSlider(Qt::Horizontal);
    speed->setMinimumWidth(this->width()/3);
    speed->setMaximumHeight(this->height()/24);
    speed->setMinimum(0);
    speed->setMaximum(1000);
    speed->setValue(20);
    simConfigLayout->addWidget(speed);
    speed->hide();

    /* Lien des layouts vers le layout principal */
    mainLayout->addLayout(serverConnectLayout);
    mainLayout->addLayout(simulationActionLayout);
    mainLayout->addLayout(simConfigLayout);
    mainLayout->addLayout(simLayout);
    mainWidget->setLayout(mainLayout);

    this->setCentralWidget(mainWidget);

    timer = new QTimer(this); // Déclaration timer

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

void MainWindow::speedChanged(){  //Quand le slider change de valeur
    client->send("SERVER@delay:1 "+std::to_string(speed->value()));
}

void MainWindow::connectToServer(){ //Quand on veux se connecter au serveur
    //Création du client QT
    client = new QT_GenericClient(serverAddress->toPlainText(), serverPort->value());

    //Affichagee des widgets
    fullScrean->show();
    simulationStart->show();
    simulationPause->show();
    quitButton->show();
    speed->show();
    simW->show();
    simH->show();
    nbLions->show();
    nbGazelles->show();

    //Début timer
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(refreshDelay); //Délai par défault
}

void MainWindow::sendSimulationStart(){ //Quand on veut démarrer la simulation
    if(client != nullptr){ // Si client initialisé
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

void MainWindow::sendSimulationPause(bool value){ //Quand on veut metre en pause
    if(client != nullptr){
        if(value){
            client->send("SERVER@pause:1 on");
        }else{
            client->send("SERVER@pause:1 off");
        }
    }
}

void MainWindow::toggleFS(bool value){ //Fullscreen
    if(value){
        this->showFullScreen();
    }else{
        this->showNormal();
    }
}

void MainWindow::update(){ //Reception des commandes
    auto tmp = client->receive(); //Récupération des données recues

    if(tmp.size() > 0){ //Si données recues
        for(auto &i : tmp){
            auto cmd = cmdFormat::parseCommand(i); //Formatage commande

            if(!cmd.command.compare("createEnvironnement")){ //Si création monde
                if(cmd.args.size() < 2){ //detect error
                    client->send("SERVER@createEnvironnement:2 error invalid parameters");
                }else if(scene == nullptr){ //si pas de scene
                    scene = new Scene(this->width(),this->height() , this); //Création scène
                    sceneView = new QGraphicsView(); //Création vue
                    sceneView->setScene(scene);
                    simLayout->addWidget(sceneView);
                    scene->createEnvironnement(std::atoi(cmd.args[0].c_str()), std::atoi(cmd.args[1].c_str())); //Et la lumière fut

                    qDebug() << "Earth is Flat !";
                }else{
                    client->send("SERVER@createEnvironnement:2 error world already created"); //Aie
                }
            }else if(!cmd.command.compare("addAnimal")){ //Ajout animal
                if(cmd.args.size() < 5){ //Si pas assez de paramètres
                    client->send("SERVER@addAnimal:2 error invalid parameters");
                }else if(scene!=nullptr){ //Si pas de scène
                    scene->addAnimal(cmd.args[0], std::atoi(cmd.args[1].c_str()), std::atoi(cmd.args[2].c_str()),std::atoi(cmd.args[3].c_str()),std::atoi(cmd.args[4].c_str()));//Ajout
                }
            }else if(!cmd.command.compare("attack")){//Attaque d'un animal
                if(cmd.args.size() < 4){
                    client->send("SERVER@attack:2 error invalid parameters");
                }else if(scene!=nullptr){
                    scene->attack(cmd.args[0], std::atoi(cmd.args[1].c_str()), cmd.args[2], std::atoi(cmd.args[3].c_str()));
                }
            }else if(!cmd.command.compare("move")){ //Déplacement
                if(cmd.args.size() < 4){
                    client->send("SERVER@move:2 error invalid parameters");
                }else if(scene != nullptr){
                    scene->move(cmd.args[0], std::atoi(cmd.args[1].c_str()), cmd.args[2], std::atoi(cmd.args[3].c_str()));
                }
            }else if(!cmd.command.compare("damage")){ //Dégats
                if(cmd.args.size() < 3){
                    client->send("SERVER@damage:2 error invalid parameters");
                }else if(scene != nullptr){
                    scene->damage(cmd.args[0], std::atoi(cmd.args[1].c_str()), std::atoi(cmd.args[2].c_str()));
                }
            }
        }
    }
}

void MainWindow::quit(){ //Quand on veuxquitter
    client->send("SERVER@quit:0");

    exit(0);
}
