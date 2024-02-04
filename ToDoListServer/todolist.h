#ifndef TODOLIST_H
#define TODOLIST_H


#include <QMainWindow>
#include <person.h>
#include <QString>
#include <person.h>
#include <organization.h>
#include <QJsonArray>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>
#include <QDebug>
#include <QMessageBox>
#include <QVector>
#include <organizationswindow.h>
#include <QListWidget>
#include <allserverusers.h>
#include <orgdialog.h>
#include <QTimer>
#include <QDateTime>

QT_BEGIN_NAMESPACE
namespace Ui {
class ToDoList;
}
QT_END_NAMESPACE

class ToDoList : public QMainWindow
{
    Q_OBJECT

public:
    ToDoList(QWidget *parent = nullptr);
    ~ToDoList();
    void makeOrgFiles(QString);
    void removeOrgFiles(QString);
    void makeOrganizationsFile();    
    void saveOrganizations();
    void loadOrganizations();

public slots:

    void searchOrganizations();

private slots:
    void on_add_organization_BTN_clicked();

    void add_organization(QString);

    void on_edit_organization_BTN_clicked();

    void edit_organization(QString);

    void on_remove_organization_BTN_clicked();

    void on_sort_organizations_BTN_clicked();

    void on_actionSort_Organizations_triggered();

    void on_actionExit_triggered();

    void on_actionHelp_triggered();

    void on_actionAbout_triggered();

    void on_todolist_organizations_list_itemDoubleClicked(QListWidgetItem *item);

    void on_add_new_user_BTN_clicked();

    void on_search_organizations_qstring_textChanged(const QString &arg1);

    void on_pushButton_2_clicked();

    void ser_server_info(QString user,QString pass);

    void time_counter();

private:
    Ui::ToDoList *ui;
    QString this_time;
    QTimer * timer;

signals:
    void org_name_signal(QString);

};
#endif // TODOLIST_H
