#include <QCoreApplication>
#include "watch_exchange.h"


int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    watch_exchange watch_exchange;
    return app.exec();
}
