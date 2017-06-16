#include "watch_exchange.h"

#include <iostream>
#include <iomanip>
#include <QCoreApplication>
#include <QDateTime>
#include <QMetaClassInfo>
#include <qamqp/qamqpclient.h>
#include <qamqp/qamqpexchange.h>
#include <qamqp/qamqpqueue.h>

watch_exchange::watch_exchange (SHOW_SECTIONS show_sections) :
    show_sections (show_sections) {
    rabbit = new QAmqpClient (this);

    connect (&idle_timer, SIGNAL (timeout()),
             this, SLOT (stop()));
    idle_timer.setSingleShot (true);

    if (!host.isEmpty())
        rabbit->setHost (host);
    if (!virtual_host.isEmpty())
        rabbit->setVirtualHost (virtual_host);
    if (!user.isEmpty())
        rabbit->setUsername (user);
    if (!passwd.isEmpty())
        rabbit->setPassword (passwd);

    connect (rabbit, SIGNAL (connected()),
             this, SLOT (clientConnected()));
    connect (rabbit, SIGNAL (error (QAMQP::Error)),
             this, SLOT (clientError (QAMQP::Error)));
    rabbit->connectToHost();
}

watch_exchange::~watch_exchange() {}

void watch_exchange::clientConnected() {
    qDebug() << __FUNCTION__;

    queue = rabbit->createQueue();
    connect (queue, SIGNAL (declared()),
             this, SLOT (queueDeclared()));
    connect (queue, SIGNAL (error (QAMQP::Error)),
             this, SLOT (clientError (QAMQP::Error)));
    connect (queue, SIGNAL (messageReceived()),
             this, SLOT (messageReceived()));
    queue->declare (QAmqpQueue::Exclusive);
}

void watch_exchange::exchangeDeclared() {
    auto x = qobject_cast<QAmqpExchange *> (sender ());
    if (!x) return;

    QString x_name = x->name();
    qDebug() << __FUNCTION__ << x_name;
    queue->bind (x_name, QString());
}

void watch_exchange::queueDeclared() {
    queue->consume (QAmqpQueue::coNoAck);
    for (auto const &name: exchanges) {
        QAmqpExchange *x = rabbit->createExchange (name);
        connect (x, SIGNAL (declared()),
                 this, SLOT (exchangeDeclared()));
        connect (x, SIGNAL (error (QAMQP::Error)),
                 this, SLOT (clientError (QAMQP::Error)));
        x->declare (QAmqpExchange::Headers,
                    QAmqpExchange::Passive);
    }
}

void indent (unsigned level) {
    if (level)
        std::cout << std::setw (level * 4) << ' ';
}

void display (unsigned level,
              QString const &title,
              QVariant const &data) {
    indent (level);
    if (!title.isNull())
        std::cout << title.toStdString() << ':';

    switch (data.type()) {
    case QVariant::Hash: {
        std::cout << '{' << std::endl;
        auto h = data.toHash();
        for (auto const &k: h.keys()) {
            display (level+1, k, h[k]);
        }
        indent (level);
        std::cout << '}';
        break;
    }
    case QVariant::List: {
        std::cout << '[' << std::endl;
        for (auto const &e: data.toList()) {
            display (level+1, QString(), e);
        }
        indent (level);
        std::cout << ']';
        break;
    }
    case QVariant::DateTime: {
        std::cout << data.typeName() << ':' << data.toDateTime().
                  toString ("yyyy-MM-ddThh:mm:ss.zzz").toStdString();
        break;
    }
    default: {
        std::cout << data.typeName() << ':';
        if (data.canConvert<QString>()) {
            QString s = data.toString();
            std::cout << s.toStdString();
        } else {
            std::cout << "CAN NOT CONVERT";
        }
        break;
    }
    } // switch
    std::cout << std::endl;
}

void watch_exchange::messageReceived() {
    QAmqpQueue *q = qobject_cast<QAmqpQueue*> (sender());
    if (!q) return;

    QAmqpMessage message = q->dequeue();
    std::cout << "*** Message Received ***" << std::endl;
    if (show_sections & SHOW_KEY)
        display (0, "key", message.routingKey());
    if (show_sections & SHOW_HEADERS)
        display (0, "headers", message.property (QAmqpMessage::Headers));
    if (show_sections & SHOW_TYPE) {
        display (0, "content-type",
                 message.property (QAmqpMessage::ContentType));
        display (0, "content-encoding",
                 message.property (QAmqpMessage::ContentEncoding));
    }
    if (show_sections & SHOW_PAYLOAD)
        display (0, "payload", message.payload());
    if (count) {
        if (!--count) stop();
    }
    if (timeout) {
        idle_timer.start (timeout);
    }
}

void watch_exchange::clientError (QAMQP::Error error) {
    auto s = sender();
    if (!s) return;
    {
        auto c = qobject_cast<QAmqpClient *> (s);
        if (c) {
            qDebug() << "clientError:" << c->errorString();
            stop();
        }
    }
    {
        auto h = qobject_cast<QAmqpChannel *> (s);
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

int watch_exchange::list2sections (const char* list) {
    static const QMetaEnum rts_modes =
        watch_exchange::staticMetaObject.enumerator (
            watch_exchange::staticMetaObject.indexOfEnumerator (
                "SHOW_SECTIONS"));
    return rts_modes.keyToValue ( ("SHOW_" + QString (list).toUpper())
                                  .toLatin1().constData());
}


#include "watch_exchange.moc"
