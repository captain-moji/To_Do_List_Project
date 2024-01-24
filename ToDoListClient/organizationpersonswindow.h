#ifndef ORGANIZATIONPERSONSWINDOW_H
#define ORGANIZATIONPERSONSWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QJsonArray>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>
#include <QDebug>
#include <QMessageBox>
#include <QTreeWidgetItem>
#include <QListWidget>

namespace Ui {
class OrganizationPersonsWindow;
}

class OrganizationPersonsWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit OrganizationPersonsWindow(QWidget *parent = nullptr);
    ~OrganizationPersonsWindow();
    void loadOrgPersons();
    QString ORG;

private:

    Ui::OrganizationPersonsWindow *ui;
signals:
    void org_person_selected(QString);
private slots:
    void on_org_persons_tree_widget_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void search_user();
    void on_search_line_edit_textChanged(const QString &arg1);
    void on_members_checkbox_stateChanged(int arg1);
    void on_Admins_checkbox_stateChanged(int arg1);
};

#endif // ORGANIZATIONPERSONSWINDOW_H
