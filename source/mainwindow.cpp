#include "../include/mainwindow.h"

MainWindow::MainWindow() : project(nullptr)
{
  this->setWindowTitle("Project_X1");
  connect(&(this->network), &Network::loggedIn, this, &MainWindow::loggedIn);
  connect(&(this->BConfirm), &QPushButton::clicked, this, &MainWindow::sendLogin);
  connect(&(this->BSettings), &QPushButton::clicked, this, &MainWindow::showSettings);
  connect(&(this->BAccount), &QPushButton::clicked, this, &MainWindow::openAccount);

  this->inputAddress.setHidden(true);
  this->inputAddress.setPlaceholderText("Input new address");

  this->BSettings.setText("Settings");
  this->BConfirm.setText("Confirm");

  this->login.setPlaceholderText("Enter login");
  this->password.setPlaceholderText("Enter password");
  this->password.setEchoMode(QLineEdit::Password);

  this->LMain.addWidget(&(this->inputAddress), 1, 0, Qt::AlignLeft);
  this->LMain.addWidget(&(this->login), 0, 0, Qt::AlignLeft);
  this->LMain.addWidget(&(this->password), 1, 0, Qt::AlignLeft);
  this->LMain.addWidget(&(this->BConfirm), 0, 1, Qt::AlignLeft);
  this->LMain.addWidget(&(this->BSettings), 1, 1, Qt::AlignLeft);

  this->setLayout(&(this->LMain));
}

void MainWindow::getReply()
{
  this->LWProjects.clear();

  this->network.unpackReply("name");
  this->LWProjects.addItems(this->network.returnReply().toList());

  for (auto item:VCheckBox)
    delete item;
  this->VCheckBox.clear();

  QLayoutItem *child;
  while ((child = this->LCheckBox.takeAt(0)) != nullptr) {
      delete child;
    }

  for (int projectIndex = 0; projectIndex < this->LWProjects.count(); projectIndex++)
    {
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
          counter != this->LMain.indexOf(&(this->LWProjects)) &&
          counter != this->LMain.indexOf(&(this->BAccount)) &&
          counter != this->LMain.indexOf(&(this->projectsLabel)))
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
  if (this->projectsLabel.isHidden())
    this->projectsLabel.setVisible(true);
  if (this->BAccount.isHidden())
    this->BAccount.setVisible(true);
  this->LWProjects.clear();

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
      else
        {
          this->BCreateProject.setText("Create project");
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

  this->project = new Project(nullptr, projectName->text(), &(this->network));
  connect(this->project, &Project::showMainWindow, this, &MainWindow::projectClosed);

  this->hideAll();
  this->LMain.addWidget(this->project);
  this->project->show();
}

void MainWindow::projectClosed()
{
  connect(&(this->network), &Network::readFinished, this, &MainWindow::getReply);

  this->showMainPage();
}

void MainWindow::loggedIn()
{
  this->login.close();
  this->password.close();
  this->BConfirm.close();
  this->BSettings.close();
  this->LMain.removeWidget(&(this->BConfirm));
  this->LMain.removeWidget(&(this->login));
  this->LMain.removeWidget(&(this->password));
  this->LMain.removeWidget(&(this->BSettings));

  connect(&(this->network), &Network::readFinished, this, &MainWindow::getReply);
  connect(&(this->mainBBack), &QPushButton::clicked, this, &MainWindow::showMainPage);
  connect(&(this->BCreateProject), &QPushButton::clicked, this, &MainWindow::createProject);
  connect(&(this->BDeleteProject), &QPushButton::clicked, this, &MainWindow::deleteProject);
  connect(&(this->LWProjects), &QListWidget::itemDoubleClicked, this, &MainWindow::openProject);
  connect(&(this->network), &Network::signalReturnPersonaTasks, this, &MainWindow::getPersonalTasks);

  this->mainBBack.setText("Back");
  this->mainBBack.setHidden(true);

  this->BAccount.setText("Account");
  this->BAccount.setVisible(false);

  this->BCreateProject.setText("Create project");
  this->BDeleteProject.setText("Delete project");

  this->GBCheckBox.setHidden(true);

  this->projectsLabel.setText("Projects list");
  this->projectsLabel.setVisible(true);
  this->projectsLabel.setMargin(10);

  this->LMain.addWidget(&(this->projectsLabel), 0, 0, Qt::AlignLeft);
  this->LMain.addWidget(&(this->mainBBack), 3, 0, Qt::AlignLeft);
  this->LMain.addWidget(&(this->BCreateProject), 2, 0, Qt::AlignLeft);
  this->LMain.addWidget(&(this->BDeleteProject), 2, 1, Qt::AlignLeft);
  this->LMain.addWidget(&(this->GBCheckBox), 2, 0, Qt::AlignLeft);
  this->LMain.addWidget(&(this->LWProjects), 1, 0, Qt::AlignLeft);
  this->LMain.addWidget(&(this->BAccount), 2, 2, Qt::AlignLeft);

  this->showMainPage();
}

void MainWindow::sendLogin()
{
  if (!this->login.text().isEmpty() && !this->login.text().isEmpty())
    {
      this->network.user.login = this->login.text();
      this->network.user.password = this->password.text();
      this->network.login();

      this->password.clear();
    }
}

void MainWindow::showSettings()
{
  if (this->BSettings.text() == "Settings")
    {
      this->hideAll();
      this->BSettings.setVisible(true);
      this->BSettings.icon().detach();
      this->BSettings.setText("Save");
      this->inputAddress.setVisible(true);
    }
  else
    {
      if (!this->inputAddress.text().isEmpty())
        this->network.setServerAddress(this->inputAddress.text());
      this->hideAll();
      this->login.setVisible(true);
      this->password.setVisible(true);
      this->BConfirm.setVisible(true);
      this->BSettings.setVisible(true);
      this->BSettings.setText("Settings");
    }
}

void MainWindow::openAccount()
{
  this->hideAll();
  this->mainBBack.setVisible(true);

  if (this->LMain.indexOf(&(this->personalTasks)) == -1)
    this->LMain.addWidget(&(this->personalTasks));

  this->personalTasks.append("Job's done!");
  this->personalTasks.setReadOnly(true);
  this->personalTasks.setVisible(true);

  this->network.personalTasks();
}

void MainWindow::getPersonalTasks()
{
  qDebug() << "personal tasks";
  this->personalTasks.clear();
  this->personalTasks.append(this->network.returnPersonalTasks());
}
