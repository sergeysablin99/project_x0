#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QListWidget>
#include "Project.h"
#include "network.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

public slots:
  void openProject(QListWidgetItem *item);

private:
    QListWidget _projectList;
    Project *_project;
    Network *_network_manager;
    QGridLayout _layout;
};

#endif // MAINWINDOW_H
