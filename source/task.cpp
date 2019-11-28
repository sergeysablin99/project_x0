#include "../include/task.h"
#include "../include/network.h"

Task::Task()
{
  this->network = nullptr;
}

Task::Task(QString name, Network* network) : network(network), name(name)
{
  this->setWindowTitle(this->name);
  this->setMinimumSize(400, 250);

  connect(&(this->editButton), &QPushButton::clicked, this, &Task::SlotEditButton);
  connect(this->network, &Network::readFinished, this, &Task::getReply);

  //Элементы для изменения задачи
  this->dateEdit.setReadOnly(false);
  this->dateEdit.setVisible(false);

  this->targetEdit.setPlaceholderText("Input your target");
  this->targetEdit.setVisible(false);
  this->targetEdit.setReadOnly(false);

  this->nameEdit.setPlaceholderText("Input new name");
  this->nameEdit.setHidden(true);
  this->nameEdit.setReadOnly(false);

  this->descriptionEdit.setPlaceholderText("Text edit");
  this->descriptionEdit.setVisible(false);
  this->descriptionEdit.setReadOnly(false);

  // Получить из базы данных цель задачи

  this->target.setMaximumWidth(100);
  this->target.setBaseSize(30, 20);

  // Получить из базы данных описание задачи
  this->description.setReadOnly(true);
  this->description.setMaximumSize(200, 70);
  this->description.setBaseSize(200, 50);

  this->editButton.setText("Edit");

  //Добавим виджеты на экран
  this->layout.setSpacing(10);
  this->layout.addWidget(&(this->target), 0, 0, Qt::AlignLeft);
  this->layout.addWidget(&(this->date), 0, 1, Qt::AlignLeft);
  this->layout.addWidget(&(this->description), 1, 0, Qt::AlignLeft);
  this->layout.addWidget(&(this->employee));
  this->layout.addWidget(&(this->subtask));
  this->layout.addWidget(&(this->nameEdit));
  this->layout.addWidget(&(this->editButton), 3, 2, Qt::AlignLeft);
  this->layout.addWidget(&(this->targetEdit), 0, 1, Qt::AlignLeft);
  this->layout.addWidget(&(this->descriptionEdit), 1, 0, Qt::AlignLeft);
  this->layout.addWidget(&(this->dateEdit), 0, 2, Qt::AlignLeft);

  this->setLayout(&(this->layout));

  this->network->getTaskData(this->name);
}


void Task::SlotEditButton(){
//TODO: кликабельный список исполнителей и подзадач, чтобы применялось и сохранялось в БД
  if (this->editButton.text() == "Edit")  {
      this->description.setVisible(false);

      this->descriptionEdit.setVisible(true);
      this->targetEdit.setVisible(true);
      this->dateEdit.setVisible(true);
      this->nameEdit.setVisible(true);

      this->subtask.setVisible(false);
      this->employee.setVisible(false);
      this->editButton.setText("Save");
    } else {
      if (this->descriptionEdit.toPlainText() != "" && this->descriptionEdit.toPlainText() != this->description.toPlainText())
        {
          this->network->changeDescription(this->name, this->descriptionEdit.toPlainText());
        }
      if (this->targetEdit.text() != "" && this->targetEdit.text() != this->target.text())
        {
          this->network->changeTarget(this->name, this->targetEdit.text());
        }

      if (this->dateEdit.date() != QDate::fromString(this->date.text(), Qt::ISODate) && this->dateEdit.text() != "")
      {
          this->setDate(this->dateEdit.date());
          this->network->changeDate(this->name, this->date.text()); // крашится на этом моменте
      }

      if (this->nameEdit.text() != "" && this->nameEdit.text() != this->name)
        {
          this->network->changeName(this->name, this->nameEdit.text());
          this->name = this->nameEdit.text();
        }


       this->description.setVisible(true);

       this->descriptionEdit.setVisible(false);
       this->targetEdit.setVisible(false);
       this->dateEdit.setVisible(false);
       this->nameEdit.setVisible(false);

       this->subtask.setVisible(true);
       this->employee.setVisible(true);
       this->editButton.setText("Edit");
    }//else
}

Task::~Task()
{
  if (this->network != nullptr)
    delete this->network;
}

void Task::getReply()
{
  this->setDate(this->network->tasksDate());
  this->setTarget(this->network->tasksTarget());
  this->setDescription(this->network->tasksDescription());

  this->employee.clear();
  this->setEmployee(this->network->tasksEmployee());

  this->subtask.clear();
  this->setSubtask(this->network->tasksSubtasks());
  this->network->unpackReply("name");
}

  void Task::setName(QString newName)
  {
    this->name = newName;
  }

  void Task::setDate(QDate newDate)
  {
    this->date.setText(newDate.toString(Qt::ISODate));
    this->dateEdit.setDate(newDate);
  }

  void Task::setDescription(QString newDescription)
  {
    this->description.setPlainText(newDescription);
  }

  void Task::setEmployee(QVector<QString> employees)
  {
    if (!(employees.isEmpty()))
      this->employee.addItems(employees.toList());
  }

  void Task::setSubtask(QVector<QString> subtasks)
  {
    if (!(subtasks.isEmpty()))
      this->subtask.addItems(subtasks.toList());
  }

  void Task::setNetwork(Network* network)
  {
    if (this->network != nullptr)
      delete this->network;
    this->network = network;
  }

  void Task::setTarget(QString newTarget)
  {
    this->target.setText(newTarget);
  }
