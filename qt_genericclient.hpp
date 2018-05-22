#ifndef QT_GENERICCLIENT_HPP
#define QT_GENERICCLIENT_HPP

#include <QObject>
#include <QTcpSocket>
#include <QDebug>
#include <QVector>

#include <string>
#include <vector>

class QT_GenericClient : public QObject
{
    Q_OBJECT
public:
    explicit QT_GenericClient(QString address = "localhost", quint16 port = 42080, QObject *parent = nullptr);

    bool isAlive() const { return alive; }
    void toggleAlive() { alive = !alive; }
    void setAlive(bool parm) { alive = parm; }

    void Connect(QString address, quint16 port);
    std::vector<std::string> receive();
    bool send(const std::string &message);
    bool action(std::string msg);

signals:

public slots:
    void connectedToServer();
    void disconnectedFromServer();
    void receivedFromServer();
    void sentToServer(qint64 bytes);

private:
    QTcpSocket *socket;
    QVector<std::string> buffer;

    std::string id, key;
    bool alive;

    const int timeout = 1000;
    const int writeWaitTime = 3000;
};

#endif // QT_GENERICCLIENT_HPP
