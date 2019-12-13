#ifndef PROJECT_H
#define PROJECT_H

#include "task.h"


class Project : public QWidget
{
  Q_OBJECT

  //STATIC
  QLabel taskLabel;
  QString name;
  Network* network;
  QVBoxLayout layout;
  QHBoxLayout LButtons;
  Task *task;
  QPushButton editButton;
  QPushButton BBack;
  QPushButton BCreateTask;
  QListWidget taskList;

  //EDIT
  QDateEdit dateEdit;
  QLineEdit target;
  QLabel tasksLabel;
  QLabel employeeLabel;
  QGroupBox GBCheckBox;
  QVector<QCheckBox*> VCheckBox;
  QLineEdit setName;
  QHBoxLayout LCheckBox;
  QVBoxLayout LVCheckBox;
  QVBoxLayout LECheckBox;
  QVector<QCheckBox*> VECheckBox;
  QVBoxLayout LGroupBox;
  QLineEdit newTaskName;
  QStringList checkedTasks;
  QTextEdit newDescription;
  QMessageBox error;

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
  void getEmployee();
  void openTask(QListWidgetItem *item);
  void taskClosed();
  void createTask();
  void back();

signals:
  void showMainWindow();
};

#endif // PROJECT_H
