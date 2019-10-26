#include "../includes/network.h"
#include "../includes/task.h"

Network::Network()
{
  this->_manager = new QNetworkAccessManager(this);
  connect(_manager, &QNetworkAccessManager::finished, this, &Network::slotReadyRead);
}

void Network::doRequest(QString string)
{
  QNetworkRequest *request = new QNetworkRequest(QUrl("http://localhost:7474/db/data/transaction/commit"));

  request->setRawHeader ("Accept", "application/json; charset=UTF-8");
  request->setRawHeader ("Content-Type", "application/json");
  request->setRawHeader ("Authorization", "Basic dGVzdDp0ZXN0");

  QString *request_command = new QString(R"({"statements":[{"statement":")");
  request_command->append(string);
  request_command->append(R"("}]})");

  this->_manager->post(*request, request_command->toUtf8());

  delete request;
  delete request_command;
}

void Network::slotReadyRead(QNetworkReply *get_reply)
{
  QByteArray *bytes = new QByteArray(get_reply->readAll());

  this->_reply = nlohmann::json::parse(*bytes);

  this->_wait.exit();

  delete bytes;
}

std::vector<std::string> Network::getReply(QString param)
{
  std::vector<std::string> temp;
    for (auto& iterator : this->_reply.at("results")[0].at("data").items()) {
        for(auto& it : iterator.key()){
             temp.push_back(iterator.value().at("row")[0].at(param.toStdString().data()).dump());
        }
    }
    return temp;
}

void Network::authorize(QString login, QString password)
{
  QNetworkRequest *request = new QNetworkRequest(QUrl("http://localhost:7474/user/" + login));

  request->setRawHeader ("Accept", "application/json; charset=UTF-8");
  request->setRawHeader ("Authorization", "Basic " + QString(login + ":" + password).toUtf8().toBase64());

  this->_manager->get(*request);

  delete request;
}

Network::~Network()
{
  delete this->_manager;
}
