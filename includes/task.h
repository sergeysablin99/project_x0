#ifndef DAY_H
#define DAY_H

#include <vector>
#include <QString>
#include <QDate>
#include "user.h"
#include <QHBoxLayout>

class Task{
  QString name;
  QString target;
  QDate date;
  QString description;
  std::vector<User> employee;
  std::vector<Task> subtask;
  QHBoxLayout layout;
};

#endif // DAY_H
