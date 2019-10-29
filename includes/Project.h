#ifndef PROJECT_H
#define PROJECT_H

#include <QString>
#include <vector>
#include <QListWidget>
#include "../includes/network.h"
#include <QGridLayout>
#include "task.h"

class Project : public QWidget
{
  Q_OBJECT

  QString _name;
  Network *_network_manager;
  QGridLayout _layout;
  Task *_task;

public:
  QListWidget _tasks;

  Project();
  Project(QString name);
  ~Project();

  void setName(QString);
  void deleteTask(QString taskName);
  void addTask(QString taskName);

public slots:
  void openTask(QListWidgetItem *item);
};

#endif // PROJECT_H
