#ifndef watch_exchange_H
#define watch_exchange_H

#include <qamqp/qamqpglobal.h>

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QStringList>

extern QString host, virtual_host, user, passwd;
extern QStringList exchanges;

class watch_exchange : public QObject
{
    Q_OBJECT

public:
    watch_exchange();
    virtual ~watch_exchange();

private Q_SLOTS:
    void clientConnected();
    void clientError(QAMQP::Error error);
    void exchangeDeclared();
    void queueDeclared();
    void messageReceived();

private:
    class QAmqpClient *rabbit;
    class QAmqpQueue *queue;
    QMap<QString, QString> bindings;
};

#endif // watch_exchange_H
