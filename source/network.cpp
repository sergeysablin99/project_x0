#include "../include/network.h"

Network::Network()
{
  connect(&(this->networkManager), &QNetworkAccessManager::finished, this, &Network::slotAuth);
  this->serverAddress = "http://localhost:7474/db/data/transaction/commit";
  this->bufferDynamic = false;
}

void Network::getTasks(const QString projectName)
{
  QNetworkRequest *request = prebuildRequest();

  QString request_command = QString(R"({"statements":[{"statement":")");

  if(projectName != "")
    request_command.append("MATCH (:Project {name : '" + projectName + "'})<-[:IN_PROJECT]-(t:Task) RETURN t");
  else
    request_command.append("MATCH (t:Task) RETURN t");

  request_command.append(R"("}]})");

  this->requestBuffer.push_back(qMakePair(*request, request_command));
  if (this->bufferDynamic == false)
    this->slotReadyWrite();

  delete request;
}

void Network::slotReadyRead(QNetworkReply *reply)
{
  if (reply != nullptr){
      if (this->requestBuffer.isEmpty())
        this->bufferDynamic = false;
      else
        this->bufferDynamic = true;

      auto rep = reply->readAll();
      nlohmann::json jsonReply = nlohmann::json::parse(rep);
      qDebug() << "Response: " << jsonReply.dump().data();

      if (this->emptyJson(jsonReply))
        {
          this->replyBuffer.push_back(jsonReply);

          if (this->defineReply() == 1)
            emit this->readFinished();
          else
            if (this->defineReply() == 0)
              emit this->returnEmployee();
          else
              if (this->defineReply() == 3)
                emit this->signalReturnPersonaTasks();
          else
              emit this->returnSubtasks();
        }

      emit this->slotReadyWrite();

      delete reply;
    }
}

void Network::slotReadyWrite()
{
      QEventLoop loop;
      QTimer::singleShot(100, &loop, &QEventLoop::quit);
      loop.exec();

      if (!(this->requestBuffer.isEmpty()))
        {
          auto request = this->requestBuffer.takeFirst();
          qDebug() << "send request: " << request.second;
          QNetworkReply* reply = this->networkManager.post(request.first, request.second.toUtf8());

          QMessageBox *error = &(this->error);

          connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
                  [error, reply](QNetworkReply::NetworkError code)
          {
              if (code != QNetworkReply::NoError && code != QNetworkReply::ContentOperationNotPermittedError)
                {
                  reply->close();

                  error->setText("Server error");
                  if (!(error->button(QMessageBox::Ok)))
                    error->addButton(QMessageBox::Ok);
                  error->show();
                }
            });
        }
}

void Network::getProjects()
{
  QNetworkRequest *request = prebuildRequest();

  QString request_command = QString(R"({"statements":[{"statement":")");

  request_command.append("MATCH (p:Project) RETURN p");

  request_command.append(R"("}]})");


  this->requestBuffer.push_back(qMakePair(*request, request_command));
  if (this->bufferDynamic == false)
    this->slotReadyWrite();

  delete request;
}

void Network::deleteProject(const QString projectName)
{
  QNetworkRequest *request = prebuildRequest();

  QString request_command = QString(R"({"statements":[{"statement":")");

  request_command.append("MATCH (n:Project {name : '" + projectName + "'}) DETACH DELETE n");

  request_command.append(R"("}]})");

  this->requestBuffer.push_back(qMakePair(*request, request_command));
  if (this->bufferDynamic == false)
    this->slotReadyWrite();

  delete request;
}

void Network::deleteTask(const QString taskName, const QString projectName)
{
  QNetworkRequest *request = prebuildRequest();

  QString request_command = QString(R"({"statements":[{"statement":")");

  if(projectName != "")
    request_command.append("MATCH (p:Project {name : '" + projectName + "'})<-[e:IN_PROJECT]-"
                                                                        "(t:Task {name:'" + taskName + "'}) DELETE e");
  else
    request_command.append("MATCH (t:Task {name:'" + taskName + "'}) DETACH DELETE t");


  request_command.append(R"("}]})");


  this->requestBuffer.push_back(qMakePair(*request, request_command));
  if (this->bufferDynamic == false)
    this->slotReadyWrite();

  delete request;
}

 void Network::addTask(const QString taskName, const QString projectName)
{
  QNetworkRequest *request = prebuildRequest();

  QString request_command = QString(R"({"statements":[{"statement":")");

  request_command.append("MATCH (p:Project {name:'" + projectName + "'}),"
                         "(t:Task {name:'" + taskName + "'}) MERGE (t)-[r:IN_PROJECT]->(p)");

  request_command.append(R"("}]})");


  this->requestBuffer.push_back(qMakePair(*request, request_command));
  if (this->bufferDynamic == false)
    this->slotReadyWrite();

  delete request;
}

void Network::createTask(const QString taskName, const QStringList& tasks)
{
  QNetworkRequest *request = prebuildRequest();

  QString request_command = QString(R"({"statements":[{"statement":")");

  request_command.append("Create (t:Task {name: '" + taskName + "'})");

  request_command.append(R"("}]})");


  this->requestBuffer.push_back(qMakePair(*request, request_command));
  if (this->bufferDynamic == false)
    this->slotReadyWrite();

  for (const QString& task:tasks)
    {
      this->addSubtask(task, taskName);
    }

  delete request;
}

void Network::unpackReply(QString param)
{
  if(!(this->unpackedReply.isEmpty()))
    this->unpackedReply.clear();

  if (!(this->replyBuffer.isEmpty()))
  {
    for (auto& iterator : this->replyBuffer.at(0).at("results")[0].at("data").items())
      {
        for(auto& it : iterator.key()){
            std::string substring = iterator.value().at("row")[0].at(param.toStdString().data()).dump();
            if (!(substring.empty()))
              this->unpackedReply.push_back(substring.substr(1, substring.size()  - 2).data());
          }
      }//for
    this->replyBuffer.removeFirst();
  }
}

QNetworkRequest* Network::prebuildRequest()
{
  QNetworkRequest *request = new QNetworkRequest(QUrl(this->serverAddress));

  request->setRawHeader ("Accept", "application/json; charset=UTF-8");
  request->setRawHeader ("Content-Type", "application/json");
  request->setRawHeader ("Authorization", "Basic "  + (this->user.login + ":" +this->user. password).toUtf8().toBase64());
  return request;
}

void Network::setServerAddress(QString newAddress)
{
  this->serverAddress = newAddress;
}

Network::~Network()
{
  for (auto reply:this->replyBuffer)
    reply.clear();
}

Network::Network(const Network &network) : serverAddress(network.serverAddress),
  requestBuffer(network.requestBuffer), replyBuffer(network.replyBuffer), unpackedReply(network.unpackedReply)
{
  connect(&(this->networkManager), &QNetworkAccessManager::finished, this, &Network::slotReadyRead);
  connect(&(this->networkManager), &QNetworkAccessManager::finished, this, &Network::slotReadyWrite);
}

QVector<QString> Network::returnReply()
{
  return std::move(this->unpackedReply);
}

void Network::createProject(const QString projectName,const QStringList &tasks)
{
  QNetworkRequest *request = prebuildRequest();

  QString request_command = QString(R"({"statements":[{"statement":")");

  request_command.append("Create (p:Project {name: '" + projectName + "'})");

  request_command.append(R"("}]})");


  this->requestBuffer.push_back(qMakePair(*request, request_command));
  if (this->bufferDynamic == false)
    this->slotReadyWrite();

  for (const QString& task:tasks)
    {
      this->addTask(task, projectName);
    }

  delete request;
}

void Network::setProjectName(QString projectName, QString newName)
{
  QNetworkRequest *request = prebuildRequest();

  QString request_command = QString(R"({"statements":[{"statement":")");

  request_command.append("MATCH (p:Project {name: '" + projectName + "'}) SET p.name = '" + newName + "'");

  request_command.append(R"("}]})");

  this->requestBuffer.push_back(qMakePair(*request, request_command));
  if (this->bufferDynamic == false)
    this->slotReadyWrite();

  delete request;
}

QDate Network::tasksDate()
{
  return QDate::fromString(this->replyBuffer.at(0).at("results")[0].at("data")[0].at("row")[0].at("date").dump()
      .substr(1, this->replyBuffer.at(0).at("results")[0].at("data")[0].at("row")[0].at("date").dump().size()  - 2).data(),
      Qt::ISODate);
}

QString Network::tasksTarget()
{
  return this->replyBuffer.at(0).at("results")[0].at("data")[0].at("row")[0].at("target").dump()
   .substr(1, this->replyBuffer.at(0).at("results")[0].at("data")[0].at("row")[0].at("target").dump().size()  - 2).data();
}

QString Network::tasksDescription()
{
  return this->replyBuffer.at(0).at("results")[0].at("data")[0].at("row")[0].at("description").dump()
   .substr(1, this->replyBuffer.at(0).at("results")[0].at("data")[0].at("row")[0].at("description").dump().size()  - 2).data();
}

QVector<QString> Network::tasksEmployee()
{
  QVector<QString> employee;
  for (auto& iterator : this->replyBuffer.at(0).at("results")[0].at("data").items())
    {
      for(auto& it : iterator.key()){
          std::string substring = iterator.value().at("row")[0].at("employee").dump();
          if (!(substring.empty()))
            employee.push_back(substring.substr(1, substring.size()  - 2).data());
        }
    }//for
  return employee;
}

void Network::tasksSubtasks(QString taskName)
{
  QNetworkRequest *request = prebuildRequest();

  QString request_command = QString(R"({"statements":[{"statement":")");

  if (taskName == "")
    request_command.append("MATCH (t1:Task) RETURN t1");
  else
    request_command.append("MATCH (t1:Task)-[:LINKED]-(t:Task {name: '" + taskName + "'}) RETURN t1");

  request_command.append(R"("}]})");

  this->requestBuffer.push_back(qMakePair(*request, request_command));
  if (this->bufferDynamic == false)
    this->slotReadyWrite();

  delete request;
}

void Network::getTaskData(QString taskName)
{
  QNetworkRequest *request = prebuildRequest();

  QString request_command = QString(R"({"statements":[{"statement":")");

  request_command.append("MATCH (t:Task {name: '" + taskName + "'}) RETURN t");

  request_command.append(R"("}]})");

  this->requestBuffer.push_back(qMakePair(*request, request_command));
  if (this->bufferDynamic == false)
    this->slotReadyWrite();

  delete request;
}

void Network::changeDate(QString task, QString newDate)
{
  qDebug() << task << " " << newDate;
  QNetworkRequest *request = prebuildRequest();

  QString request_command = QString(R"({"statements":[{"statement":")");

  request_command.append("MATCH (t:Task {name: '" + task + "'}) SET t.date = '" + newDate + "' RETURN t");

  request_command.append(R"("}]})");

  this->requestBuffer.push_back(qMakePair(*request, request_command));
  if (this->bufferDynamic == false)
    this->slotReadyWrite();

  delete request;
}

void Network::changeName(QString task, QString newName)
{
  QNetworkRequest *request = prebuildRequest();

  QString request_command = QString(R"({"statements":[{"statement":")");

  request_command.append("MATCH (t:Task {name: '" + task + "'}) SET t.name = '" + newName + "' RETURN t");

  request_command.append(R"("}]})");

  this->requestBuffer.push_back(qMakePair(*request, request_command));
  if (this->bufferDynamic == false)
    this->slotReadyWrite();

  delete request;
}

void Network::changeTarget(QString task, QString newTarget)
{
  QNetworkRequest *request = prebuildRequest();

  QString request_command = QString(R"({"statements":[{"statement":")");

  request_command.append("MATCH (t:Task {name: '" + task + "'}) SET t.target = '" + newTarget + "' RETURN t");

  request_command.append(R"("}]})");

  this->requestBuffer.push_back(qMakePair(*request, request_command));
  if (this->bufferDynamic == false)
    this->slotReadyWrite();

  delete request;
}

void Network::changeDescription(QString task, QString newDescription)
{
  QNetworkRequest *request = prebuildRequest();

  QString request_command = QString(R"({"statements":[{"statement":")");

  request_command.append("MATCH (t:Task {name: '" + task + "'}) SET t.description = '" + newDescription + "' RETURN t");

  request_command.append(R"("}]})");

  this->requestBuffer.push_back(qMakePair(*request, request_command));
  if (this->bufferDynamic == false)
    this->slotReadyWrite();

  delete request;
}

bool Network::emptyJson(nlohmann::json json)
{
  if (json.at("results")[0].at("data")[0].is_null())
    return false;
  else
    return true;
}

int Network::defineReply()
{
  QString replyType = this->replyBuffer.at(0).at("results")[0].at("columns")[0].dump().
      substr(1, this->replyBuffer.at(0).at("results")[0].at("columns")[0].dump().size()- 2).data();

  if (replyType == "e")
    return 0;
  else
    if (replyType == "t1")
      return 2;
  else
      if (replyType == "pt")
          return 3;
  else
        return 1;
}

void Network::getEmployee(QString taskName)
{
  QNetworkRequest *request = prebuildRequest();

  QString request_command = QString(R"({"statements":[{"statement":")");

  if (taskName == "")
    request_command.append("MATCH (e:Employee) RETURN e");
  else
    request_command.append("MATCH (e:Employee)-[:WORKS_AT]->(:Task {name:'" + taskName + "'}) RETURN e");

  request_command.append(R"("}]})");

  this->requestBuffer.push_back(qMakePair(*request, request_command));
  if (this->bufferDynamic == false)
    this->slotReadyWrite();

  delete request;
}

void Network::deleteEmployee(QString employee, QString taskName)
{
  QNetworkRequest *request = prebuildRequest();

  QString request_command = QString(R"({"statements":[{"statement":")");

  request_command.append("MATCH (t:Task {name : '" + taskName + "'})<-[r:WORKS_AT]-"
                                                                        "(e:Employee {name:'" + employee + "'}) DELETE r");
  request_command.append(R"("}]})");


  this->requestBuffer.push_back(qMakePair(*request, request_command));
  if (this->bufferDynamic == false)
    this->slotReadyWrite();

  delete request;
}

void Network::addEmployee(QString employee, QString taskName)
{
  QNetworkRequest *request = prebuildRequest();

  QString request_command = QString(R"({"statements":[{"statement":")");

  request_command.append("MATCH (t:Task {name:'" + taskName + "'}),"
                         "(e:Employee {name:'" + employee + "'}) MERGE (e)-[r:WORKS_AT]->(t)");

  request_command.append(R"("}]})");


  this->requestBuffer.push_back(qMakePair(*request, request_command));
  if (this->bufferDynamic == false)
    this->slotReadyWrite();

  delete request;
}

void Network::addSubtask(QString subtaskName, QString taskName)
{
  QNetworkRequest *request = prebuildRequest();

  QString request_command = QString(R"({"statements":[{"statement":")");

  request_command.append("MATCH (t:Task {name:'" + taskName + "'}),"
                         "(t1:Task {name:'" + subtaskName + "'}) MERGE (t1)-[:LINKED]-(t)");

  request_command.append(R"("}]})");


  this->requestBuffer.push_back(qMakePair(*request, request_command));
  if (this->bufferDynamic == false)
    this->slotReadyWrite();

  delete request;
}

void Network::deleteSubtask(QString subtaskName, QString taskName)
{
  QNetworkRequest *request = prebuildRequest();

  QString request_command = QString(R"({"statements":[{"statement":")");

  request_command.append("MATCH (t:Task {name:'" + taskName + "'})-[r:LINKED]-"
                         "(t1:Task {name:'" + subtaskName + "'}) DELETE r");

  request_command.append(R"("}]})");


  this->requestBuffer.push_back(qMakePair(*request, request_command));
  if (this->bufferDynamic == false)
    this->slotReadyWrite();

  delete request;
}

void Network::login()
{
  QNetworkRequest *request = new QNetworkRequest(QUrl(this->serverAddress));

  request->setRawHeader ("Accept", "application/json; charset=UTF-8");
  request->setRawHeader ("Authorization", "Basic "  + (this->user.login + ":" + this->user.password).toUtf8().toBase64());

  QNetworkReply *reply = this->networkManager.get(*request);

  QMessageBox *error = &(this->error);

  connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), [&, error, reply](QNetworkReply::NetworkError code)
  {
      qDebug() << code;

  if (code != QNetworkReply::NoError && code != QNetworkReply::ContentOperationNotPermittedError)
    {
      if (reply->isOpen())
        reply->close();

      error->setText("Server error");
      if (!(error->button(QMessageBox::Ok)))
        error->addButton(QMessageBox::Ok);
      error->show();
    }
  });

  delete request;
}

void Network::slotAuth(QNetworkReply *reply)
{
  if (reply->isReadable())
    {
      QString repl = reply->readAll();
      qDebug( )<<  repl;
      if (reply != nullptr && repl == "")
        {
          disconnect(&(this->networkManager), &QNetworkAccessManager::finished, this, &Network::slotAuth);
          connect(&(this->networkManager), &QNetworkAccessManager::finished, this, &Network::slotReadyRead);
          connect(&(this->networkManager), &QNetworkAccessManager::finished, this, &Network::slotReadyWrite);
          this->bufferDynamic = false;
          emit this->loggedIn();
        }
      else
        this->loginError(reply->error());
    }
  reply->deleteLater();
}

void Network::personalTasks()
{
  QNetworkRequest *request = prebuildRequest();

  QString request_command = QString(R"({"statements":[{"statement":")");

  request_command.append("MATCH (pt:Task)<-[:WORKS_AT]-(e:Employee {name:'" + this->user.login + "'}) RETURN pt");

  request_command.append(R"("}]})");

  this->requestBuffer.push_back(qMakePair(*request, request_command));
  if (this->bufferDynamic == false)
    this->slotReadyWrite();

  delete request;
}

QString Network::returnPersonalTasks()
{
  QString reply = "Current tasks:\n";
  QStringList deadlines;
  QStringList tasks;

  if (!(this->replyBuffer.isEmpty()))
    {
      if (!this->replyBuffer.at(0).at("results")[0].at("data")[0].is_null())
      {
        for (auto& iterator : this->replyBuffer.at(0).at("results")[0].at("data").items())
          {
            for(auto& it : iterator.key()){
                std::string substring = iterator.value().at("row")[0].at("date").dump();
                if (!(substring.empty()))
                  deadlines.push_back(substring.substr(1, substring.size()  - 2).data());
              }
          }//for

        this->unpackReply("name");
        tasks = this->returnReply().toList();
        }
    }

  return reply;
}

void Network::loginError(QNetworkReply::NetworkError code)
{qDebug() << code;
  if (code != QNetworkReply::NoError && code != QNetworkReply::ContentOperationNotPermittedError)
    {
      this->error.setText("Server error");
      if (this->error.button(QMessageBox::Ok))
        this->error.addButton(QMessageBox::Ok);
      this->error.show();
    }
}
