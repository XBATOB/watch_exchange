#ifndef watch_exchange_H
#define watch_exchange_H

#include <QtCore/QObject>

class watch_exchange : public QObject
{
    Q_OBJECT

public:
    watch_exchange();
    virtual ~watch_exchange();

private slots:
    void output();
};

#endif // watch_exchange_H
