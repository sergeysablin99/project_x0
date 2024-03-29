﻿#ifndef NETWORK_H
#define NETWORK_H

#include <QWidget>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QNetworkReply>
#include <QTimer>
#include <string>
#include "json.hpp"
#include "user.h"

class Network : public QObject
{
  Q_OBJECT
  bool bufferDynamic;
  QNetworkAccessManager networkManager;
  QString serverAddress;
  QVector<QString> unpackedReply;
  QVector<QPair<const QNetworkRequest, const QString>> requestBuffer;
  QVector<nlohmann::json> replyBuffer;

  QMessageBox error;

public:
  User user;
  void getProjects();
  void getTasks(const QString projectName = "");
  void getEmployee(QString taskName = "");
  void login();
  void deleteProject(const QString projectName);
  void createProject(const QString projectName, const QStringList & tasks);
  void deleteTask(const QString taskName, const QString projectName = "");
  void addTask(const QString taskName, const QString projectName);
  void addEmployee(QString employee, QString taskName);
  void deleteEmployee(QString employee, QString taskName);
  void deleteSubtask(QString subtaskName, QString taskName);
  void addSubtask(QString subtaskName, QString taskName);
  void createTask(const QString taskName, const QStringList& tasks,const QStringList& employee,
                  const QString description,const QString target, const QDate date);
  void setFinished(QString task);

  void setServerAddress(QString newAddress);
  void unpackReply(QString param);
  void setProjectName(QString project, QString newName);
  QNetworkRequest* prebuildRequest();
  QVector<QString> returnReply();
  void getTaskData(QString taskName);
  bool emptyJson(nlohmann::json json);
  int defineReply();

  QDate tasksDate();
  QString tasksTarget();
  QString tasksDescription();
  QVector<QString> tasksEmployee();
  void tasksSubtasks(QString taskName = "");
  void personalTasks();
  QString returnPersonalTasks();

  void changeDate(QString task, QString newDate);
  void changeTarget(QString task, QString newTarget);
  void changeDescription(QString task, QString newDescription);
  void changeName(QString task, QString newName);

  Network();
  Network(Network const &ntwrk);
  ~Network();

public slots:
  void slotReadyRead(QNetworkReply *reply);
  void slotReadyWrite();
  void slotAuth(QNetworkReply *reply);
  void loginError(QNetworkReply::NetworkError code);

signals:
  void readFinished();
  void returnEmployee();
  void returnSubtasks();
  void loggedIn();
  void signalReturnPersonaTasks();
};

#endif // NETWORK_H
