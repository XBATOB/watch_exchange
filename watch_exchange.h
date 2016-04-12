#ifndef watch_exchange_H
#define watch_exchange_H

#include <qamqpglobal.h>

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtCore/QTimer>

extern unsigned count, timeout;
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
    static void stop();

private:
    class QAmqpClient *rabbit;
    class QAmqpQueue *queue;
    QTimer idle_timer;

};

#endif // watch_exchange_H
