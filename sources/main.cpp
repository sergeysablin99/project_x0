#include "includes/mainwindow.h"
#include <QApplication>

#include "../includes/Project.h"
#include "../includes/network.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Network ntwrk;

    MainWindow mainwindow;
    mainwindow.show();

    return a.exec();
}
