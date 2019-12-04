﻿#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Project.h"
#include "user.h"

class MainWindow : public QWidget
{
  Q_OBJECT

  QLabel projectsLabel;
  QGridLayout LMain;
  QPushButton mainBBack;
  QPushButton BDeleteProject;
  QListWidget LWProjects;
  Network network;
  Project* project;

  //Widgets for edits
  QVector<QCheckBox*> VCheckBox;
  QGroupBox GBCheckBox;
  QVBoxLayout LCheckBox;
  QVBoxLayout LGroupBox;
  QPushButton BCreateProject;
  QLineEdit newProjectName;

  //LOGIN
  QLineEdit login;
  QLineEdit password;
  QPushButton BConfirm;

  QPushButton BAccount;
  QTextEdit personalTasks;

  //SETTINGS
  QPushButton BSettings;
  QLineEdit inputAddress;

public:
  MainWindow();
  ~MainWindow();
  void hideAll();

public slots:
  void showMainPage();
  void showSettings();
  void createProject();
  void deleteProject();
  void getReply();
  void projectClosed();
  void openProject(QListWidgetItem* projectName);
  void sendLogin();
  void loggedIn();
  void getPersonalTasks();
  void openAccount();
};
#endif // MAINWINDOW_H
