#include "organizationswindow.h"
#include "ui_organizationswindow.h"
#include <qtcpsocket.h>
#include <todolist.h>

OrganizationsWindow::OrganizationsWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::OrganizationsWindow)
{
    ui->setupUi(this);
}

OrganizationsWindow::~OrganizationsWindow()
{
    delete ui;
}

void OrganizationsWindow::connectionMaker(QString ip, int port) // (QTcpSocket * s)
{
    this_ip = ip;
    this_port = port;
    socket = new QTcpSocket();
    socket->connectToHost(ip, port);
    connect(socket, &QTcpSocket::connected, this, &OrganizationsWindow::socket_connected);
    connect(socket, &QTcpSocket::readyRead, this, &OrganizationsWindow::socket_readyRead);
    connect(socket, &QTcpSocket::bytesWritten, this, &OrganizationsWindow::socket_bytesWritten);
    connect(socket, &QTcpSocket::disconnected, this, &OrganizationsWindow::socket_disconnected);
    qDebug() << "org win connected!";
}

void OrganizationsWindow::socket_readyRead()
{
    QByteArray data = socket->readAll();
    responseChecker(QString(data));
}

void OrganizationsWindow::socket_connected()
{
}

void OrganizationsWindow::socket_bytesWritten()
{
}

void OrganizationsWindow::socket_disconnected()
{
}

void OrganizationsWindow::sendRequest(QString s)
{
    QByteArray data = s.toUtf8();
    socket->write(data);
    qDebug() << s;
}

void OrganizationsWindow::responseChecker(QString s)
{
    qDebug() << "response get is: " << s;
    QJsonDocument doc = QJsonDocument::fromJson(s.toUtf8());
    QString resState = doc.object().value("res-state").toString();

    if (resState == "is-admin-checked")
    {
        bool p = doc.object().value("is-admin").toBool();
        qDebug() << " bool is: " << p;
        this_user_check_admin(p);
    }
    if (resState == "org-persons-list-ok")
    {
        load_org_persons(s);
    }
    if (resState == "user-not-found-in-server")
    {
        QString mess = doc.object().value("message").toString();
        QMessageBox::warning(this, "Not found", mess);
    }
    if (resState == "user-added-to-org")
    {
        QString mess = doc.object().value("message").toString();
        QMessageBox::information(this, "Added", mess);
        loadAllOrgPersons();
    }
    if (resState == "user-remove-from-org-ok")
    {
        QString mess = doc.object().value("message").toString();
        QMessageBox::information(this, "Removed!", mess);
        loadAllOrgPersons();
    }
    if (resState == "user-role-changed-ok")
    {
        QString mess = doc.object().value("message").toString();
        QMessageBox::information(this, "Change role!", mess);
        loadAllOrgPersons();
    }
    if (resState == "org-teams-list-ok")
    {
        show_org_teams(s);
    }
    if (resState == "team-add-to-org-ok")
    {
    QString mess = doc.object().value("message").toString();
    QMessageBox::information(this, "team added", mess);
    loadAllOrgTeams();
    }
    if (resState == "team-add-to-org-failed")
    {
        QString mess = doc.object().value("message").toString();
        QMessageBox::warning(this, "team add failed", mess);
    }
    if (resState == "team-removed-from-org-ok")
    {
        QString mess = doc.object().value("message").toString();
        QMessageBox::warning(this, "team removed", mess);
        loadAllOrgTeams();
    }
    if (resState == "team-edited-in-org-ok")
    {
        QString mess = doc.object().value("message").toString();
        QMessageBox::warning(this, "team Edited", mess);
        loadAllOrgTeams();
    }
    if (resState == "org-projects-list-ok")
    {
        show_org_projects(s);
    }
    if (resState == "project-add-to-org-ok")
    {
        QString mess = doc.object().value("message").toString();
        QMessageBox::information(this, "Project added", mess);
        loadAllOrgProjects();
    }
    if (resState == "project-add-to-org-failed")
    {
        QString mess = doc.object().value("message").toString();
        QMessageBox::warning(this, "Project add failed", mess);
    }
    if (resState == "project-edit-in-org-ok")
    {
        QString mess = doc.object().value("message").toString();
        QMessageBox::warning(this, "Project Edited", mess);
        loadAllOrgProjects();
    }
    if (resState == "project-remove-from-org-ok")
    {
        QString mess = doc.object().value("message").toString();
        QMessageBox::information(this, "Project Removed", mess);
        loadAllOrgProjects();
    }
    if (resState == "access-to-org-team-failed")
    {
        QString mess = doc.object().value("message").toString();
        QMessageBox::warning(this, "access failed", mess);
    }
    if (resState == "access-to-org-team-ok")
    {
        QString name = doc.object().value("name").toString();
        QString team_id = doc.object().value("team-id").toString();
        QString username = doc.object().value("username").toString();
        QString id = doc.object().value("id").toString();
        QString org_id = doc.object().value("org-id").toString();
        bool is_org_admin = doc.object().value("is-org-admin").toBool();
        bool is_team_admin = doc.object().value("is-team-admin").toBool();
        org_team_show(username, name,id,org_id, team_id, is_team_admin,is_org_admin);
    }

    if (resState == "is-project-member-ok")
    {
        QString name = doc.object().value("name").toString();
        QString username = doc.object().value("username").toString();
        QString id = doc.object().value("id").toString();
        QString org_id = doc.object().value("org-id").toString();
        show_org_project_to_member (username, name, id, org_id,true,false);
    }
    if (resState == "is-project-team-member-ok")
    {
        QString name = doc.object().value("name").toString();
        QString username = doc.object().value("username").toString();
        QString id = doc.object().value("id").toString();
        QString org_id = doc.object().value("org-id").toString();
        show_org_project_to_member (username,name,id, org_id, false , true);
    }
    if (resState == "is-project-member-failed")
    {
        QString message = doc.object().value("message").toString();
        QMessageBox::warning(this, "access failed", message);
    }


}


void OrganizationsWindow:: this_org_maker(QString a)
{
    this_org.orgSetName(a);
    ui->org_name_lable->setText(this_org.orgGetName());
}

void OrganizationsWindow::this_user_maker(Person temp_user, QString temp_org_id)
{
    this_user.perSetName(temp_user.perGetName());
    this_user.perSetID(temp_user.perGetId());
    this_user.perSetUsername(temp_user.perGetUsername());
    this_user.orgPerSetID(temp_org_id);
    ui->user_name_text->setText(temp_user.perGetName());
    ui->user_username_text->setText(temp_user.perGetUsername());

    QJsonObject jsonObject;
    jsonObject["req-type"] = "user-check-admin";
    jsonObject["username"] = this_user.perGetUsername();
    jsonObject["orgname"] = this_org.orgGetName();
    QJsonDocument jsonDocument(jsonObject);
    QString req = jsonDocument.toJson(QJsonDocument::Compact);
    sendRequest(req);
}

void OrganizationsWindow::this_user_check_admin(bool res)
{
    this_user.orgPerSetIsAdmin(res);
    qDebug ()<< "is admin this user: " << this_user.orgPerGetIsAdmin();
    if (res==true)
        ui->user_role_text->setText("Admin");
    else
        ui->user_role_text->setText("Normal User");

    loadAllOrgPersons();
}

void OrganizationsWindow::loadAllOrgPersons()
{
    QJsonObject jsonObject;
    jsonObject["req-type"] = "org-persons-list";
    jsonObject["orgname"] = this_org.orgGetName();
    QJsonDocument jsonDocument(jsonObject);
    QString req = jsonDocument.toJson(QJsonDocument::Compact);
    sendRequest(req);
}

void OrganizationsWindow::load_org_persons(QString s)
{
    ui->org_persons_tree_widget->clear();
    QJsonDocument doc = QJsonDocument::fromJson(s.toUtf8());
    QJsonObject obj = doc.object();
    QJsonArray persons = obj["org-persons"].toArray();

    for (int i = 0; i < persons.size(); ++i) {
        QJsonObject person = persons[i].toObject();
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->org_persons_tree_widget);
        item->setText(0, person["name"].toString());
        item->setText(1, person["username"].toString());
        item->setText(2, person["org_id"].toString());
        item->setText(3, person["is_admin"].toBool() ? "YES" : "NO");
        item->setText(4, person["id"].toString());

    }
    loadAllOrgTeams();
}

void OrganizationsWindow::on_org_page_add_person_BTN_clicked()
{
    if (this_user.orgPerGetIsAdmin() == false)
    {
        QMessageBox::warning(this, "admin permission", "You are not the organization admin!");
    }
    else
    {
        allServerUsers * a = new allServerUsers(this);
        connect (a,SIGNAL(name_selected(QString)),this,SLOT(add_new_person_to_organization(QString)));
        a->typeSetter("NEW_USER");
        a->setWindowTitle("Add New Person");
        a->show();
    }
}

void OrganizationsWindow::add_new_person_to_organization(QString a)
{
    addUserToOrganization(a);
}

void OrganizationsWindow::addUserToOrganization(QString input_user)
{
    QJsonObject jsonObject;
    jsonObject["req-type"] = "add-user-to-org";
    jsonObject["username"] = input_user;
    jsonObject["orgname"] = this_org.orgGetName();
    QJsonDocument jsonDocument(jsonObject);
    QString req = jsonDocument.toJson(QJsonDocument::Compact);
    sendRequest(req);
}

void OrganizationsWindow::on_org_page_remove_person_BTN_clicked()
{
    QTreeWidgetItem *selectedItem = ui->org_persons_tree_widget->currentItem();
    if (selectedItem)
    {
        if (this_user.orgPerGetIsAdmin() == false)
        {
            QMessageBox::warning(this, "admin permission", "You are not the organization admin!");
        }
        else
        {
            if (selectedItem->text(1) == this_user.perGetUsername() )
            {
                QMessageBox::warning(this, "A Big mistake!", "Really you wanna delete yourself ?!\nthat is NOT possible!");
            }
            else
            {
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, "Delete a person", "Are you sure?\nDeleteing is not returnable!", QMessageBox::Yes | QMessageBox::No);
                if (reply == QMessageBox::Yes)
                {
                    QString selecteduser = selectedItem->text(1);
                    removeUserFromOrganization(selecteduser);
                }
            }
        }
    }
    else
        QMessageBox::warning(this, "Select a person", "select a person from the list!");
}

void OrganizationsWindow::removeUserFromOrganization(QString user)
{
    QJsonObject jsonObject;
    jsonObject["req-type"] = "remove-user-from-org";
    jsonObject["username"] = user;
    jsonObject["orgname"] = this_org.orgGetName();
    QJsonDocument jsonDocument(jsonObject);
    QString req = jsonDocument.toJson(QJsonDocument::Compact);
    sendRequest(req);
}

void OrganizationsWindow::on_org_page_edit_person_BTN_clicked()
{
QTreeWidgetItem *selectedItem = ui->org_persons_tree_widget->currentItem();
if (selectedItem)
    {
    if (this_user.orgPerGetIsAdmin() == false)
    {
        QMessageBox::warning(this, "admin permission", "You are not the organization admin!");
    }
    else
    {
        if (selectedItem->text(1) == this_user.perGetUsername() )
        {
            QMessageBox::warning(this, "Self Demoting", "Really you wanna demote yourself ?!\nthat is NOT possible!");
        }
        else
        {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "change access of a person", "Are you sure to change " + selectedItem->text(1) + " role ?" , QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::Yes)
            {
                QString selecteduser = selectedItem->text(1);
                promoteToAdmin(selecteduser);
            }
        }
    }
}
else
    QMessageBox::warning(this, "Select a person", "select a person from the list!");
}

void OrganizationsWindow::promoteToAdmin(QString user)
{
    QJsonObject jsonObject;
    jsonObject["req-type"] = "change-user-role-in-org";
    jsonObject["username"] = user;
    jsonObject["orgname"] = this_org.orgGetName();
    QJsonDocument jsonDocument(jsonObject);
    QString req = jsonDocument.toJson(QJsonDocument::Compact);
    sendRequest(req);
}

void OrganizationsWindow::search_org_user()
{
    QString input = ui->org_users_search_line_edit->text();
    QString search_type;

    for (int i = 0; i < ui->org_persons_tree_widget->topLevelItemCount(); ++i)
    {
        QTreeWidgetItem *item = ui->org_persons_tree_widget->topLevelItem(i);
        {
            if (ui->search_org_per_by_name_radio->isChecked())
                search_type = item->text(0);
            else if (ui->search_org_per_by_username_radio->isChecked())
                search_type = item->text(1);
            else if (ui->search_org_per_by_org_ID_radio->isChecked())
                search_type = item->text(2);
        }

        if(!item->isHidden())
        {
            if (search_type.startsWith(input))
            {
                item->setHidden(false);
            }
            else
            {
                item->setHidden(true);
            }
        }
    }
}

void OrganizationsWindow::on_org_users_search_line_edit_textChanged(const QString &arg1)
{
    on_is_org_admin_checkbox_stateChanged(0);
    on_is_org_member_checkbox_stateChanged(0);
    search_org_user();
}

void OrganizationsWindow::on_is_org_admin_checkbox_stateChanged(int arg1)
{
    for (int i = 0; i < ui->org_persons_tree_widget->topLevelItemCount(); ++i)
    {
        QTreeWidgetItem *item = ui->org_persons_tree_widget->topLevelItem(i);

        if (ui->is_org_admin_checkbox->isChecked())
        {
            if (item->text(3)=="YES")
            {
                item->setHidden(false);
            }
        }
        else if (!ui->is_org_admin_checkbox->isChecked())
        {
            if (item->text(3)=="YES")
            {
                item->setHidden(true);
            }
        }
    }
    search_org_user();
}

void OrganizationsWindow::on_is_org_member_checkbox_stateChanged(int arg1)
{
    for (int i = 0; i < ui->org_persons_tree_widget->topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = ui->org_persons_tree_widget->topLevelItem(i);

        if (ui->is_org_member_checkbox->isChecked())
        {
            if (item->text(3)=="NO")
            {
                item->setHidden(false);
            }
        }
        else if (!ui->is_org_member_checkbox->isChecked())
        {
            if (item->text(3)=="NO")
            {
                item->setHidden(true);
            }
        }
    }
    search_org_user();
}




void OrganizationsWindow::loadAllOrgTeams()
{
    QJsonObject jsonObject;
    jsonObject["req-type"] = "load-org-teams";
    jsonObject["orgname"] = this_org.orgGetName();
    QJsonDocument jsonDocument(jsonObject);
    QString req = jsonDocument.toJson(QJsonDocument::Compact);
    sendRequest(req);
}

void OrganizationsWindow::show_org_teams(QString s)
{
    ui->teams_tree_widget->clear();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(s.toUtf8());
    QJsonObject jsonObj = jsonDoc.object();
    QJsonArray orgTeams = jsonObj["org-teams"].toArray();

    for (const QJsonValue &teamValue : orgTeams) {
        QJsonObject teamObj = teamValue.toObject();
        QString teamId = teamObj["team_id"].toString();
        QString teamName = teamObj["team_name"].toString();
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->teams_tree_widget);
        item->setText(0, teamName);
        item->setText(1, teamId);
        ui->teams_tree_widget->addTopLevelItem(item);
    }
    loadAllOrgProjects();
}

void OrganizationsWindow::on_add_team_BTN_clicked()
{
    if (this_user.orgPerGetIsAdmin() == false)
    {
        QMessageBox::warning(this, "admin permission", "You are not the organization admin!");
    }
    else
    {
        OrgDialog *temp_dialog = new OrgDialog(this);
        temp_dialog->type="TEAM_DIALOG";
        temp_dialog->ORG = this_org.orgGetName();
        temp_dialog->set_text("Enter name of new team:");
        connect(temp_dialog, SIGNAL(name_readed(QString)),this,SLOT(add_new_team_to_organization(QString)));
        temp_dialog->setWindowTitle("New Team Name");
        temp_dialog->show();
    }
}

void OrganizationsWindow::add_new_team_to_organization(QString a)
{
    addTeamToOrganization(a);
}

void OrganizationsWindow::addTeamToOrganization(QString s)
{
    QJsonObject jsonObject;
    jsonObject["req-type"] = "add-team-to-org";
    jsonObject["team_name"] = s;
    jsonObject["orgname"] = this_org.orgGetName();
    QJsonDocument jsonDocument(jsonObject);
    QString req = jsonDocument.toJson(QJsonDocument::Compact);
    sendRequest(req);
}

void OrganizationsWindow::on_remove_team_BTN_clicked()
{
    QTreeWidgetItem *selectedItem = ui->teams_tree_widget->currentItem();
    if (selectedItem)
    {
            if (this_user.orgPerGetIsAdmin() == false)
            {
                QMessageBox::warning(this, "admin permission", "You are not the organization admin!");
            }
            else
            {
                    QMessageBox::StandardButton reply;
                    reply = QMessageBox::question(this, "remove a team", "Are you sure to remove " + selectedItem->text(0) + " team?!" , QMessageBox::Yes | QMessageBox::No);
                    if (reply == QMessageBox::Yes)
                    {
                        QString selecteduser = selectedItem->text(0);
                        removeTeamFromOrganization(selecteduser);
                    }
            }
    }
    else
    QMessageBox::warning(this, "Select a team", "select a team from the list!");
}

void OrganizationsWindow::removeTeamFromOrganization(QString team_name)
{
    QJsonObject jsonObject;
    jsonObject["req-type"] = "remove-a-team-from-org";
    jsonObject["team_name"] = team_name;
    jsonObject["orgname"] = this_org.orgGetName();
    QJsonDocument jsonDocument(jsonObject);
    QString req = jsonDocument.toJson(QJsonDocument::Compact);
    sendRequest(req);
}

void OrganizationsWindow::on_edit_team_name_BTN_clicked()
{
    QTreeWidgetItem *selectedItem = ui->teams_tree_widget->currentItem();
    if (selectedItem)
    {
        if (this_user.orgPerGetIsAdmin() == false)
        {
            QMessageBox::warning(this, "admin permission", "You are not the organization admin!");
        }
        else
        {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "remove a team", "Are you sure to Edit " + selectedItem->text(0) + " Name?!" , QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::Yes)
            {
                team.teamSetName(ui->teams_tree_widget->currentItem()->text(0));
                OrgDialog *temp_dialog = new OrgDialog(this);
                temp_dialog->type="TEAM_DIALOG";
                temp_dialog->ORG = this_org.orgGetName();
                temp_dialog->set_text("Enter name of new team:");
                connect(temp_dialog, SIGNAL(name_readed(QString)),this, SLOT(edit_team_in_organization(QString)));
                temp_dialog->setWindowTitle("Edit Team Name");
                temp_dialog->show();
            }
        }
    }
    else
        QMessageBox::warning(this, "Select a team", "select a team from the list!");
}

void OrganizationsWindow::edit_team_in_organization(QString a)
{
    editTeaminOrganization(team.teamGetName(),a);
}

void OrganizationsWindow::editTeaminOrganization(QString old_name,QString new_name)
{
    QJsonObject jsonObject;
    jsonObject["req-type"] = "edit-team-in-org";
    jsonObject["team_old_name"] = old_name;
    jsonObject["team_new_name"] = new_name;
    jsonObject["orgname"] = this_org.orgGetName();
    QJsonDocument jsonDocument(jsonObject);
    QString req = jsonDocument.toJson(QJsonDocument::Compact);
    sendRequest(req);
}

void OrganizationsWindow::search_org_team()
{
    QString s = ui->search_teams_line_edit->text();
    for (int i = 0; i < ui->teams_tree_widget->topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = ui->teams_tree_widget->topLevelItem(i);
        if (item->text(0).startsWith(s)) {
            item->setHidden(false);
        } else {
            item->setHidden(true);
        }
    }
}

void OrganizationsWindow::on_search_teams_line_edit_textChanged(const QString &arg1)
{
    search_org_team();
}


QString OrganizationsWindow::getTeamIdByName(QString name)
{
    QString file_Path = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/" + this_org.orgGetName() + "/ORG_TEAMS.json" ;

    QFile file(file_Path);
    if (!file.open(QIODevice::ReadOnly))
    {
        return "";
    }

    QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll());
    file.close();

    QJsonArray jsonArray = jsonDoc.array();

    for (int i = 0; i < jsonArray.size(); ++i)
    {
        QJsonObject jsonObj = jsonArray[i].toObject();

        if (jsonObj["team_name"].toString() == name)
        {
            return jsonObj["team_id"].toString();
        }
    }

    return "";
}


void OrganizationsWindow::loadAllOrgProjects()
{
    QJsonObject jsonObject;
    jsonObject["req-type"] = "load-org-projects";
    jsonObject["orgname"] = this_org.orgGetName();
    QJsonDocument jsonDocument(jsonObject);
    QString req = jsonDocument.toJson(QJsonDocument::Compact);
    sendRequest(req);
}

void OrganizationsWindow::show_org_projects(QString s)
{
    ui->projects_tree_widget->clear();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(s.toUtf8());
    QJsonObject jsonObj = jsonDoc.object();
    QJsonArray orgTeams = jsonObj["org-projects"].toArray();

    for (const QJsonValue &teamValue : orgTeams) {
        QJsonObject teamObj = teamValue.toObject();
        QString projectAdminID = teamObj["project_admin_id"].toString();
        QString projectName = teamObj["project_name"].toString();
        QString projectAdminName;
        for (int i = 0; i < ui->org_persons_tree_widget->topLevelItemCount(); ++i) {
            QTreeWidgetItem* item = ui->org_persons_tree_widget->topLevelItem(i);
            if (item->text(4) == projectAdminID) {
                projectAdminName = item->text(1);
            }
        }

        QTreeWidgetItem *item = new QTreeWidgetItem(ui->projects_tree_widget);
        item->setText(0, projectName);
        item->setText(1, projectAdminName);
        ui->projects_tree_widget->addTopLevelItem(item);
    }
}

void OrganizationsWindow::on_add_project_BTN_clicked()
{ 
    if (this_user.orgPerGetIsAdmin() == false)
    {
        QMessageBox::warning(this, "admin permission", "You are not the organization admin!");
    }
    else
    {
    OrgDialog *temp_dialog = new OrgDialog(this);
    temp_dialog->type="PROJECT_DIALOG";
    temp_dialog->ORG = this_org.orgGetName();
    temp_dialog->set_text("Enter name of new project:");
    connect(temp_dialog, SIGNAL(name_readed(QString)),this,SLOT(add_new_project_to_organization(QString)));
    temp_dialog->setWindowTitle("New Project Name");
    temp_dialog->show();
    }
}

void OrganizationsWindow::add_new_project_to_organization(QString a)
{
    addProjectToOrganization(a);
}

void OrganizationsWindow::addProjectToOrganization(QString new_project)
{
    QJsonObject jsonObject;
    jsonObject["req-type"] = "add-project-to-org";
    jsonObject["projname"] = new_project;
    jsonObject["orgname"] = this_org.orgGetName();
    QJsonDocument jsonDocument(jsonObject);
    QString req = jsonDocument.toJson(QJsonDocument::Compact);
    sendRequest(req);
}

void OrganizationsWindow::on_edit_project_BTN_clicked()
{
    if (this_user.orgPerGetIsAdmin() == false)
    {
            QMessageBox::warning(this, "admin permission", "You are not the organization admin!");
    }
    else
    {
        QTreeWidgetItem *item = ui->projects_tree_widget->currentItem();
        if(item!=nullptr)
        {
            temp_project.projSetName(ui->projects_tree_widget->currentItem()->text(0));
            OrgDialog *temp_dialog = new OrgDialog(this);
            temp_dialog->type="PROJECT_DIALOG";
            temp_dialog->ORG = this_org.orgGetName();
            temp_dialog->set_text("Enter new name of project:");
            connect(temp_dialog, SIGNAL(name_readed(QString)),this, SLOT(edit_project_in_organization(QString)));
            temp_dialog->setWindowTitle("Edit Project Name");
            temp_dialog->show();
        }
        else
        {
            QMessageBox::warning(this, "select a project", "select a project from the list!");
        }
    }
}

void OrganizationsWindow::edit_project_in_organization(QString n)
{
    editProjectinOrganization(temp_project.projGetName(),n);
}

void OrganizationsWindow::editProjectinOrganization(QString old_name, QString new_name)
{
    QJsonObject jsonObject;
    jsonObject["req-type"] = "edit-project-in-org";
    jsonObject["proj_oldname"] = old_name;
    jsonObject["proj_newname"] = new_name;
    jsonObject["orgname"] = this_org.orgGetName();
    QJsonDocument jsonDocument(jsonObject);
    QString req = jsonDocument.toJson(QJsonDocument::Compact);
    sendRequest(req);
}

void OrganizationsWindow::on_remove_project_BTN_clicked()
{
    if (this_user.orgPerGetIsAdmin() == false)
    {
        QMessageBox::warning(this, "admin permission", "You are not the organization admin!");
    }
    else
    {
        QTreeWidgetItem *item = ui->projects_tree_widget->currentItem();
        if(item!=nullptr)
        {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Delete a Project", "Are you sure?\nDeleteing is not returnable!", QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::Yes)
            {
                QString name = item->text(0);
                removeProjectFromOrganization(name);
            }
        }
        else
        {
            QMessageBox::information(this, "select a project", "select a project first!");
        }
        item = nullptr;
    }
}

void OrganizationsWindow::removeProjectFromOrganization(QString project_name)
{
    QJsonObject jsonObject;
    jsonObject["req-type"] = "remove-project-from-org";
    jsonObject["projname"] = project_name;
    jsonObject["orgname"] = this_org.orgGetName();
    QJsonDocument jsonDocument(jsonObject);
    QString req = jsonDocument.toJson(QJsonDocument::Compact);
    sendRequest(req);
}

void OrganizationsWindow::search_org_project()
{
    QString s = ui->search_projects_line_edit->text();
    for (int i = 0; i < ui->projects_tree_widget->topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = ui->projects_tree_widget->topLevelItem(i);
        if (item->text(0).startsWith(s)) {
            item->setHidden(false);
        } else {
            item->setHidden(true);
        }
    }
}

void OrganizationsWindow::closeEvent(QCloseEvent *event)
{
    qDebug() << "closing" ;
    ToDoList * t = new ToDoList();
    t->show();
    socket->close();
    t->connectionMaker(this_ip,this_port);
    t->thisUserMaker(this_user.perGetUsername(),this_user.perGetId(),this_user.perGetName());
    t->loadOrganizations();
}


void OrganizationsWindow::on_search_projects_line_edit_textChanged(const QString &arg1)
{
    search_org_project();
}


void OrganizationsWindow::on_tabWidget_currentChanged(int index)
{
    ui->projects_tree_widget->setCurrentItem(nullptr);
    ui->teams_tree_widget->setCurrentItem(nullptr);
    ui->org_persons_tree_widget->setCurrentItem(nullptr);
}


void OrganizationsWindow::on_teams_tree_widget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if (this_user.orgPerGetIsAdmin() == true)
    {
        org_team_show_to_org_admin();
    }
    else
    {
        QJsonObject jsonObject;
        jsonObject["req-type"] = "check-org-team-access";
        jsonObject["teamname"] = item->text(0);
        jsonObject["orgname"] = this_org.orgGetName();
        jsonObject["username"] = this_user.perGetUsername();
        QJsonDocument jsonDocument(jsonObject);
        QString req = jsonDocument.toJson(QJsonDocument::Compact);
        sendRequest(req);
    }
}


void OrganizationsWindow::org_team_show(QString user, QString name, QString id, QString org_id, QString team_id , bool team_admin , bool org_admin)
{
    TeamsWindow * t = new TeamsWindow (this);
    t->this_org_maker(this_org.orgGetName());
    t->this_team_maker(ui->teams_tree_widget->currentItem()->text(0));
    t->this_team_id_maker(ui->teams_tree_widget->currentItem()->text(1));
    t->admin_access(team_admin,false);
    t->setWindowTitle("Team Management");
    t->this_user_maker(this_user.perGetUsername());
    t->show();
    socket->disconnect();
    qDebug() << "org win Disconnected!";
    t->connectionMaker(this_ip,this_port);
    connect(t,SIGNAL(reconnect(QString,int)),this,SLOT(connectionMaker(QString,int)));
    t->loadTeamPersons();
}

void OrganizationsWindow::org_team_show_to_org_admin()
{
    TeamsWindow * t = new TeamsWindow (this);
    t->this_org_maker(this_org.orgGetName());
    t->this_team_maker(ui->teams_tree_widget->currentItem()->text(0));
    t->this_team_id_maker(ui->teams_tree_widget->currentItem()->text(1));
    t->admin_access(true,true);
    t->setWindowTitle("Team Management");
    t->this_user_maker(this_user.perGetUsername());
    t->show();
    socket->disconnect();
    qDebug() << "org win Disconnected!";
    t->connectionMaker(this_ip,this_port);
    connect(t,SIGNAL(reconnect(QString,int)),this,SLOT(connectionMaker(QString,int)));
    t->loadTeamPersons();
}


void OrganizationsWindow::on_projects_tree_widget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if (this_user.orgPerGetIsAdmin() == true)
    {
        show_org_project_to_admin(item->text(0));
    }
    else if(this_user.perGetUsername() == item->text(1))
    {
        show_org_project_to_project_admin (item->text(0));
    }
    else
    {
        QJsonObject jsonObject;
        jsonObject["req-type"] = "check-org-project-access";
        jsonObject["projectname"] = item->text(0);
        jsonObject["orgname"] = this_org.orgGetName();
        jsonObject["username"] = this_user.perGetUsername();
        QJsonDocument jsonDocument(jsonObject);
        QString req = jsonDocument.toJson(QJsonDocument::Compact);
        sendRequest(req);
    }
}


void OrganizationsWindow:: show_org_project_to_project_admin (QString proj_name)
{
    qDebug() << "State = project admin";
    ProjectsWindow * t = new ProjectsWindow (this);
    t->setWindowTitle("Project Management");
    t->this_org_maker(this_org.orgGetName());
    t->this_project_maker(proj_name);
    t->thisProjectShowAdmin(ui->projects_tree_widget->currentItem()->text(1));
    t->project_this_person_maker(this_user);
    t->project_admin_access_maker(true);
    t->show();

    socket->disconnect();
    qDebug() << "org win Disconnected!";
    t->connectionMaker(this_ip,this_port);
    connect(t,SIGNAL(reconnect(QString,int)),this,SLOT(connectionMaker(QString,int)));

}


void OrganizationsWindow:: show_org_project_to_admin (QString proj_name)
{
    qDebug() << "State = org admin";
    ProjectsWindow * t = new ProjectsWindow (this);
    t->setWindowTitle("Project Management");
    t->this_org_maker(this_org.orgGetName());
    t->this_project_maker(proj_name);
    t->thisProjectShowAdmin(ui->projects_tree_widget->currentItem()->text(0));
    t->project_this_person_maker(this_user);
    t->project_admin_access_maker(true);
    t->show();

    socket->disconnect();
    qDebug() << "org win Disconnected!";
    t->connectionMaker(this_ip,this_port);
    connect(t,SIGNAL(reconnect(QString,int)),this,SLOT(connectionMaker(QString,int)));

}


void OrganizationsWindow:: show_org_project_to_member (QString username , QString name, QString id, QString org_id, bool is_member , bool is_team_member)
{
    if (is_member == true)
    {
        qDebug() << "State = project member";
    }
    else
    {
    qDebug() << "State = project Team member";
    }

    ProjectsWindow * t = new ProjectsWindow (this);
    t->setWindowTitle("Project Management");
    t->this_org_maker(this_org.orgGetName());
    t->this_project_maker(ui->projects_tree_widget->currentItem()->text(0));
    t->thisProjectShowAdmin(ui->projects_tree_widget->currentItem()->text(1));
    t->project_this_person_maker(this_user);
    t->project_admin_access_maker(false);
    t->show();

    socket->disconnect();
    qDebug() << "org win Disconnected!";
    t->connectionMaker(this_ip,this_port);
    connect(t,SIGNAL(reconnect(QString,int)),this,SLOT(connectionMaker(QString,int)));
}
