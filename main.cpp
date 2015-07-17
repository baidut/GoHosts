#include "gohosts.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    GoHosts g;
    g.show();

    return a.exec();
}
