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

private:
    Ui::ToDoList *ui;
};
#endif // TODOLIST_H
