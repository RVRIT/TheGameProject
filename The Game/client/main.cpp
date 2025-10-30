#include "client.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    client window;
    window.show();
    return app.exec();
}
