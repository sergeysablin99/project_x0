#include "../include/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  QString styles = R"(
  .MainWindow {
                  background-color:#E0FFFF;
                  min-width: 500px;
                  min-height: 350px;
                  }

  QPushButton {
      background-color:#66CDAA;
      color: #ffffff;
      text-transform: uppercase;
      border-style: none;
      min-width: 70px;
      min-height: 20px;
      border-radius: 2px;
      font-weight: 10px;
      padding: 0 5px 0 5px;
      }

  QLineEdit, QTextEdit, QGroupBox {
      background-color:#ffffff;
      }

  QGroupBox#GBECheckBox, QGroupBox#GBTChechBox, QCheckBox  {
      min-width: 50px;
      min-height 50px;
      }

  QGroupBox, QListWidget {
            min-width: 200px;
            min-height: 100px;
        }
  QLabel {
      color:#66CDAA;
      text-transform: uppercase;
      font-weight: bold;
      min-width: 120px;
      min-height: 15px;
      }

  QTextEdit#descriptionEdit {
      min-widht: 200px;
      min-height: 50px;
      }

  QLineEdit#targetEdit, QLineEdit#nameEdit {
      min-width: 200px;
      }
  )";

  a.setStyleSheet(styles);

  MainWindow *w = new MainWindow;
  w->show();

  return a.exec();
}
