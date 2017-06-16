#ifndef watch_exchange_H
#define watch_exchange_H

#include <qamqp/qamqpglobal.h>

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtCore/QTimer>

extern unsigned count, timeout;
extern QString host, virtual_host, user, passwd;
extern QStringList exchanges;

class watch_exchange : public QObject {
    Q_OBJECT

public:
    typedef enum {
        SHOW_KEY =     0x01,
        SHOW_HEADERS = 0x02,
        SHOW_TYPE =    0x04,
        SHOW_PAYLOAD = 0x08,
        SHOW_ALL =     0x0f
    } SHOW_SECTION;
    Q_DECLARE_FLAGS (SHOW_SECTIONS, SHOW_SECTION)
    Q_FLAG (SHOW_SECTIONS)

    static int list2sections (const char *list);

    watch_exchange (SHOW_SECTIONS show_sections = SHOW_ALL);
    virtual ~watch_exchange();

private Q_SLOTS:
    void clientConnected();
    void clientError (QAMQP::Error error);
    void exchangeDeclared();
    void queueDeclared();
    void messageReceived();
    static void stop();

private:
    class QAmqpClient *rabbit;
    class QAmqpQueue *queue;
    QTimer idle_timer;
    SHOW_SECTIONS show_sections;

};

Q_DECLARE_OPERATORS_FOR_FLAGS (watch_exchange::SHOW_SECTIONS)

#endif // watch_exchange_H
