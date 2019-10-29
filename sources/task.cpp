#include "../includes/task.h"
#include "../includes/network.h"

Task::Task()
{
  this->_network_manager = nullptr;
}

Task::Task(QString name)
{
  this->_network_manager = new Network;

  // Получить из базы данных имя задачи
  _network_manager->doRequest("MATCH (n:Task {name:'" + name + "'}) RETURN n");
  this->_network_manager->_wait.exec(); //Ждём, пока придёт ответ от сервера
  this->_name = _network_manager->getReply("name")[0].data();
  this->_name.remove(0,1).remove(this->_name.size() - 1, 1);

  // Получить из базы данных дедлайн задачи
  this->_date.setText(_network_manager->getReply("date")[0].data());

  this->setWindowTitle(this->_name);
  this->setMinimumSize(400, 250);

  connect(&(this->_editButton), &QPushButton::clicked, this, &Task::SlotEditButton);

  //Элементы для изменения задачи
  this->_dateEdit.setReadOnly(false);
  this->_dateEdit.setVisible(false);

  this->_targetEdit.setPlaceholderText("Input your target");
  this->_targetEdit.setVisible(false);
  this->_targetEdit.setReadOnly(false);

  this->_descriptionEdit.setPlaceholderText("Text edit");
  this->_descriptionEdit.setVisible(false);
  this->_descriptionEdit.setReadOnly(false);

  // Получить из базы данных цель задачи
  this->_target.setText(_network_manager->getReply("target")[0].data());
  this->_target.setMaximumWidth(100);
  this->_target.setBaseSize(30, 20);

  // Получить из базы данных описание задачи
  this->_description.setReadOnly(true);
  this->_description.setPlainText(_network_manager->getReply("description")[0].data());
  this->_description.setMaximumSize(200, 70);
  this->_description.setBaseSize(200, 50);

  this->_editButton.setText("Edit");

  //Добавим виджеты на экран
  this->_layout.setSpacing(10);
  this->_layout.addWidget(&(this->_target), 0, 0, Qt::AlignLeft);
  this->_layout.addWidget(&(this->_date), 0, 1, Qt::AlignLeft);
  this->_layout.addWidget(&(this->_description), 1, 0, Qt::AlignLeft);
  this->_layout.addWidget(&(this->_employee));
  this->_layout.addWidget(&(this->_subtask));
  this->_layout.addWidget(&(this->_editButton), 3, 2, Qt::AlignLeft);
  this->_layout.addWidget(&(this->_targetEdit), 0, 1, Qt::AlignLeft);
  this->_layout.addWidget(&(this->_descriptionEdit), 1, 0, Qt::AlignLeft);
  this->_layout.addWidget(&(this->_dateEdit), 0, 2, Qt::AlignLeft);

  this->setLayout(&(this->_layout));

  //Вектор для считывания списков
  std::vector<std::string> *replyData = new std::vector<std::string>;

  // Получить из базы данных исполнителей
  this->_network_manager->doRequest("MATCH (n:Employee)-[:WORKS_AT]->(m:Task {name:'" + this->_name + "'}) RETURN n");
  this->_network_manager->_wait.exec(); //Ждём, пока придёт ответ от сервера

  *replyData = this->_network_manager->getReply("name");

  std::for_each(replyData->begin(),
                replyData->end(), [this](std::string value)
  {
    this->_employee.addItem(value.data());
  });

  //Получим из базы данных список задач
  this->_network_manager->doRequest("MATCH (n:Task)-[:LINKED_WITH]-(m:Task {name:'" + this->_name + "'}) RETURN n");
  this->_network_manager->_wait.exec();
  *replyData = this->_network_manager->getReply("name");
  std::for_each(replyData->begin(),
                replyData->end(), [this](std::string value)
  {
    this->_subtask.addItem(value.data());
  });
  delete replyData;
}


void Task::SlotEditButton(){
//TODO: кликабельный список исполнителей и подзадач, чтобы применялось и сохранялось в БД
  if (this->_editButton.text() == "Edit")  {
      this->_description.setVisible(false);

      this->_descriptionEdit.setVisible(true);
      this->_targetEdit.setVisible(true);
      this->_dateEdit.setVisible(true);

      this->_subtask.setVisible(false);
      this->_employee.setVisible(false);
      this->_editButton.setText("Save");
    } else {
      if (this->_descriptionEdit.toPlainText () != "")
      {
          this->_description.setPlainText(this->_descriptionEdit.toPlainText());
          _network_manager->doRequest ("MATCH (n:Task {name:'" + this->_name + "'})"
                                       " SET n.description = '" + this->_descriptionEdit.toPlainText() + "' RETURN n");

          this->_network_manager->_wait.exec(); //Ждём, пока придёт ответ от сервера
        }
      if (this->_targetEdit.text() != "")
      {
        this->_target.setText(this->_targetEdit.text());
       _network_manager->doRequest ("MATCH (n:Task {name:'" + this->_name + "'})"
                                       " SET n.target = '" + this->_targetEdit.text() + "' RETURN n") ;

       this->_network_manager->_wait.exec(); //Ждём, пока придёт ответ от сервера
      }
      this->_date.setText(this->_dateEdit.text());
       _network_manager->doRequest ("MATCH (n:Task {name:'" + this->_name + "'})"
                                       " SET n.date = '" + this->_dateEdit.text() + "' RETURN n");

       this->_network_manager->_wait.exec(); //Ждём, пока придёт ответ от сервера

       this->_description.setVisible(true);

       this->_descriptionEdit.setVisible(false);
       this->_targetEdit.setVisible(false);
       this->_dateEdit.setVisible(false);

       this->_subtask.setVisible(true);
       this->_employee.setVisible(true);
       this->_editButton.setText("Edit");
    }//else
}

Task::~Task()
{
  if (this->_network_manager != nullptr)
    delete this->_network_manager;
}
