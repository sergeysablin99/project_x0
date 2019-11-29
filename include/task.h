#ifndef TASK_H
#define TASK_H

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
#include <QCheckBox>
#include <QGroupBox>
#include <QSharedPointer>
//#include "user.h"
#include "network.h"

class Task : public QWidget
{
  Q_OBJECT

  Network* network;

  //Static
  QString name;
  QLabel target;
  QLabel date;
  QPlainTextEdit description;
  QListWidget employee;
  QListWidget subtask;
  QGridLayout layout;
  QPushButton editButton;
  QPushButton BBack;

  //Edit
  QLineEdit nameEdit;
  QLineEdit targetEdit;
  QTextEdit descriptionEdit;
  QDateEdit dateEdit;

public:
  Task(QWidget* parent = nullptr);
  Task(QWidget* parent = nullptr, QString name = "", Network* manager = nullptr);
//  void updateEmployee(QListWidget *list);
//  void updateSubtask(QListWidget *list);
  ~Task();

  void setName(QString newName);
  void setDate(QDate newDate);
  void setDescription(QString newDescription);
  void setTarget(QString newTarget);
  void setEmployee(QVector<QString>);
  void setSubtask(QVector<QString>);
  void setNetwork(Network* network);
  void updateData(Task updateTask);

public slots:
  void SlotEditButton();
  void getReply();
  void back();

signals:
  void showProject();
};

#endif // TASK_H
