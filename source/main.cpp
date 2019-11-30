#include "../include/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  MainWindow *w = new MainWindow;
  w->show();

//  Network n;
//  n.login("test", "test");
  return a.exec();
}
