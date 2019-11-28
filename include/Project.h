#ifndef PROJECT_H
#define PROJECT_H

#include "task.h"


class Project : public QWidget
{
  Q_OBJECT

  //STATIC
  QString name;
  Network* network;
  QHBoxLayout layout;
  Task *task;
  QPushButton editButton;
  QListWidget taskList;

  //EDIT
  QGroupBox GBCheckBox;
  QVector<QCheckBox *> VCheckBox;
  QLineEdit setName;
  QVBoxLayout LCheckBox;
  QVBoxLayout LGroupBox;

public:
  Project();
  Project(QString name, Network* manager);
  ~Project();
  void hideAll();
  void edit();
  void showMainPage();

public slots:
  void getReply();
  void openTask(QListWidgetItem *item);
};

#endif // PROJECT_H
