#include "subscriber.h"
#include "dialogue.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Subscriber s;

    return a.exec();
}
