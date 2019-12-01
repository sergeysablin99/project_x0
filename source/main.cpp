#include "../include/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  QString styles = R"(
  MainWindow {
                  background-color:#66CDAA;
                  }
  QPushButton {
      background-color:#66CDAA;
      }
  QLineEdit, QTextEdit, QGroupBox {
      background-color:#ffffff;
      }
  )";

  a.setStyleSheet(styles);

  MainWindow *w = new MainWindow;
  w->show();

  return a.exec();
}
