#include "../includes/Project.h"

Project::Project()
{
  this->_network_manager = nullptr;
  this->_task = nullptr;
}

Project::Project(QString name)
{
  connect(&(this->_tasks), &QListWidget::itemActivated, this, &Project::openTask);

  this->_network_manager = new Network;
  this->_task = nullptr;

  // Получить из базы данных имя задачи
  _network_manager->doRequest("MATCH (n:Project {name:'" + name + "'}) RETURN n");
  this->_network_manager->_wait.exec(); //Ждём, пока придёт ответ от сервера
  this->_name = _network_manager->getReply("name")[0].data();
  this->_name.remove(0,1).remove(this->_name.size() - 1, 1);

  //Вектор для считывания списков
  std::vector<std::string> *replyData = new std::vector<std::string>;

  //получить список задач
  this->_network_manager->doRequest("MATCH (n:Task)-[:IN_PROJECT]->(m:Project {name:'" + this->_name + "'}) RETURN n");
  this->_network_manager->_wait.exec();

  *replyData = this->_network_manager->getReply("name");
  std::for_each(replyData->begin(),
                replyData->end(), [this](std::string value)
  {
    this->_tasks.addItem(value.data());
  });

  delete replyData;

  //Добавим виджеты на экран
  this->_layout.addWidget(&(this->_tasks));
  this->setLayout(&(this->_layout));
}

void Project::setName(QString name) //TODO переделать в кнопку изменения параметров проекта
{

  _network_manager->doRequest ("MATCH (n:Project {name:'" + this->_name + "'})"
                                       " SET n.name = '" + name + "' RETURN n");

  this->_network_manager->_wait.exec(); //Ждём, пока придёт ответ от сервера

  this->_name = name;
}

void Project::addTask(QString taskName)
{
  _network_manager->doRequest ("MATCH (n:Project {name:'" + this->_name + "'}),"
                                       "(m:Task {name:'" + taskName + "'}) MERGE (n)<-[:IN_PROJECT]-(m) RETURN n");

  this->_network_manager->_wait.exec(); //Ждём, пока придёт ответ от сервера
  this->_tasks.addItem(taskName);
}

void Project::deleteTask(QString taskName)
{
   _network_manager->doRequest ("MATCH (n:Project {name:'" + this->_name + "'})<-[r:IN_PROJECT]-"
                                                "(m:Task {name:'" + taskName + "'}) DELETE r");

  this->_network_manager->_wait.exec(); //Ждём, пока придёт ответ от сервера
  this->_tasks.removeItemWidget(this->_tasks.findItems(taskName, Qt::MatchExactly).first());
}

void Project::openTask (QListWidgetItem *item)
{
  QString *substr = new QString(item->text());
  substr->remove(0,1).remove(substr->size() - 1, 1);

  _network_manager->doRequest ("MATCH (n:Task {name:'" + *substr + "'}) RETURN n");
  this->_network_manager->_wait.exec(); //Ждём, пока придёт ответ от сервера

  if (this->_task != nullptr)
//    delete this->_task;

  delete substr;

  substr = new QString(this->_network_manager->getReply("name")[0].data());
  substr->remove(0,1).remove(substr->size() - 1, 1);
  this->_task = new  Task(*substr);

  this->close();
  this->_task->show();

  delete substr;
}

Project::~Project()
{
  if (this->_network_manager != nullptr)
    delete this->_network_manager;
  if (this->_task != nullptr)
    delete this->_task;
}
