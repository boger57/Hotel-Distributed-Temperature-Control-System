#include "login.h"
#include <QApplication>
#include "newconnect.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (!createConnection())
        return 1;
    Login d;
    d.show();

    return a.exec();
}
