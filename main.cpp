#include "resistors_calculator.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Resistors_Calculator w;
    w.show();
    return a.exec();
}
