#include "../include/Project.h"

Project::Project(QWidget* parent) : QWidget(parent), network(nullptr), task(nullptr)
{
}

Project::Project(QWidget* parent, QString name, Network* manager): QWidget(parent), name(name), network(manager), task(nullptr)
{
  connect(&(this->taskList), &QListWidget::itemActivated, this, &Project::openTask);
  connect(&(this->editButton), &QPushButton::clicked, this, &Project::edit);
  connect(this->network, &Network::readFinished, this, &Project::getReply);
  connect(this->network, &Network::returnEmployee, this, &Project::getEmployee);
  connect(&(this->BBack), &QPushButton::clicked, this, &Project::back);
  connect(&(this->BCreateTask), &QPushButton::clicked, this, &Project::createTask);

  this->editButton.setText("Edit project");
  this->BBack.setText("Back");

  this->BCreateTask.setText("Create new task");
  this->BCreateTask.setHidden(false);

  this->setName.setHidden(true);
  this->setName.setPlaceholderText(this->name);

  this->taskLabel.setText("Task list");

  this->taskList.setBaseSize(100, 100);

  this->tasksLabel.setText("Tasks");
  this->employeeLabel.setText("Employees");

  this->GBCheckBox.setHidden(true);


  this->LButtons.addWidget(&(this->editButton));
  this->LButtons.addWidget(&(this->BBack));
  this->LButtons.addWidget(&(this->BCreateTask));

  this->layout.addWidget(&(this->GBCheckBox));
  this->layout.addWidget(&(this->taskLabel));
  this->layout.addWidget(&(this->taskList));
  this->layout.addLayout(&(this->LButtons));

  this->setLayout(&(this->layout));

  this->showMainPage();
}

void Project::edit()
{
  if (this->editButton.text() == "Edit project"){
      this->editButton.setText("Save");

      this->hideAll();
      this->setName.clear();
      this->setName.setVisible(true);
      this->editButton.setVisible(true);
      this->GBCheckBox.setVisible(true);
      this->LGroupBox.insertWidget(0, &(this->setName));

      if (this->LCheckBox.indexOf(&(this->LVCheckBox)) == -1)
        this->LCheckBox.addLayout(&(this->LVCheckBox));

      this->LGroupBox.insertLayout(1, &(this->LCheckBox));

      this->setName.setPlaceholderText(this->name);

      this->network->getTasks();
    } else {
      if (!this->setName.text().isEmpty())
        {
          this->network->setProjectName(this->name, this->setName.text());
          this->name = this->setName.text();
        }

      for (auto task:this->VCheckBox)
        {
          if (!this->checkedTasks.contains(task->text()) && task->isChecked())
            this->network->addTask(task->text(), this->name);
          else
            if (this->checkedTasks.contains(task->text()) && !task->isChecked())
              this->network->deleteTask(task->text(), this->name);
        }

      this->setName.setHidden(true);
      this->showMainPage();
    }
}

void Project::openTask (QListWidgetItem *taskName)
{
  disconnect(this->network, &Network::readFinished, this, &Project::getReply);
  disconnect(this->network, &Network::returnEmployee, this, &Project::getEmployee);
  this->hideAll();

  this->task = new Task(this, taskName->text(), this->network);
  connect(this->task, &Task::showProject, this, &Project::taskClosed);
  connect(this->task->getSubtasks(), &QListWidget::itemDoubleClicked, this->task, &Task::back);
  connect(this->task->getSubtasks(), &QListWidget::itemDoubleClicked, this, &Project::openTask);

  this->layout.addWidget(this->task);
  this->task->show();
}

Project::~Project()
{
  if (this->task != nullptr)
    delete this->task;
}

void Project::getReply()
{
  this->network->unpackReply("name");
  QStringList checkList = this->network->returnReply().toList();

  if (this->layout.indexOf(&(this->taskList)) == -1)
    {
      this->layout.addWidget(&(this->taskList));
    }

  for (auto item:VCheckBox)
    delete item;
  this->VCheckBox.clear();

  QLayoutItem *child;
  while ((child = this->LVCheckBox.takeAt(0)) != nullptr) {
      delete child;
    }

  for (int taskIndex = 0; taskIndex < checkList.count(); taskIndex++)
    {
      QCheckBox *newObject = new QCheckBox(checkList.at(taskIndex));

      for (int task = 0; task < this->taskList.count(); task++)
        if (this->taskList.item(task)->text() == checkList.at(taskIndex))
          {
            newObject->setCheckState(Qt::Checked);
            this->checkedTasks.append(newObject->text());
            break;
          }

      this->VCheckBox.push_back(newObject);
      this->LVCheckBox.addWidget(this->VCheckBox[this->VCheckBox.indexOf(newObject)]);
    }

  this->taskList.clear();
  this->taskList.addItems(checkList);

  if (this->LVCheckBox.indexOf(&(this->tasksLabel)) == -1)
    this->LVCheckBox.insertWidget(0, &(this->tasksLabel));
  if (this->LGroupBox.indexOf(&(this->LCheckBox)) == -1)
    this->LGroupBox.addLayout(&(this->LCheckBox));
  if (this->GBCheckBox.layout() == nullptr)
    this->GBCheckBox.setLayout(&(this->LGroupBox));
}

void Project::hideAll()
{
  for (int counter = 0; counter < this->layout.count(); counter++)
    {
      if (this->layout.itemAt(counter)->widget())
        this->layout.itemAt(counter)->widget()->setHidden(true);
    }
    for (int counter = 0; counter < this->LButtons.count(); counter++)
    {
        if(this->LButtons.itemAt(counter)->widget())
          this->LButtons.itemAt(counter)->widget()->setHidden(true);
    }
}

void Project::showMainPage()
{

  this->hideAll();

  this->editButton.setText("Edit project");

  if (this->taskList.isHidden())
    this->taskList.setVisible(true);
  if (this->editButton.isHidden())
    this->editButton.setVisible(true);
  if (this->BBack.isHidden())
    this->BBack.setVisible(true);
  if (this->BCreateTask.isHidden())
    this->BCreateTask.setVisible(true);
  if (this->taskLabel.isHidden())
    this->taskLabel.setVisible(true);

  this->network->getTasks(this->name);
}

void Project::taskClosed()
{
  connect(this->network, &Network::readFinished, this, &Project::getReply);
  connect(this->network, &Network::returnEmployee, this, &Project::getEmployee);
  this->showMainPage();
}

void Project::back()
{
  disconnect(this->network, &Network::readFinished, this, &Project::getReply);
  this->close();
  emit this->showMainWindow();
}

Project::Project(const Project& copy): name(copy.name), network(copy.network), task(copy.task)
{
  connect(&(this->taskList), &QListWidget::itemActivated, this, &Project::openTask);
  connect(&(this->editButton), &QPushButton::clicked, this, &Project::edit);
  connect(this->network, &Network::readFinished, this, &Project::getReply);
  connect(&(this->BBack), &QPushButton::clicked, this, &Project::back);

  this->editButton.setText("Edit project");
  this->BBack.setText("Back");

  this->setName.setHidden(true);
  this->setName.setPlaceholderText(this->name);

  this->GBCheckBox.setHidden(true);

  this->layout.addWidget(&(this->taskList));
  this->layout.addWidget(&(this->editButton));
  this->layout.addWidget(&(this->BBack));
  this->layout.addWidget(&(this->GBCheckBox));
  this->setLayout(&(this->layout));

  this->showMainPage();
}

void Project::createTask()
{
  if (this->BCreateTask.text() == "Create new task")
    {
      for (auto item:VCheckBox)
        delete item;
      this->VCheckBox.clear();

      QLayoutItem *child;
      while ((child = this->LVCheckBox.takeAt(0)) != nullptr) {
          delete child;
        }
      this->newTaskName.clear();

      this->hideAll();
      this->tasksLabel.setVisible(true);
      this->employeeLabel.setVisible(true);
      this->BBack.setVisible(true);
      this->BCreateTask.setVisible(true);
      this->newTaskName.setVisible(true);
      this->GBCheckBox.setVisible(true);

      this->newTaskName.setReadOnly(false);
      this->newTaskName.setPlaceholderText("Enter new task's name");

      if (this->LCheckBox.indexOf(&(this->LECheckBox)) == -1)
        this->LCheckBox.addLayout(&(this->LECheckBox));

      if (this->LCheckBox.indexOf(&(this->LVCheckBox)) == -1)
        this->LCheckBox.addLayout(&(this->LVCheckBox));

      if (this->LGroupBox.indexOf(&(this->newTaskName)) == -1)
        this->LGroupBox.insertWidget(0, &(this->newTaskName));

      if (this->LGroupBox.indexOf(&(this->newDescription)) == -1)
        this->LGroupBox.insertWidget(1, &(this->newDescription));

      if (this->GBCheckBox.layout() == nullptr)
        this->GBCheckBox.setLayout(&(this->LGroupBox));

      this->newDescription.setReadOnly(false);
      this->newDescription.setPlaceholderText("Input description");
      this->newDescription.setVisible(true);

      if (this->layout.indexOf(&(this->GBCheckBox)) == -1)
        this->layout.addWidget(&(this->GBCheckBox));
      this->BCreateTask.setText("Save");

      this->network->getTasks();
      this->network->getEmployee();
    }
  else
    {
      if (this->newTaskName.text() != "")
        {
          QStringList checkedTasks;
          checkedTasks.clear();

          for (auto checkedTask:this->VCheckBox)
            {
              if (checkedTask->isChecked())
                checkedTasks.append(checkedTask->text());
            }

          this->network->createTask(this->newTaskName.text(), checkedTasks);

          this->newTaskName.setHidden(true);
          this->showMainPage();
        }
      else
        {
          this->BCreateTask.setText("Create new task");
          this->showMainPage();
        }
    }
}

void Project::getEmployee()
{
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

      for (int task = 0; task < this->taskList.count(); task++)
        if (this->taskList.item(task)->text() == checkList.at(taskIndex))
          {
            newObject->setCheckState(Qt::Checked);
            this->checkedTasks.append(newObject->text());
            break;
          }

      this->VECheckBox.push_back(newObject);
      this->LECheckBox.addWidget(this->VECheckBox[this->VECheckBox.indexOf(newObject)]);
    }

  if (this->LECheckBox.indexOf(&(this->employeeLabel)) == -1)
    this->LECheckBox.insertWidget(0, &(this->employeeLabel));
}
