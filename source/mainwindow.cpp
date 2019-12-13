#include "../include/mainwindow.h"

MainWindow::MainWindow() : project(nullptr), LMain(QBoxLayout::LeftToRight), LEnter(QBoxLayout::TopToBottom), LButtons(QBoxLayout::TopToBottom)
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

  this->tasksLabel.setHidden(true);
  this->tasksLabel.setText("Choose tasks");

  this->LEnter.addWidget(&(this->login));
  this->LEnter.addWidget(&(this->password));
  this->LEnter.addWidget(&(this->inputAddress));
  this->LEnter.setAlignment(Qt::AlignCenter);
  this->LEnter.setContentsMargins(100, 0, 0, 0);

  this->LButtons.addWidget(&(this->BConfirm));
  this->LButtons.addWidget(&(this->BSettings));
  this->LButtons.setAlignment(Qt::AlignCenter);
  this->LButtons.setContentsMargins(0, 0, 100, 0);

  this->LMain.addLayout(&(this->LEnter));
  this->LMain.addLayout(&(this->LButtons));

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

  if (this->LCheckBox.indexOf(&(this->tasksLabel)) == -1)
    this->LCheckBox.insertWidget(0, &(this->tasksLabel));
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
  this->hideAll();

  this->mainBBack.setText("Back");
  this->BDeleteProject.setText("Delete project");
  this->BCreateProject.setText("Create project");

  this->LWProjects.clear();

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
      this->tasksLabel.setText("choose new project's subtasks");
      this->tasksLabel.setVisible(true);
      this->mainBBack.setVisible(true);
      this->BCreateProject.setVisible(true);
      this->newProjectName.setVisible(true);
      this->GBCheckBox.setVisible(true);

      this->newProjectName.setReadOnly(false);
      this->newProjectName.setPlaceholderText("Enter new project's name");

      if (this->LGroupBox.indexOf(&(this->newProjectName)) == -1)
        this->LGroupBox.insertWidget(0, &(this->newProjectName));

      this->BCreateProject.setText("Save");

      //Get taks from server
      this->network.getTasks();
    }
  else
    {
      if (!this->newProjectName.text().isEmpty())
        {
          QRegExp exp = QRegExp("[^']+\\w");
          QRegExpValidator validator;
          validator.setRegExp(exp);
          int pos = 0;
          QString stringToValidate = this->newProjectName.text().simplified();

          if (validator.validate(stringToValidate, pos) != QValidator::Invalid)
            {
              QStringList checkedTasks;
              checkedTasks.clear();

              for (auto checkedTask:this->VCheckBox)
                {
                  if (checkedTask->isChecked())
                    checkedTasks.append(checkedTask->text().simplified());
                }

              this->network.createProject(this->newProjectName.text().simplified(), checkedTasks);
              this->showMainPage();
            }
          else
            {
              this->error.setText("Incorrect input");
              if (!(this->error.button(QMessageBox::Ok)))
                this->error.addButton(QMessageBox::Ok);
              this->error.show();
            }
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
      if (this->LMain.itemAt(counter)->widget() != nullptr)
        this->LMain.itemAt(counter)->widget()->setHidden(true);
    }

  for (int counter = 0; counter < this->LEnter.count(); counter++)
    {
      if (this->LEnter.itemAt(counter)->widget() != nullptr)
        this->LEnter.itemAt(counter)->widget()->setHidden(true);
    }

  for (int counter = 0; counter < this->LButtons.count(); counter++)
    {
      if (this->LButtons.itemAt(counter)->widget() != nullptr)
        this->LButtons.itemAt(counter)->widget()->setHidden(true);
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
      this->tasksLabel.setText("Choose project to delete");
      this->tasksLabel.setVisible(true);
      this->mainBBack.setVisible(true);
      this->BDeleteProject.setVisible(true);
      this->newProjectName.setHidden(true);
      this->GBCheckBox.setVisible(true);

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
  connect(&(this->network), &Network::signalReturnPersonaTasks, this, &MainWindow::getPersonalTasks);
  this->showMainPage();
}

void MainWindow::loggedIn()
{
  this->login.close();
  this->password.close();
  this->BConfirm.close();
  this->BSettings.close();
  this->LEnter.removeWidget(&(this->login));
  this->LEnter.removeWidget(&(this->password));
  this->LEnter.removeWidget(&(this->inputAddress));
  this->LEnter.setContentsMargins (0, 0, 0, 0);

  this->LButtons.removeWidget(&(this->BConfirm));
  this->LButtons.removeWidget(&(this->BSettings));
  this->LButtons.setContentsMargins (0, 0, 0, 0);

  this->LMain.takeAt(this->LMain.indexOf(&(this->LEnter)));
  this->LMain.takeAt(this->LMain.indexOf(&(this->LButtons)));
  this->LMain.setDirection(QBoxLayout::TopToBottom);

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

  this->LEnter.addWidget(&(this->GBCheckBox));
  this->LEnter.addWidget(&(this->personalTasks));
  this->LEnter.addWidget(&(this->newProjectName));
  this->LEnter.addWidget(&(this->personalTasks));

  this->LButtons.setDirection(QBoxLayout::LeftToRight);
  this->LButtons.addWidget(&(this->mainBBack));
  this->LButtons.addWidget(&(this->BCreateProject));
  this->LButtons.addWidget(&(this->BDeleteProject));
  this->LButtons.addWidget(&(this->BAccount));

  this->LMain.addWidget(&(this->projectsLabel));
  this->LMain.addWidget(&(this->LWProjects));
  this->LMain.addLayout(&(this->LEnter));
  this->LMain.addLayout(&(this->LButtons));
  this->LMain.setAlignment(Qt::AlignCenter);

  this->showMainPage();
}

void MainWindow::sendLogin()
{

  if (!this->login.text().isEmpty() && !this->login.text().isEmpty())
    {
      QRegExp exp = QRegExp("[^']+\\w");
      QRegExpValidator validator;
      validator.setRegExp(exp);
      int pos = 0;
      QString stringToValidate = this->login.text().simplified() + this->password.text().simplified();

      if (validator.validate(stringToValidate, pos) != QValidator::Invalid)
        {
          this->network.user.login = this->login.text().simplified();
          this->network.user.password = this->password.text().simplified();
          this->network.login();

          this->password.clear();
        }
  else
    {
      this->error.setText("Incorrect input");
      if (!(this->error.button(QMessageBox::Ok)))
        this->error.addButton(QMessageBox::Ok);
      this->error.show();
    }
    }
}

void MainWindow::showSettings()
{
  if (this->BSettings.text() == "Settings")
    {
      this->hideAll();
      this->BSettings.setVisible(true);
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

  this->personalTasks.clear();
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
