#include "qt_genericclient.hpp"

QT_GenericClient::QT_GenericClient(QString address, quint16 port, QObject *parent) : QObject(parent), alive{true}
{
    socket = new QTcpSocket(this);
    Connect(address, port);

    connect(socket, SIGNAL(connected()), this, SLOT(connectedToServer()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnectedFromServer()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(receivedFromServer()));
    connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(sentToServer(qint64)));
}

void QT_GenericClient::Connect(QString address, quint16 port){

    qDebug() << "Connexion au Serveur "+address+" au port "+QString::number(port);

    socket->connectToHost(address, port);

    if(socket->waitForDisconnected(timeout))
    {
        qDebug() << "Error: " << socket->errorString();
    }
}

std::vector<std::string> QT_GenericClient::receive(){
    std::vector<std::string> output;

    for(const auto &it : buffer){
        output.push_back(it);
    }
    buffer.clear();

    return output;
}

bool QT_GenericClient::send(const std::string &message){
    if (message.length() <= 0) {
        qDebug() << "No message to send !";
        return false;
    }

    socket->write((message+"\n").c_str());

    return socket->waitForBytesWritten(writeWaitTime);;
}

bool QT_GenericClient::action(std::string msg) {
    auto cmd = cmdFormat::parseCommand(msg);

    if (!cmd.command.compare("quit")) {
        socket->close();
        alive = false;
    } else if (!cmd.command.compare("auth")) {
        if (cmd.args.size() < 2) {
            send(cmd.id + "@auth:2 error wrong values");
        } else {
            id = cmd.args[0];
            key = cmd.args[1];

            qDebug() << "id set to " << QString::fromStdString(id) << " and key is " << QString::fromStdString(key);
        }
    }else {
        return false;
    }

    return true;
}
void QT_GenericClient::connectedToServer(){
    qDebug() << "Connected to Server !";
}

void QT_GenericClient::disconnectedFromServer(){
    qDebug() << "Disconnected from server !";
}

void QT_GenericClient::receivedFromServer(){
    while (socket->canReadLine()) {
        auto tmp = socket->readLine();
        QString tmpBuffer = QString(tmp).trimmed();

        if (tmpBuffer.size() <= 0) {
            qDebug() << "Error: Server Disconnected !";
        } else if(!action(tmpBuffer.toStdString())){
            buffer.append(tmpBuffer.toStdString());
        }
    }
}

void QT_GenericClient::sentToServer(qint64 bytes){
    qDebug() << "Sent to Server: " << bytes << " bytes";
}
