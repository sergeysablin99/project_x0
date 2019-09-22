#include "includes/mainwindow.h"
#include <QApplication>

#include "tests/User/tst_user.h"
#include <gtest/gtest.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    ::testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();

    return a.exec();
}
