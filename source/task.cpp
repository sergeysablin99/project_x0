#include "../include/task.h"
#include "../include/network.h"

Task::Task(QWidget* parent): QWidget(parent)
{
  this->network = nullptr;
}

Task::Task(QWidget* parent, QString name, Network* network) : QWidget(parent), network(network), name(name)
{
  this->setWindowTitle(this->name);
  this->setMinimumSize(400, 250);

  connect(&(this->editButton), &QPushButton::clicked, this, &Task::SlotEditButton);
  connect(this->network, &Network::readFinished, this, &Task::getReply);
  connect(this->network, &Network::returnEmployee, this, &Task::returnEmployee);
  connect(this->network, &Network::returnSubtasks, this, &Task::returnSubtasks);
  connect(&(this->BBack), &QPushButton::clicked, this, &Task::back);

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
  this->BBack.setText("Back");

  this->GBECheckBox.setHidden(true);
  this->GBTCheckBox.setHidden(true);

  //Добавим виджеты на экран
  this->layout.setSpacing(10);
  this->layout.addWidget(&(this->target), 0, 0, Qt::AlignLeft);
  this->layout.addWidget(&(this->date), 0, 1, Qt::AlignLeft);
  this->layout.addWidget(&(this->description), 1, 0, Qt::AlignLeft);
  this->layout.addWidget(&(this->employee));
  this->layout.addWidget(&(this->subtask));
  this->layout.addWidget(&(this->nameEdit));
  this->layout.addWidget(&(this->BBack));
  this->layout.addWidget(&(this->GBECheckBox));
  this->layout.addWidget(&(this->GBTCheckBox));
  this->layout.addWidget(&(this->editButton), 3, 2, Qt::AlignLeft);
  this->layout.addWidget(&(this->targetEdit), 0, 1, Qt::AlignLeft);
  this->layout.addWidget(&(this->descriptionEdit), 1, 0, Qt::AlignLeft);
  this->layout.addWidget(&(this->dateEdit), 0, 2, Qt::AlignLeft);

  this->setLayout(&(this->layout));

  this->network->getEmployee(this->name);
  this->network->tasksSubtasks(this->name);
  this->network->getTaskData(this->name);
}

void Task::SlotEditButton()
{
  //TODO: кликабельный список исполнителей и подзадач, чтобы применялось и сохранялось в БД
  if (this->editButton.text() == "Edit")  {
      this->description.setVisible(false);

      this->descriptionEdit.setVisible(true);
      this->targetEdit.setVisible(true);
      this->dateEdit.setVisible(true);
      this->nameEdit.setVisible(true);
      this->GBECheckBox.setVisible(true);
      this->GBTCheckBox.setVisible(true);

      this->BBack.setVisible(false);
      this->target.setVisible(false);
      this->subtask.setVisible(false);
      this->employee.setVisible(false);
      this->editButton.setText("Save");

      this->network->getEmployee();
      this->network->tasksSubtasks();
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

      for (auto employee:this->VECheckBox){
          if (employee->isChecked())
            this->network->addEmployee(employee->text(), this->name);
          else
            {
              this->network->deleteEmployee(employee->text(), this->name);
            }
        }

      for (auto subtask:this->VTCheckBox){
          if (subtask->isChecked())
            this->network->addSubtask(subtask->text(), this->name);
          else
            {
              this->network->deleteSubtask(subtask->text(), this->name);
            }
        }

      this->description.setVisible(true);

      this->descriptionEdit.setVisible(false);
      this->targetEdit.setVisible(false);
      this->dateEdit.setVisible(false);
      this->nameEdit.setVisible(false);
      this->GBECheckBox.setVisible(false);
      this->GBTCheckBox.setVisible(false);

      this->target.setVisible(true);
      this->subtask.setVisible(true);
      this->employee.setVisible(true);
      this->BBack.setVisible(true);
      this->editButton.setText("Edit");

      this->network->getEmployee(this->name);
      this->network->tasksSubtasks(this->name);
      this->network->getTaskData(this->name);
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

  this->network->unpackReply("name");
  this->network->returnReply();
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

void Task::back()
{
  qDebug() << "task back";
  disconnect(this->network, &Network::readFinished, this, &Task::getReply);
  disconnect(this->network, &Network::returnEmployee, this, &Task::returnEmployee);
  disconnect(this->network, &Network::returnSubtasks, this, &Task::returnSubtasks);
  this->close();
  emit this->showProject();
}

void Task::returnEmployee()
{
qDebug() << "1";
    this->network->unpackReply("name");
    QStringList checkList = this->network->returnReply().toList();

    for (auto item:VECheckBox)
      delete item;
    this->VECheckBox.clear();

    QLayoutItem *child;
    while ((child = this->LECheckBox.takeAt(0)) != nullptr) {
        delete child;
      }

    for (int taskIndex = 0; taskIndex < checkList.count(); taskIndex++)
      {
        QCheckBox *newObject = new QCheckBox(checkList.at(taskIndex));

        for (int item = 0; item < this->employee.count(); item++)
          if (this->employee.item(item)->text() == checkList.at(taskIndex))
            {
              newObject->setCheckState(Qt::Checked);
              break;
            }

        this->VECheckBox.push_back(newObject);
        this->LECheckBox.addWidget(this->VECheckBox[this->VECheckBox.indexOf(newObject)]);
      }

    this->employee.clear();
    this->employee.addItems(checkList);

    if (this->LEGroupBox.indexOf(&(this->LECheckBox)) == -1)
      this->LEGroupBox.addLayout(&(this->LECheckBox));
    if (this->GBECheckBox.layout() == nullptr)
      this->GBECheckBox.setLayout(&(this->LEGroupBox));
}

void Task::returnSubtasks()
{
  qDebug() << '2';
  this->network->unpackReply("name");
  QStringList checkList = this->network->returnReply().toList();

  for (auto item:VTCheckBox)
    delete item;
  this->VTCheckBox.clear();

  QLayoutItem *child;
  while ((child = this->LTCheckBox.takeAt(0)) != nullptr) {
      delete child;
    }

  for (int taskIndex = 0; taskIndex < checkList.count(); taskIndex++)
    {
      if (checkList.at(taskIndex) == this->name)
        continue;
      QCheckBox *newObject = new QCheckBox(checkList.at(taskIndex));

      for (int item = 0; item < this->subtask.count(); item++)
        if (this->subtask.item(item)->text() == checkList.at(taskIndex))
          {
            newObject->setCheckState(Qt::Checked);
            break;
          }

      this->VTCheckBox.push_back(newObject);
      this->LTCheckBox.addWidget(this->VTCheckBox[this->VTCheckBox.indexOf(newObject)]);
    }

  this->subtask.clear();
  this->subtask.addItems(checkList);

  if (this->LTGroupBox.indexOf(&(this->LTCheckBox)) == -1)
    this->LTGroupBox.addLayout(&(this->LTCheckBox));
  if (this->GBTCheckBox.layout() == nullptr)
    this->GBTCheckBox.setLayout(&(this->LTGroupBox));
}

QListWidget* Task::getSubtasks()
{
  return &(this->subtask);
}
