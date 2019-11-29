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
  QPushButton BBack;
  QListWidget taskList;

  //EDIT
  QGroupBox GBCheckBox;
  QVector<QCheckBox *> VCheckBox;
  QLineEdit setName;
  QVBoxLayout LCheckBox;
  QVBoxLayout LGroupBox;

public:
  Project(QWidget* parent = nullptr);
  Project(QWidget* parent = nullptr, QString name = "", Network* manager = nullptr);
  Project(const Project& copy);
  ~Project();
  void hideAll();
  void edit();
  void showMainPage();

public slots:
  void getReply();
  void openTask(QListWidgetItem *item);
  void taskClosed();
  void back();

signals:
  void showMainWindow();
};

#endif // PROJECT_H
