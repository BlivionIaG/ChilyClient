#ifndef QT_GENERICCLIENT_HPP
#define QT_GENERICCLIENT_HPP

#include <QObject>

class QT_GenericClient : public QObject
{
    Q_OBJECT
public:
    explicit QT_GenericClient(QObject *parent = nullptr);

signals:

public slots:
};

#endif // QT_GENERICCLIENT_HPP