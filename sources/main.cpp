#include "includes/mainwindow.h"
#include <QApplication>

#include "../includes/task.h"
#include "../includes/network.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Network ntwrk;

    Task task("imya");
    task.show();

    return a.exec();
}
