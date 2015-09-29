#include "watch_exchange.h"

#include <QDebug>
#include <QCoreApplication>
#include <qamqp/qamqpclient.h>
#include <qamqp/qamqpexchange.h>
#include <qamqp/qamqpqueue.h>

watch_exchange::watch_exchange() {
    rabbit = new QAmqpClient(this);

    connect(&idle_timer, SIGNAL(timeout()),
            this, SLOT(stop()));
    idle_timer.setSingleShot(true);

    if (!host.isEmpty())
        rabbit->setHost(host);
    if (!virtual_host.isEmpty())
        rabbit->setVirtualHost(virtual_host);
    if (!user.isEmpty())
        rabbit->setUsername(user);
    if (!passwd.isEmpty())
        rabbit->setPassword(passwd);

    connect (rabbit, SIGNAL (connected()),
             this, SLOT (clientConnected()));
    connect (rabbit, SIGNAL(error(QAMQP::Error)),
             this, SLOT (clientError(QAMQP::Error)));
    rabbit->connectToHost();
}

watch_exchange::~watch_exchange() {}

void watch_exchange::clientConnected() {
    qDebug() << __FUNCTION__;

    queue = rabbit->createQueue();
    connect (queue, SIGNAL (declared()),
             this, SLOT (queueDeclared()));
    connect(queue, SIGNAL(error(QAMQP::Error)),
            this, SLOT(clientError(QAMQP::Error)));
    connect (queue, SIGNAL (messageReceived()),
             this, SLOT (messageReceived()));
    queue->declare (QAmqpQueue::Exclusive);
}

void watch_exchange::exchangeDeclared() {
    auto x = qobject_cast<QAmqpExchange *>(sender ());
    if (!x) return;

    QString x_name = x->name();
    qDebug() << __FUNCTION__ << x_name;
    queue->bind(x_name, QString());
}

void watch_exchange::queueDeclared() {
    queue->consume(QAmqpQueue::coNoAck);
    for (auto const &name: exchanges) {
        QAmqpExchange *x = rabbit->createExchange (name);
        connect (x, SIGNAL (declared()),
                 this, SLOT (exchangeDeclared()));
        connect(x, SIGNAL(error(QAMQP::Error)),
                this, SLOT(clientError(QAMQP::Error)));
        x->declare (QAmqpExchange::Headers,
                    QAmqpExchange::Passive);
    }
}

void watch_exchange::messageReceived() {
    QAmqpQueue *q = qobject_cast<QAmqpQueue*> (sender());
    if (!q) return;

    QAmqpMessage message = q->dequeue();
    qDebug() << "*** Message Received ***";
    qDebug() << "key:" << message.routingKey();
    qDebug() << "headers:";
    auto hdrs = message.property (QAmqpMessage::Headers).toHash();
    for (auto h: hdrs.keys()) {
        auto v = hdrs[h].toString();
        if (v.isEmpty())
            qDebug() << "*" << h << hdrs[h];
        else
            qDebug() << "*" << h << v;
    }
    qDebug() << "payload:" << message.payload();
    if (count) {
        if (!--count) stop();
    }
    if (timeout) {
        idle_timer.start(timeout);
    }
}

void watch_exchange::clientError(QAMQP::Error error) {
    auto s = sender();
    if (!s) return;
    {
        auto c = qobject_cast<QAmqpClient *>(s);
        if (c) {
            qDebug() << "clientError:" << c->errorString();
            stop();
        }
    }
    {
        auto h = qobject_cast<QAmqpChannel *>(s);
        if (h) {
            qDebug() << "channel" << h->name() <<
                     "Error:" << h->errorString();
            stop();

        }
    }
    stop();
}

void watch_exchange::stop() {
    QCoreApplication::instance()->quit();
}

#include "watch_exchange.moc"
