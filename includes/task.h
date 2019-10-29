﻿#ifndef DAY_H
#define DAY_H

#include <QListWidget>
#include <QWidget>
#include <QGridLayout>
#include <QPlainTextEdit>
#include <QString>
#include <QLabel>
#include <QDateEdit>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QDateEdit>
#include "user.h"
#include "network.h"

class Task : public QWidget
{
  Q_OBJECT

  Network *_network_manager;

  //Static
  QString _name;
  QLabel _target;
  QLabel _date;
  QPlainTextEdit _description;
  QListWidget _employee;
  QListWidget _subtask;
  QGridLayout _layout;
  QPushButton _editButton;

  //Edit
  QLineEdit _targetEdit;
  QTextEdit _descriptionEdit;
  QDateEdit _dateEdit;

public:
  Task();
  Task(QString name);
  ~Task();

public slots:
  void SlotEditButton();
};

#endif // DAY_H
