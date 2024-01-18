#ifndef ORGANIZATIONSWINDOW_H
#define ORGANIZATIONSWINDOW_H

#include <QMainWindow>
#include <person.h>
#include <QString>
#include <organization.h>
#include <QJsonArray>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>
#include <QDebug>
#include <QMessageBox>
#include <allserverusers.h>
#include <orgperson.h>
#include <QTreeWidgetItem>
#include <QMap>
#include <QTimer>

namespace Ui {
class OrganizationsWindow;
}

class OrganizationsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit OrganizationsWindow(QWidget *parent = nullptr);
    ~OrganizationsWindow();
    void all_org_persons_display();
    void removeUserFromOrganization(QString);

private slots:
    void this_org_maker(QString);

    void on_org_page_add_person_BTN_clicked();

    void addUserToOrganization(QString);

    void promoteToAdmin(QString user);

    void on_org_page_edit_person_BTN_clicked();

    void on_org_page_remove_person_BTN_clicked();

    void on_org_users_search_line_edit_textChanged(const QString &arg1);


    void on_is_org_admin_checkbox_stateChanged(int arg1);

    void on_is_org_member_checkbox_stateChanged(int arg1);

private:
    QString this_org;
    OrgPerson temp_org_person;
    Ui::OrganizationsWindow *ui;
    void makeAllpersonsFile();
    void search_org_user();
};

#endif // ORGANIZATIONSWINDOW_H
