#include "../include/mainwindow.h"

MainWindow::MainWindow() //: project(nullptr)
{
  connect(&(this->network), &Network::readFinished, this, &MainWindow::getReply);
  connect(&(this->mainBBack), &QPushButton::clicked, this, &MainWindow::showMainPage);
  connect(&(this->BCreateProject), &QPushButton::clicked, this, &MainWindow::createProject);
  connect(&(this->BDeleteProject), &QPushButton::clicked, this, &MainWindow::deleteProject);
  connect(&(this->LWProjects), &QListWidget::itemDoubleClicked, this, &MainWindow::openProject);

  this->mainBBack.setText("Back");
  this->mainBBack.setHidden(true);

  this->BCreateProject.setText("Create project");
  this->BDeleteProject.setText("Delete project");

  this->GBCheckBox.setHidden(true);

  this->LMain.addWidget(&(this->mainBBack));
  this->LMain.addWidget(&(this->BCreateProject));
  this->LMain.addWidget(&(this->BDeleteProject));
  this->LMain.addWidget(&(this->GBCheckBox));
  this->LMain.addWidget(&(this->LWProjects));
  this->setLayout(&(this->LMain));

  this->showMainPage();
}

void MainWindow::getReply()
{
  qDebug() << "start of getReply";
  this->LWProjects.clear();

  this->network.unpackReply("name");
  this->LWProjects.addItems(this->network.returnReply().toList());
  qDebug() << LWProjects.item(0);

  for (auto item:VCheckBox)
    delete item;
  this->VCheckBox.clear();

  QLayoutItem *child;
  while ((child = this->LCheckBox.takeAt(0)) != nullptr) {
      delete child;
    }

  for (int projectIndex = 0; projectIndex < this->LWProjects.count(); projectIndex++)
    {
      qDebug() << "Push checkbox";
      QCheckBox *newObject = new QCheckBox(LWProjects.item(projectIndex)->text());
      this->VCheckBox.push_back(newObject);
      this->LCheckBox.addWidget(this->VCheckBox[this->VCheckBox.indexOf(newObject)]);
    }

  if (LGroupBox.indexOf(&(this->LCheckBox)) == -1)
    {
      this->LGroupBox.addLayout(&(this->LCheckBox));
      this->GBCheckBox.setLayout(&(this->LGroupBox));
      this->LCheckBox.setContentsMargins(10, 10, 10, 10);
    }
}

MainWindow::~MainWindow()
{
//  this->LCheckBox.deleteLater();
  for (auto item:VCheckBox)
    delete item;
  if (this->project != nullptr)
    delete this->project;
}

void MainWindow::showMainPage()
{
  for (int counter = 0; counter < this->LMain.count(); counter++)
    {
      if (counter != this->LMain.indexOf(&(this->BCreateProject)) &&
          counter != this->LMain.indexOf(&(this->BDeleteProject)) &&
          counter != this->LMain.indexOf(&(this->LWProjects)))
        {
          if (this->LMain.itemAt(counter)->widget())
            {
              this->LMain.itemAt(counter)->widget()->setHidden(true);
            }
        }//if
    }

  this->mainBBack.setText("Back");
  this->BDeleteProject.setText("Delete project");
  this->BCreateProject.setText("Create project");

  if (this->BCreateProject.isHidden())
    this->BCreateProject.setVisible(true);
  if(this->BDeleteProject.isHidden())
    this->BDeleteProject.setVisible(true);
  if (this->LWProjects.isHidden())
    this->LWProjects.setVisible(true);
  this->LWProjects.clear();
  qDebug() << "Show MainPage";
  this->network.getProjects();
}

void MainWindow::createProject()
{
  if (this->BCreateProject.text() == "Create project")
    {
      //Clear old data
      for (auto item:VCheckBox)
        delete item;
      this->VCheckBox.clear();

      QLayoutItem *child;
      while ((child = this->LCheckBox.takeAt(0)) != nullptr) {
          delete child;
        }
      this->newProjectName.clear();

      //Set "create" widgets
      this->hideAll();
      this->mainBBack.setVisible(true);
      this->BCreateProject.setVisible(true);
      this->newProjectName.setVisible(true);
      this->GBCheckBox.setVisible(true);

      this->newProjectName.setReadOnly(false);
      this->newProjectName.setPlaceholderText("Enter new project's name");

      if (this->LGroupBox.indexOf(&(this->newProjectName)) == -1)
        this->LGroupBox.insertWidget(0, &(this->newProjectName));

      if (this->LMain.indexOf(&(this->GBCheckBox)) == -1)
        this->LMain.addWidget(&(this->GBCheckBox));
      this->BCreateProject.setText("Save");

      //Get taks from server
      this->network.getTasks();
    }
  else
    {
      if (this->newProjectName.text() != "")
        {
          QStringList checkedTasks;
          checkedTasks.clear();

          for (auto checkedTask:this->VCheckBox)
            {
              if (checkedTask->isChecked())
                checkedTasks.append(checkedTask->text());
            }
          qDebug() << this->newProjectName.text();
          this->network.createProject(this->newProjectName.text(), checkedTasks);
          this->showMainPage();
        }
    }
}

void MainWindow::hideAll()
{
  for (int counter = 0; counter < this->LMain.count(); counter++)
    {
      this->LMain.itemAt(counter)->widget()->setHidden(true);
    }
}

void MainWindow::deleteProject()
{
  if (this->BDeleteProject.text() == "Delete project")
    {
      for (auto item:VCheckBox)
        delete item;
      this->VCheckBox.clear();

      QLayoutItem *child;
      while ((child = this->LCheckBox.takeAt(0)) != nullptr) {
          delete child;
        }

      //Set "create" widgets
      this->hideAll();
      this->mainBBack.setVisible(true);
      this->BDeleteProject.setVisible(true);
      this->newProjectName.setHidden(true);
      this->GBCheckBox.setVisible(true);

      if (this->LMain.indexOf(&(this->GBCheckBox)) == -1)
        this->LMain.addWidget(&(this->GBCheckBox));

      this->BDeleteProject.setText("Confirm");

      //Get taks from server
      this->network.getProjects();
    }
  else
  {
      for (auto checkedProject:this->VCheckBox)
        {
          if (checkedProject->isChecked())
            this->network.deleteProject(checkedProject->text());
        }
        qDebug() << "ShowMainPage";
      this->showMainPage();
  }
}

void MainWindow::openProject (QListWidgetItem* projectName)
{
  disconnect(&(this->network), &Network::readFinished, this, &MainWindow::getReply);

  this->project = new Project(this, projectName->text(), &(this->network));
  connect(this->project, &Project::showMainWindow, this, &MainWindow::projectClosed);

  this->hideAll();
  this->LMain.addWidget(this->project);
  this->project->show();
}

void MainWindow::projectClosed()
{
  qDebug() << "project closed";
  connect(&(this->network), &Network::readFinished, this, &MainWindow::getReply);

  this->showMainPage();
}

void MainWindow::debugParents()
{
  for (int counter = 0; counter < this->LMain.count(); counter++)
    {
      qDebug() << this->LMain.itemAt(counter)->widget() << "Parent: " << this->LMain.itemAt(counter)->widget()->parent();
    }
}
