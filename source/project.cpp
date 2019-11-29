#include "../include/Project.h"

Project::Project(QWidget* parent) : QWidget(parent)
{
  this->network = nullptr;
  //  this->task = nullptr;
}

Project::Project(QWidget* parent, QString name, Network* manager): QWidget(parent), name(name), network(manager), task(nullptr)
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

  //Добавим виджеты на экран
  this->layout.addWidget(&(this->taskList));
  this->layout.addWidget(&(this->editButton));
  this->layout.addWidget(&(this->BBack));
  this->layout.addWidget(&(this->GBCheckBox));
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

      this->setName.setPlaceholderText(this->name);

      this->LGroupBox.setContentsMargins(10, 10, 10, 10);

      this->network->getTasks();
    } else {
      if (!this->setName.text().isEmpty())
      {
          this->network->setProjectName(this->name, this->setName.text());
          this->name = this->setName.text();
      }

      for (auto task:this->VCheckBox){
          if (task->isChecked())
            this->network->addTask(task->text(), this->name);
          else
            {
              this->network->deleteTask(task->text(), this->name);
            }
        }

      this->showMainPage();
    }
}

void Project::openTask (QListWidgetItem *taskName)
{
  disconnect(this->network, &Network::readFinished, this, &Project::getReply);
  this->hideAll();

//  if (this->task != nullptr)
//    delete this->task;

  this->task = new Task(this, taskName->text(), this->network);
  connect(this->task, &Task::showProject, this, &Project::taskClosed);

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
    qDebug() << "Start of project's get reply";

    this->network->unpackReply("name");
    QStringList checkList = this->network->returnReply().toList();
    qDebug() << checkList;

    if (this->layout.indexOf(&(this->taskList)) == -1)
      {
        this->layout.addWidget(&(this->taskList));
      }

    //Add data to "create" field
    for (auto item:VCheckBox)
      delete item;
    this->VCheckBox.clear();

    QLayoutItem *child;
    while ((child = this->LCheckBox.takeAt(0)) != nullptr) {
        delete child;
      }

    for (int taskIndex = 0; taskIndex < checkList.count(); taskIndex++)
      {
        qDebug() << "Project's Push checkbox";
        QCheckBox *newObject = new QCheckBox(checkList.at(taskIndex));

        for (int task = 0; task < this->taskList.count(); task++)
          if (this->taskList.item(task)->text() == checkList.at(taskIndex))
            {
              newObject->setCheckState(Qt::Checked);
              break;
            }

        this->VCheckBox.push_back(newObject);
        this->LCheckBox.addWidget(this->VCheckBox[this->VCheckBox.indexOf(newObject)]);
      }

    this->taskList.clear();
    this->taskList.addItems(checkList);

    if (this->LGroupBox.indexOf(&(this->LCheckBox)) == -1)
      this->LGroupBox.addLayout(&(this->LCheckBox));
    if (this->GBCheckBox.layout() == nullptr)
      this->GBCheckBox.setLayout(&(this->LGroupBox));
  }

  void Project::hideAll()
  {
    for (int counter = 0; counter < this->layout.count(); counter++)
      {
        this->layout.itemAt(counter)->widget()->setHidden(true);
      }
  }

  void Project::showMainPage()
  {
    for (int counter = 0; counter < this->layout.count(); counter++)
      {
        if (counter != this->layout.indexOf(&(this->editButton)) &&
            counter != this->layout.indexOf(&(this->taskList)) &&
            counter != this->layout.indexOf(&(this->BBack)))
          {
            if (this->layout.itemAt(counter)->widget())
              {
                this->layout.itemAt(counter)->widget()->setHidden(true);
              }
          }//if
      }

    this->editButton.setText("Edit project");

    if (this->taskList.isHidden())
      this->taskList.setVisible(true);
    if (this->editButton.isHidden())
      this->editButton.setVisible(true);
    if (this->BBack.isHidden())
      this->BBack.setVisible(true);

    qDebug() << "Show MainPage";
    this->network->getTasks(this->name);
  }

void Project::taskClosed()
{
qDebug() << "taskClosed";
  connect(this->network, &Network::readFinished, this, &Project::getReply);
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

  //Добавим виджеты на экран
  this->layout.addWidget(&(this->taskList));
  this->layout.addWidget(&(this->editButton));
  this->layout.addWidget(&(this->BBack));
  this->layout.addWidget(&(this->GBCheckBox));
  this->setLayout(&(this->layout));

  this->showMainPage();
}
