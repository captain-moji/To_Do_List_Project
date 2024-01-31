#ifndef TASKWINDOW_H
#define TASKWINDOW_H

#include <QMainWindow>
#include <task.h>
#include "personsandteamswindow.h"
#include <QMessageBox>

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
    void thisOrgProjectSetter(QString,QString);

private slots:

    void on_make_new_task_BTN_clicked();

    void on_save_task_BTN_clicked();

    void this_task_maker(Task);

    void on_set_new_owner_BTN_clicked();

    void owner_name_setter(QString);
    void owner_id_setter(QString);

signals:
    void new_task_maked(Task);
    void task_edited(Task);


private:

    QString temp_owner_name;
    QString temp_owner_id;

    Task this_task;
    QString this_org;
    QString this_project;
    Ui::TaskWindow *ui;
};

#endif // TASKWINDOW_H
