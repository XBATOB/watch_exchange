#include <QCoreApplication>
#include <QTextCodec>
#include <QStringList>
#include "watch_exchange.h"

#include <argp.h>

unsigned count, timeout;
QString host, virtual_host, user, passwd;
QStringList exchanges;
watch_exchange::SHOW_SECTIONS sections = watch_exchange::SHOW_ALL;

static error_t apf (int key, char *arg, struct argp_state *state) {
    switch (key) {
    case 'H':
        host = arg;
        break;

    case 'V':
        virtual_host = arg;
        break;

    case 'U':
        user = arg;
        break;

    case 'P':
        passwd = arg;
        break;

    case 'c':
        count = strtoul(arg, nullptr, 0);
        break;

    case 't':
        timeout = strtoul(arg, nullptr, 0) * 1000;
        break;

    case 's':
        sections = watch_exchange::list2sections (arg);
        break;

    case ARGP_KEY_ARG:
        exchanges << arg;
        break;

    case ARGP_KEY_END:
        if (exchanges.isEmpty())
            argp_error (state, "No exchanges set");
        break;

    default:
        return  ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static const struct argp_option apo[] = {
    {
        "host", 'H', "NAME", 0,
        "Server host"
    }, {
        "virtual-host", 'V', "NAME", 0,
        "Virtual host on server"
    }, {
        "user", 'U', "NAME", 0,
        "User name"
    }, {
        "password", 'P', "PASSWORD", 0,
        "User password"
    }, {
        "count", 'c', "NUM", 0,
        "Stop after NUM packets received"
    }, {
        "timeout", 't', "SEC", 0,
        "Stop after SECs of idle"
    }, {
        "sections", 's', "list", 0,
        "Sections to show"
        " [ALL=KEY|HEADERS|TYPE|PAYLOAD]"
    }, {
        0, 0, 0, 0,
        "Qt options are also acceptable."
        " See Qt documentation.", -10
    }, {
        0
    }
};

static const struct argp ap = {
    apo, apf, "exchange...",
    "Watch and display some exchanges"
};

int main(int argc, char** argv) {
    QCoreApplication app(argc, argv);

    argp_parse (&ap, argc, argv, 0, 0, 0);

    watch_exchange watch_exchange (sections);
    return app.exec();
}
