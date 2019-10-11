#ifndef TASKFOLDER_H
#define TASKFOLDER_H

#include <QTcpSocket>
#include <QString>
#include <QByteArray>
#include <QDataStream>

class Tcp{
  QString driver_name = "OrientDB Java client";
  QString driver_version = "1.0rc8-SNAPSHOT";
  short protocol_version = 30;
  QString client_id = "2480";
  QString serialization_impl = "ORecordSerializerBinary";
  bool token_session = true;
  bool support_push = false;
  bool collect_stats = false;
  QString database_name = "demodb";
  QString user_name = "admin";
  QString user_password = "admin";

  QTcpSocket socket;

public:
  void doConenction();
};

void Tcp::doConenction()
{
  socket.connectToHost ("localhost", 2480);
  QByteArray writeBlock;
  QDataStream out(&writeBlock, QIODevice::WriteOnly);
  out << driver_name << driver_version << protocol_version
      << client_id << serialization_impl << token_session <<
         support_push << collect_stats << database_name << user_name << user_password;
  socket.write(writeBlock);
}

#endif // TASKFOLDER_H
