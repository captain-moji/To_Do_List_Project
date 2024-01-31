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
#include <organizationswindow.h>
#include <QListWidget>
#include <allserverusers.h>
#include <orgdialog.h>
#include <QTcpSocket>

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

    void makeOrganizationsFile();    
    void saveOrganizations();
    void loadOrganizations();
    void searchOrganizations();
    void thisUserMaker (QString username, QString id , QString name);

public slots:

    void socket_readyRead();
    void socket_connected();
    void socket_bytesWritten();
    void socket_disconnected();

    void sendRequest(QString s);
    void responseChecker(QString);

private slots:
    void connectionMaker(QTcpSocket * s);

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

private:
    Ui::ToDoList *ui;
    Person this_user;
    QTcpSocket * connection;

signals:
    void org_name_signal(QString);

};
#endif // TODOLIST_H
