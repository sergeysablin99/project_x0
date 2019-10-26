#ifndef TASKFOLDER_H
#define TASKFOLDER_H

#include <QWidget>
#include <QNetworkReply>
#include "../includes/json.hpp"
#include <QEventLoop>

class Network : public QWidget
{
  Q_OBJECT

  QNetworkAccessManager *_manager;
  nlohmann::json _reply;
  QMetaObject::Connection _connection;

public:
  QEventLoop _wait;

  std::vector<std::string> getReply(QString param);
  void doRequest(QString string);
  void authorize(QString login, QString password);
  void waitFunction();
  Network();
  ~Network();

public slots:
  void slotReadyRead(QNetworkReply *reply);
};


#endif // TASKFOLDER_H
