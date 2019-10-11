#include "includes/mainwindow.h"
#include <QApplication>

//#include "tests/User/tst_user.h"
//#include <gtest/gtest.h>
#include "../includes/task.h"

#include "../includes/taskfolder.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);



    Tcp tcp;
    tcp.doConenction();

    Task task;
    task.show();
    return a.exec();
}
