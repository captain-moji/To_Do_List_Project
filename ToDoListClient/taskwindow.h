#ifndef TASKWINDOW_H
#define TASKWINDOW_H

#include <QMainWindow>
#include <task.h>
#include <QMessageBox>
#include <QStringList>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QDateTime>
#include <organization.h>
#include <orgdialog.h>
#include <orgperson.h>
#include <project.h>
#include <task.h>
#include <comment.h>
#include <QtAlgorithms>


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


    void thisOrgSetter(QString);
    void thisprojectSetter(QString);
    void thistaskSetter(const Task &);
    void thisCommentSetter(Comment &);
    QVector<Comment> &commentsGetter();

    void thisPersonSetter(const OrgPerson &);

public slots:
    void loadTaskComments(QString s);
    void addCommentsToTree();

private slots:

    void on_make_new_task_BTN_clicked();

    void on_save_task_BTN_clicked();

    void this_task_maker(Task);

    void owner_name_setter(QString);

    void owner_id_setter(QString);

    void on_teams_list_widget_itemClicked(QListWidgetItem *item);

    void on_persons_list_widget_itemClicked(QListWidgetItem *item);

    void on_Owner_type_combobox_currentTextChanged(const QString &arg1);


    void on_task_add_new_comment_BTN_clicked();
    void add_comment(QString text);

    QString saveTaskComments();

    void addCommentToTree(Comment &comment, QTreeWidgetItem *item);
    Comment getCommentFromItem(QTreeWidgetItem *item);
    void getCommentFromTree();

    void on_task_add_new_reply_BTN_clicked();
    void add_reply(QString text);

    void on_task_remove_comment_BTN_clicked();

signals:
    //void new_task_maked(QString, Task);
    void new_task_maked(Task);

    void task_edited(Task);

    //void task_edited(QString, Task);
    void comments_saved(QString);


private:

    QString temp_owner_name;
    QString temp_owner_id;

    Task this_task;
    QString this_org;
    QString this_project;
    Ui::TaskWindow *ui;

    QVector<Comment> comments;
    Comment this_comment;

    OrgPerson this_person;

    QString json;

};

#endif // TASKWINDOW_H
