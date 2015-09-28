#include "watch_exchange.h"

#include <QTimer>
#include <iostream>

watch_exchange::watch_exchange()
{
    QTimer* timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()), SLOT(output()) );
    timer->start( 1000 );
}

watch_exchange::~watch_exchange()
{}

void watch_exchange::output()
{
    std::cout << "Hello World!" << std::endl;
}

#include "watch_exchange.moc"
