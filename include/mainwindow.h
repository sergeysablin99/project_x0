#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Project.h"

class MainWindow : public QWidget
{
  Q_OBJECT

  QHBoxLayout LMain;
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

public:
  MainWindow();
  ~MainWindow();
  void hideAll();
  void debugParents();

public slots:
  void showMainPage();
  void createProject();
  void deleteProject();
  void getReply();
  void projectClosed();
  void openProject(QListWidgetItem* projectName);
};
#endif // MAINWINDOW_H
