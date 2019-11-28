#ifndef NETWORK_H
#define NETWORK_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QNetworkReply>
#include <QTimer>
#include <string>
#include "json.hpp"


class Network : public QObject
{
  Q_OBJECT
  bool bufferDynamic;
  QNetworkAccessManager networkManager;
  QString serverAddress;
  QVector<QString> unpackedReply;
  QVector<QPair<const QNetworkRequest, const QString>> requestBuffer;
  QVector<nlohmann::json> replyBuffer;

public:
  void getProjects();
  void getTasks(const QString projectName = "");
//  QString login(const QString login, const QString password);
  void deleteProject(const QString projectName);
  void createProject(const QString projectName, const QStringList & tasks);
  void deleteTask(const QString taskName, const QString projectName = "");
  void addTask(const QString taskName, const QString projectName);
  void createTask(const QString taskName);

  void setServerAddress(QString newAddress);
  void unpackReply(QString param);
  void setProjectName(QString project, QString newName);
  QNetworkRequest* prebuildRequest();
  QVector<QString> returnReply();
  void getTaskData(QString taskName);
  bool emptyJson(nlohmann::json json);

  QDate tasksDate();
  QString tasksTarget();
  QString tasksDescription();
  QVector<QString> tasksEmployee();
  QVector<QString> tasksSubtasks();

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

signals:
  void readFinished();
};

#endif // NETWORK_H
