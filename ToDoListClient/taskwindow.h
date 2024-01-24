#ifndef TASKWINDOW_H
#define TASKWINDOW_H

#include <QMainWindow>
#include <task.h>

namespace Ui {
class TaskWindow;
}

class TaskWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TaskWindow(QWidget *parent = nullptr);
    ~TaskWindow();
    void typeSetter(QString);

private slots:


    void on_make_new_task_BTN_clicked();

signals:
    void new_task_maked(Task);

private:
    Task new_task;
    Task this_task;
    Ui::TaskWindow *ui;
};

#endif // TASKWINDOW_H
