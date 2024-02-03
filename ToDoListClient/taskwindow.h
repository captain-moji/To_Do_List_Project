#ifndef TASKWINDOW_H
#define TASKWINDOW_H

#include <QMainWindow>
#include <task.h>
#include <QMessageBox>
#include <QStringList>
#include <QListWidgetItem>


namespace Ui
{
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
    void ownerListSetter (QStringList teams , QStringList persons);
private slots:

    void on_make_new_task_BTN_clicked();

    void on_save_task_BTN_clicked();

    void this_task_maker(Task);

    void owner_name_setter(QString);

    void owner_id_setter(QString);

    void on_teams_list_widget_itemClicked(QListWidgetItem *item);

    void on_persons_list_widget_itemClicked(QListWidgetItem *item);

    void on_Owner_type_combobox_currentTextChanged(const QString &arg1);

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
