#include "lekaren.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    lekaren w;
    w.show();
    return a.exec();
}
