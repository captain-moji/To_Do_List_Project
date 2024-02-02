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


}



void OrganizationsWindow::this_org_maker(QString a)
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

void OrganizationsWindow::on_teams_list_widget_itemDoubleClicked(QListWidgetItem *item)
{
    TeamsWindow * t = new TeamsWindow (this);
    connect (this,SIGNAL(team_name_signal(QString)),t,SLOT(this_team_maker(QString)));
    connect (this,SIGNAL(org_name_signal(QString)),t,SLOT(this_org_maker(QString)));
    connect (this,SIGNAL(team_id_signal(QString)),t,SLOT(this_team_id_maker(QString)));
    emit team_name_signal(item->text());
    emit org_name_signal(this_org.orgGetName());
    emit team_id_signal(getTeamIdByName(item->text()));
    t->setWindowTitle("Team Management");
    t->show();
    t->loadTeamPersons();
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
    OrgDialog *temp_dialog = new OrgDialog(this);
    temp_dialog->type="PROJECT_DIALOG";
    temp_dialog->ORG = this_org.orgGetName();
    temp_dialog->set_text("Enter name of new project:");
    connect(temp_dialog, SIGNAL(name_readed(QString)),this,SLOT(add_new_project_to_organization(QString)));
    temp_dialog->setWindowTitle("New Project Name");
    temp_dialog->show();
}

void OrganizationsWindow::add_new_project_to_organization(QString a)
{
    /*
    ui->projects_list_widget->addItem(a);
    addProjectToOrganization(a);
    loadAllOrgProjects();
*/
}



void OrganizationsWindow::addProjectToOrganization(QString new_project)
{
    temp_project.projSetName(new_project);
    temp_project.projSetAdminId("");
    QString file_Path = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/"+ this_org.orgGetName()+ "/ORG_PROJECTS.json" ;
    QString project_folder = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/" + this_org.orgGetName() + "/ORG_PROJECTS/" + temp_project.projGetName();

    QDir sDir2(project_folder);
    if (!sDir2.exists())
    {
        sDir2.mkpath(".");
    }

    QFile file(file_Path);
    file.open(QIODevice::ReadOnly);

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument jsonDoc(QJsonDocument::fromJson(jsonData));

    QJsonArray jsonArray = jsonDoc.array();

    QJsonObject newObj;
    newObj.insert("project_name", temp_project.projGetName());
    newObj.insert("project_admin_id", temp_project.projGetAdminId());

    jsonArray.append(newObj);
    QJsonDocument updatedJsonDoc(jsonArray);

    file.open(QIODevice::WriteOnly);
    file.write(updatedJsonDoc.toJson());
    file.close();
}


void OrganizationsWindow::on_edit_project_BTN_clicked()
{/*

    QListWidgetItem *item = ui->projects_list_widget->currentItem();
    if(item!=nullptr)
    {
        temp_project.projSetName(ui->projects_list_widget->currentItem()->text());
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
        QMessageBox::information(this, "select a project", "select a project first!");
    }
    loadAllOrgPersons();
*/
}

void OrganizationsWindow::edit_project_in_organization(QString n)
{
    editProjectinOrganization(temp_project.projGetName(),n);
    loadAllOrgProjects();
}


void OrganizationsWindow::editProjectinOrganization(QString old_name, QString new_name)
{
    /*
    QString project_folder = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/" + this_org.orgGetName() + "/ORG_PROJECTS/" + old_name ;
    QString new_project_folder = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/"+ this_org.orgGetName() + "/ORG_PROJECTS/" + new_name ;
    QDir sDir(project_folder);
    if(sDir.exists())
    {
        sDir.rename(project_folder,new_project_folder);
    }

    QString file_Path = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/" + this_org.orgGetName() + "/ORG_PROJECTS.json" ;
    QFile file(file_Path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return ;

    QString jsonContent = file.readAll();
    file.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonContent.toUtf8());
    QJsonArray jsonArray = jsonDoc.array();

    QListWidget listWidget;
    for (int i = 0; i < jsonArray.size(); i++)
    {
        QJsonObject jsonObject = jsonArray.at(i).toObject();
        QString teamName = jsonObject.value("project_name").toString();
        listWidget.addItem(teamName);
    }

    int selectedIndex = ui->projects_list_widget->currentRow();
    if (selectedIndex < 0 || selectedIndex >= jsonArray.size())
        return;

    QJsonObject jsonObject = jsonArray.at(selectedIndex).toObject();
    jsonObject["project_name"] = new_name;
    jsonArray[selectedIndex] = jsonObject;

    QFile saveFile(file_Path);
    if (!saveFile.open(QIODevice::WriteOnly ))
        return;

    QJsonDocument jsonDoc2(jsonArray);
    saveFile.write(jsonDoc2.toJson());
    saveFile.close();
*/
}


void OrganizationsWindow::on_remove_project_BTN_clicked()
{
    /*
    QListWidgetItem *item = ui->projects_list_widget->currentItem();
    if(item!=nullptr)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Delete a project", "Are you sure?\nDeleteing is not returnable!", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            QString name = item->text();
            removeProjectFromOrganization(name);
        }
    }
    else
    {
        QMessageBox::information(this, "select a project", "select a project first!");
    }
    loadAllOrgProjects();
*/
}




void OrganizationsWindow::removeProjectFromOrganization(QString project_name)
{
    QString project_folder = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/" + this_org.orgGetName() + "/ORG_PROJECTS/" + project_name ;

    QDir sDir(project_folder);
    if(sDir.exists())
    {
        sDir.removeRecursively();
    }

    QString file_Path = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/"+ this_org.orgGetName()+ "/ORG_PROJECTS.json" ;

    QFile file(file_Path);
    file.open(QIODevice::ReadOnly);

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument jsonDoc(QJsonDocument::fromJson(jsonData));

    QJsonArray jsonArray = jsonDoc.array();

    for (int i = 0; i < jsonArray.size(); i++) {
        QJsonObject obj = jsonArray.at(i).toObject();
        if (obj.value("project_name").toString() == project_name) {
            jsonArray.removeAt(i);
            i--;
        }
    }

    QJsonDocument updatedJsonDoc(jsonArray);
    file.open(QIODevice::WriteOnly);
    file.write(updatedJsonDoc.toJson());
    file.close();
}

void OrganizationsWindow::search_org_project()
{
    /*
    QString s = ui->search_projects_line_edit->text();
    for (int i = 0; i < ui->projects_list_widget->count(); ++i)
    {
        QListWidgetItem *item = ui->projects_list_widget->item(i);
        if (item->text().startsWith(s)) {
            item->setHidden(false);
        } else {
            item->setHidden(true);
        }
    }
    */
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


void OrganizationsWindow::on_sort_projects_BTN__clicked()
{
    //ui->projects_list_widget->sortItems();
}


void OrganizationsWindow::on_projects_list_widget_itemDoubleClicked(QListWidgetItem *item)
{
    ProjectsWindow * t = new ProjectsWindow (this);
    connect (this,SIGNAL(project_name_signal(QString)),t,SLOT(this_project_maker(QString)));
    connect (this,SIGNAL(org_name_signal(QString)),t,SLOT(this_org_maker(QString)));
    emit project_name_signal(item->text());
    emit org_name_signal(this_org.orgGetName());
    t->setWindowTitle("Project Management");
    t->show();
    t->loadProjectTeams();
    t->loadOrgTeamsComboBox();
    t->thisProjectShowAdmin();
    t->loadProjectPersons();
}



