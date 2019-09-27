#ifndef CALENDAR_H
#define CALENDAR_H

#include <QCalendarWidget>
#include <QLayout>

class Calendar : public QCalendarWidget{
public:
  Calendar(QWidget *parent = nullptr);

public slots:
  void SlotShowDay();
};

#endif // CALENDAR_H
