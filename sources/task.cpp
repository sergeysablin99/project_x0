#include "../includes/task.h"

Task::Task(){
  this->setWindowTitle(this->_name);
  this->setMinimumSize(400, 250);
  connect(&(this->_editButton), &QPushButton::clicked, this, &Task::SlotEditButton);

  this->_dateEdit.setReadOnly(false);
  this->_dateEdit.setVisible(false);

  this->_targetEdit.setPlaceholderText("Target");
  this->_targetEdit.setVisible(false);
  this->_targetEdit.setReadOnly(false);

  this->_descriptionEdit.setPlaceholderText("Text edit");
  this->_descriptionEdit.setVisible(false);
  this->_descriptionEdit.setReadOnly(false);

  this->_date.setText("Deadline: " + QDate::currentDate().toString("dd.MM.yyyy"));

  this->_target.setText("Target ");
  this->_target.setMaximumWidth(100);
  this->_target.setBaseSize(30, 20);

  this->_description.setReadOnly(true);
  this->_description.setPlainText("Description");
  this->_description.setMaximumSize(200, 70);
  this->_description.setBaseSize(200, 50);

  this->_editButton.setText("Edit");

  this->_layout.setSpacing(10);
  this->_layout.addWidget(&(this->_target), 0, 0, Qt::AlignLeft);
  this->_layout.addWidget(&(this->_date), 0, 1, Qt::AlignLeft);
  this->_layout.addWidget(&(this->_description), 1, 0, Qt::AlignLeft);
  this->_layout.addWidget(&(this->_employee));
  this->_layout.addWidget(&(this->_subtask));
  this->_layout.addWidget(&(this->_editButton), 3, 2, Qt::AlignLeft);
  this->_layout.addWidget(&(this->_targetEdit), 0, 1, Qt::AlignLeft);
  this->_layout.addWidget(&(this->_descriptionEdit), 1, 0, Qt::AlignLeft);
  this->_layout.addWidget(&(this->_dateEdit), 0, 2, Qt::AlignLeft);

  this->setLayout(&_layout);
}


void Task::SlotEditButton(){
  if (this->_editButton.text() == "Edit")  {
      this->_description.setVisible(false);

      this->_descriptionEdit.setVisible(true);
      this->_targetEdit.setVisible(true);
      this->_dateEdit.setVisible(true);

      this->_subtask.setVisible(false);
      this->_employee.setVisible(false);
      this->_editButton.setText("Save");
    } else {
      if (this->_descriptionEdit.toPlainText () != "")
        this->_description.setPlainText(this->_descriptionEdit.toPlainText());
      if (this->_targetEdit.text() != "")
        this->_target.setText(this->_targetEdit.text());
      this->_date.setText(this->_dateEdit.text());


      this->_description.setVisible(true);

      this->_descriptionEdit.setVisible(false);
      this->_targetEdit.setVisible(false);
      this->_dateEdit.setVisible(false);

      this->_subtask.setVisible(true);
      this->_employee.setVisible(true);
      this->_editButton.setText("Edit");
    }//else
}
