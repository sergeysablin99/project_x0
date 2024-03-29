﻿#ifndef TASK_H
#define TASK_H

#include <QListWidget>
#include <QWidget>
#include <QValidator>
#include <QGridLayout>
#include <QPlainTextEdit>
#include <QString>
#include <QLabel>
#include <QDateEdit>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QDateEdit>
#include <QCheckBox>
#include <QGroupBox>
#include <QSharedPointer>
#include "network.h"

class Task : public QWidget
{
  Q_OBJECT

  Network* network;

  //Static
  QVBoxLayout LMain;
  QHBoxLayout LLineEdit;
  QHBoxLayout LGroupBox;
  QHBoxLayout LButtons;

  QString name;
  QLabel target;
  QLabel date;
  QPlainTextEdit description;
  QListWidget employee;
  QListWidget subtask;

//  QGridLayout layout;
  QPushButton editButton;
  QPushButton BBack;
  QPushButton BFinish;

  //Edit
  QMessageBox error;
  QLabel tasksLabel;
  QLabel employeeLabel;
  QLineEdit nameEdit;
  QLineEdit targetEdit;
  QTextEdit descriptionEdit;
  QDateEdit dateEdit;
  QGroupBox GBECheckBox;
  QVector<QCheckBox *> VECheckBox;
  QVBoxLayout LECheckBox;
  QVBoxLayout LEGroupBox;

  QGroupBox GBTCheckBox;
  QVector<QCheckBox *> VTCheckBox;
  QVBoxLayout LTCheckBox;
  QVBoxLayout LTGroupBox;

public:
  Task(QWidget* parent = nullptr);
  Task(QWidget* parent = nullptr, QString name = "", Network* manager = nullptr);
  ~Task();

  void setName(QString newName);
  void setDate(QDate newDate);
  void setDescription(QString newDescription);
  void setTarget(QString newTarget);
  void setEmployee(QVector<QString>);
  void setSubtask(QVector<QString>);
  void setNetwork(Network* network);
  void updateData(Task updateTask);
  QListWidget* getSubtasks();

public slots:
  void finished();
  void SlotEditButton();
  void returnEmployee();
  void returnSubtasks();
  void getReply();
  void back();

signals:
  void showProject();
};

#endif // TASK_H
