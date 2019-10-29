#include "includes/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent):
    QWidget(parent)
{
    this->_project = new Project;

    connect(&(this->_projectList), &QListWidget::itemActivated, this, &MainWindow::openProject);

    this->_network_manager = new Network; //TODO проекты и задачи пользуются некторком главного окна, а не собственными

    this->_layout.addWidget(&(this->_projectList));
    this->setLayout(&(this->_layout));

    //Вектор для считывания списков
    std::vector<std::string> *replyData = new std::vector<std::string>;

    // Получить из базы данных исполнителей
    this->_network_manager->doRequest("MATCH (n:Project) RETURN n");
    this->_network_manager->_wait.exec(); //Ждём, пока придёт ответ от сервера

    *replyData = this->_network_manager->getReply("name");

    std::for_each(replyData->begin(),
                replyData->end(), [this](std::string value)
    {
      this->_projectList.addItem(value.data());
    });

    delete replyData;
}

void MainWindow::openProject (QListWidgetItem *item)
{
  QString *substr = new QString(item->text());
  substr->remove(0,1).remove(substr->size() - 1, 1);

  _network_manager->doRequest ("MATCH (n:Project {name:'" + *substr + "'}) RETURN n");
  this->_network_manager->_wait.exec(); //Ждём, пока придёт ответ от сервера

  if (this->_project != nullptr)
    delete this->_project;

  delete substr;

  substr = new QString(this->_network_manager->getReply("name")[0].data());
  substr->remove(0,1).remove(substr->size() - 1, 1);
  this->_project = new Project(*substr);

  this->close();
  this->_project->show();

  delete substr;
}

MainWindow::~MainWindow()
{
  if (this->_project != nullptr)
    delete this->_project;
  if (this->_network_manager != nullptr)
    delete this->_network_manager;
}
