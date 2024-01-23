#include "organizationswindow.h"
#include "ui_organizationswindow.h"

OrganizationsWindow::OrganizationsWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::OrganizationsWindow)
{
    makeAllpersonsFile();
    ui->setupUi(this);
}


OrganizationsWindow::~OrganizationsWindow()
{
    delete ui;
}


void OrganizationsWindow::addUserToOrganization(QString input_user)
{
    QString per_file = QDir::currentPath() + "/APPDATA/ALL_PERSONS.json";
    QFile file(per_file);
    if (!file.open(QIODevice::ReadOnly))
    {
        return;
    }
    
    QByteArray jsonData = file.readAll();
    file.close();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    QJsonArray jsonArray = jsonDoc.array();
    
    for (int i = 0; i < jsonArray.size(); ++i)
    {
        QJsonObject jsonObject = jsonArray.at(i).toObject();

        QString username = jsonObject.value("username").toString();
        QString name = jsonObject.value("name").toString();
        QString id = jsonObject.value("id").toString();

        if (username == input_user)
        {
            temp_org_person.perSetID(id);
            temp_org_person.perSetName(name);
            temp_org_person.perSetUsername(username);
            temp_org_person.orgPerSetID(temp_org_person.orgIdGenerator());
            temp_org_person.orgPerSetIsAdmin(false);
            break;
        }
    }

    QString file_Path2 = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/"+this_org.orgGetName()+"/ORG_PERSONS.json";
    QFile file2(file_Path2);
    file2.open(QIODevice::ReadWrite);

    QByteArray jsonData2 = file2.readAll();
    QJsonDocument jsonDoc2 = QJsonDocument::fromJson(jsonData2);
    QJsonArray jsonArray2 = jsonDoc2.array();

    for (int i = 0; i < jsonArray2.size(); ++i)
    {
        QJsonObject jsonObject = jsonArray2.at(i).toObject();
        if (jsonObject.value("username").toString() == input_user)
        {
            QMessageBox ::warning(this, "Error!" , "This user already added to this organization!");
            file.close();
            return;
        }
    }

    QJsonObject newJsonObject;
    newJsonObject["id"] = temp_org_person.perGetId();
    newJsonObject["org_id"] = temp_org_person.orgPerGetId();
    newJsonObject["name"] = temp_org_person.perGetName();
    newJsonObject["username"] = temp_org_person.perGetUsername();
    newJsonObject["is_admin"] = temp_org_person.orgPerGetIsAdmin();

    jsonArray2.append(newJsonObject);
    jsonDoc.setArray(jsonArray2);
    file2.resize(0);
    file2.write(jsonDoc.toJson());
    file2.close();
}


void OrganizationsWindow::removeUserFromOrganization(QString user)
{
    QString file_Path = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/"+this_org.orgGetName()+"/ORG_PERSONS.json";
    QFile file(file_Path);
    file.open(QIODevice::ReadWrite);

    QByteArray jsonData = file.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(jsonData));
    QJsonArray jsonArray = doc.array();

    for (int i = 0; i < jsonArray.size(); i++) {
        QJsonObject jsonObject = jsonArray[i].toObject();
        if (jsonObject["username"].toString() == user) {
            jsonArray.removeAt(i);
            break;
        }
    }

    file.resize(0);
    file.write(QJsonDocument(jsonArray).toJson());
    file.close();

    QString file_Path2 = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/"+this_org.orgGetName()+"/ORG_TEAMS.json";
    QFile file2(file_Path2);
    if (!file2.open(QIODevice::ReadOnly)) {
        return;
    }
    QByteArray jsonData2 = file2.readAll();
    file2.close();

    QJsonDocument doc2 = QJsonDocument::fromJson(jsonData2);
    QJsonArray jsonArray2 = doc2.array();
    for (int i = 0; i < jsonArray2.size(); ++i)
    {
        QJsonObject jsonObject2 = jsonArray2.at(i).toObject();
        QString name = jsonObject2.value("team_name").toString();
        TeamsWindow * w = new TeamsWindow(this);
        connect (this,SIGNAL(team_name_signal(QString)),w,SLOT(this_team_maker(QString)));
        connect (this,SIGNAL(org_name_signal(QString)),w,SLOT(this_org_maker(QString)));
        emit team_name_signal(name);
        emit org_name_signal(this_org.orgGetName());
        w->removeTeamPerson(user);
    }


    QString file_Path3 = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/"+this_org.orgGetName()+"/ORG_PROJECTS.json";
    QFile file3(file_Path3);
    if (!file3.open(QIODevice::ReadOnly)) {
        return;
    }
    QByteArray jsonData3 = file3.readAll();
    file3.close();

    QJsonDocument doc3 = QJsonDocument::fromJson(jsonData3);
    QJsonArray jsonArray3 = doc3.array();
    for (int i = 0; i < jsonArray3.size(); ++i)
    {
        QJsonObject jsonObject3 = jsonArray3.at(i).toObject();
        QString name = jsonObject3.value("project_name").toString();
        ProjectsWindow * w = new ProjectsWindow(this);
        connect (this,SIGNAL(project_name_signal(QString)),w,SLOT(this_project_maker(QString)));
        connect (this,SIGNAL(org_name_signal(QString)),w,SLOT(this_org_maker(QString)));
        emit project_name_signal(name);
        emit org_name_signal(this_org.orgGetName());
        w->removeProjectPerson(user);
    }

}


void OrganizationsWindow::promoteToAdmin(QString user)
{
    QString file_Path = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/"+this_org.orgGetName()+"/ORG_PERSONS.json";
    QFile file(file_Path);
    file.open(QIODevice::ReadOnly);

    QByteArray fileData = file.readAll();
    file.close();

    QJsonDocument jsonDocument = QJsonDocument::fromJson(fileData);

    if (jsonDocument.isArray()) {
        QJsonArray jsonArray = jsonDocument.array();

        for (int i = 0; i < jsonArray.size(); i++)
        {
            QJsonObject jsonObject = jsonArray[i].toObject();

            if (jsonObject["username"].toString() == user)
            {
                if (jsonObject["is_admin"] == true)
                {
                    jsonObject["is_admin"] = false;
                }
                else
                {
                    jsonObject["is_admin"] = true;
                }
                jsonArray[i] = jsonObject;
                break;
            }
        }

        QByteArray updatedData = QJsonDocument(jsonArray).toJson(QJsonDocument::Indented);

        file.open(QIODevice::WriteOnly);
        file.write(updatedData);
        file.close();
    }
    on_org_users_search_line_edit_textChanged(0);


    QString file_Path2 = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/"+this_org.orgGetName()+"/ORG_TEAMS.json";
    QFile file2(file_Path2);
    file2.open(QIODevice::ReadOnly);
    QByteArray jsonData2 = file2.readAll();
    file2.close();

    QJsonDocument doc2 = QJsonDocument::fromJson(jsonData2);
    QJsonArray jsonArray2 = doc2.array();
    for (int i = 0; i < jsonArray2.size(); ++i)
    {
        QJsonObject jsonObject2 = jsonArray2.at(i).toObject();
        QString name = jsonObject2.value("team_name").toString();
        TeamsWindow * w = new TeamsWindow(this);
        connect (this,SIGNAL(team_name_signal(QString)),w,SLOT(this_team_maker(QString)));
        connect (this,SIGNAL(org_name_signal(QString)),w,SLOT(this_org_maker(QString)));
        emit team_name_signal(name);
        emit org_name_signal(this_org.orgGetName());
        w->changeOrgPersonRole(user);
    }


}


void OrganizationsWindow::this_org_maker(QString a)
{
    this_org.orgSetName(a);
    ui->org_name_lable->setText(this_org.orgGetName());
}


void OrganizationsWindow::makeAllpersonsFile()
{
    QString file_Path = QDir::currentPath() + "/APPDATA/ALL_PERSONS.json";
    QFile file(file_Path);
    file.open(QFile::ReadOnly);
    if (!file.exists())
    {
        QFile n_file(file_Path);
        n_file.open(QFile::WriteOnly);
        n_file.close();
    }
    file.close();
}


void OrganizationsWindow::on_org_page_add_person_BTN_clicked()
{
    allServerUsers * a = new allServerUsers(this);
    connect (a,SIGNAL(user_selected(QString)),this,SLOT(add_new_person_to_organization(QString)));
    a->btn_disable();
    a->setWindowTitle("Add New Person");
    a->show();
}


void OrganizationsWindow::add_new_person_to_organization(QString a)
{
    addUserToOrganization(a);
    loadAllOrgPersons();
}


void OrganizationsWindow::loadAllOrgPersons()
{
    ui->org_persons_tree_widget->clear();
    QString file_Path = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/" + this_org.orgGetName() + "/ORG_PERSONS.json";
    QFile file(file_Path);
    file.open(QIODevice::ReadOnly);

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument doc(QJsonDocument::fromJson(jsonData));
    QJsonArray jsonArray = doc.array();

    QMap<QString, QTreeWidgetItem*> uniqueItems;

    for (int i = 0; i < jsonArray.size(); ++i) {
        QJsonObject jsonObject = jsonArray[i].toObject();

        QString name = jsonObject["name"].toString();
        QString username = jsonObject["username"].toString();
        QString orgid = jsonObject["org_id"].toString();
        QString id = jsonObject["id"].toString();

        QString is_admin;

        if(jsonObject["is_admin"] == false)
        {
            is_admin = "NO";
        }
        else
        {
            is_admin = "YES";
        }

        QTreeWidgetItem *item = new QTreeWidgetItem(ui->org_persons_tree_widget);
        item->setText(0, name);
        item->setText(1, username);
        item->setText(2, orgid);
        item->setText(3, is_admin);

        uniqueItems.insert(id, item);
        uniqueItems.insert(username, item);
    }
}


void OrganizationsWindow::on_org_page_edit_person_BTN_clicked()
{
    QTreeWidgetItem *selectedItem = ui->org_persons_tree_widget->currentItem();
    if (selectedItem)
    {
        if(ui->org_persons_tree_widget->currentItem()->text(3)=="YES")
            QMessageBox::information(this, "demoted!", "User demoted to member!");
        else
            QMessageBox::information(this, "Promoted!", "User promoted to admin!");
        QString selecteduser = selectedItem->text(1);
        promoteToAdmin(selecteduser);
        loadAllOrgPersons();
    }
    else
    QMessageBox::warning(this, "Select a person", "select a person from the list!");
}


void OrganizationsWindow::on_org_page_remove_person_BTN_clicked()
{
    QTreeWidgetItem *selectedItem = ui->org_persons_tree_widget->currentItem();
    if (selectedItem)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Delete a person", "Are you sure?\nDeleteing is not returnable!", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
        QString selecteduser = selectedItem->text(1);
        removeUserFromOrganization(selecteduser);
        loadAllOrgPersons();
        }
    }
    else
    QMessageBox::warning(this, "Select a person", "select a person from the list!");
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


void OrganizationsWindow::on_add_team_BTN_clicked()
{
    OrgDialog *temp_dialog = new OrgDialog(this);
    temp_dialog->type="TEAM_DIALOG";
    temp_dialog->ORG = this_org.orgGetName();
    temp_dialog->set_text("Enter name of new team:");
    connect(temp_dialog, SIGNAL(name_readed(QString)),this,SLOT(add_new_team_to_organization(QString)));
    temp_dialog->setWindowTitle("New Team Name");
    temp_dialog->show();
}


void OrganizationsWindow::add_new_team_to_organization(QString a)
{
    ui->teams_list_widget->addItem(a);
    addTeamToOrganization(a);
    loadAllOrgTeams();
}


void OrganizationsWindow::addTeamToOrganization(QString new_team)
{
    team.teamSetName(new_team);
    team.teamSetId(team.teamIdGenerator());
    QString file_Path = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/"+ this_org.orgGetName()+ "/ORG_TEAMS.json" ;
    QString team_file = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/" + this_org.orgGetName() + "/ORG_TEAMS/" +team.teamGetName()+".json" ;

    QFile team_js_File(team_file);
    if (!team_js_File.exists())
    {
        QFile n_file(team_file);
        n_file.open(QFile::WriteOnly);
        n_file.close();
    }

    QFile file(file_Path);
    file.open(QIODevice::ReadOnly);

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument jsonDoc(QJsonDocument::fromJson(jsonData));

    QJsonArray jsonArray = jsonDoc.array();

    QJsonObject newObj;
    newObj.insert("team_name", team.teamGetName());
    newObj.insert("team_id", team.teamGetid());

    jsonArray.append(newObj);
    QJsonDocument updatedJsonDoc(jsonArray);

    file.open(QIODevice::WriteOnly);
    file.write(updatedJsonDoc.toJson());
    file.close();
}


void OrganizationsWindow::on_remove_team_BTN_clicked()
{
    QListWidgetItem *item = ui->teams_list_widget->currentItem();
    if(item!=nullptr)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Delete a team", "Are you sure?\nDeleteing is not returnable!", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            QString name = item->text();
            removeTeamFromOrganization(name);
        }
    }
    else
    {
        QMessageBox::information(this, "select a team", "select a team first!");
    }
    loadAllOrgTeams();
}


void OrganizationsWindow::removeTeamFromOrganization(QString team_name)
{
    QString team_file = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/" + this_org.orgGetName() + "/ORG_TEAMS/" + team_name +".json" ;

    QFile team_js_File(team_file);
    if (team_js_File.exists())
    {
        team_js_File.remove();
    }
    QString file_Path = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/"+ this_org.orgGetName()+ "/ORG_TEAMS.json" ;

    QFile file(file_Path);
    file.open(QIODevice::ReadOnly);

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument jsonDoc(QJsonDocument::fromJson(jsonData));

    QJsonArray jsonArray = jsonDoc.array();

    for (int i = 0; i < jsonArray.size(); i++) {
        QJsonObject obj = jsonArray.at(i).toObject();
        if (obj.value("team_name").toString() == team_name) {
            jsonArray.removeAt(i);
            i--;
        }
    }

    QJsonDocument updatedJsonDoc(jsonArray);
    file.open(QIODevice::WriteOnly);
    file.write(updatedJsonDoc.toJson());
    file.close();
}


void OrganizationsWindow::loadAllOrgTeams()
{
    ui->teams_list_widget->clear();
    QString file_Path = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/" + this_org.orgGetName() + "/ORG_TEAMS.json" ;

    QFile file(file_Path);
    if (!file.open(QIODevice::ReadOnly))
    {
        return;
    }

    QByteArray jsonData = file.readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);

    QJsonArray jsonArray = jsonDoc.array();

    foreach (const QJsonValue &value, jsonArray) {
        QJsonObject obj = value.toObject();
        QString teamName = obj.value("team_name").toString();
        bool exists = false;
        for (int i = 0; i < ui->teams_list_widget->count(); i++) {
            if (ui->teams_list_widget->item(i)->text() == teamName) {
                exists = true;
                break;
            }
        }

        if (!exists) {
            ui->teams_list_widget->addItem(teamName);
        }
    }
}


void OrganizationsWindow::on_edit_team_name_BTN_clicked()
{
    QListWidgetItem *item = ui->teams_list_widget->currentItem();
    if(item!=nullptr)
    {
        team.teamSetName(ui->teams_list_widget->currentItem()->text());
            OrgDialog *temp_dialog = new OrgDialog(this);
            temp_dialog->type="TEAM_DIALOG";
            temp_dialog->ORG = this_org.orgGetName();
            temp_dialog->set_text("Enter name of new team:");
            connect(temp_dialog, SIGNAL(name_readed(QString)),this, SLOT(edit_team_in_organization(QString)));
            temp_dialog->setWindowTitle("Edit Team Name");
            temp_dialog->show();
    }
    else
    {
    QMessageBox::information(this, "select a team", "select a team first!");
    }
}


void OrganizationsWindow::edit_team_in_organization(QString a)
{
    editTeaminOrganization(team.teamGetName(),a);
    loadAllOrgTeams();
}


void OrganizationsWindow::editTeaminOrganization(QString old_name,QString new_name)
{
    QString team_file = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/" + this_org.orgGetName() + "/ORG_TEAMS/" + old_name + ".json" ;
    QString new_team_file = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/"+ this_org.orgGetName() + "/ORG_TEAMS/" + new_name + ".json" ;
    QFile nfile(team_file);
    nfile.rename(new_team_file);
    nfile.close();

    QString file_Path = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/" + this_org.orgGetName() + "/ORG_TEAMS.json" ;
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
        QString teamName = jsonObject.value("team_name").toString();
        listWidget.addItem(teamName);
    }

    int selectedIndex = ui->teams_list_widget->currentRow();
    if (selectedIndex < 0 || selectedIndex >= jsonArray.size())
        return;

    QJsonObject jsonObject = jsonArray.at(selectedIndex).toObject();
    jsonObject["team_name"] = new_name;
    jsonArray[selectedIndex] = jsonObject;

    QFile saveFile(file_Path);
    if (!saveFile.open(QIODevice::WriteOnly ))
        return;

    QJsonDocument jsonDoc2(jsonArray);
    saveFile.write(jsonDoc2.toJson());
    saveFile.close();
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



void OrganizationsWindow::search_org_team()
{
    QString s = ui->search_teams_line_edit->text();
    for (int i = 0; i < ui->teams_list_widget->count(); ++i) {
        QListWidgetItem *item = ui->teams_list_widget->item(i);
        if (item->text().startsWith(s)) {
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


void OrganizationsWindow::on_sort_teams_BTN_clicked()
{
    ui->teams_list_widget->sortItems();
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
    ui->projects_list_widget->addItem(a);
    addProjectToOrganization(a);
    loadAllOrgProjects();
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
    makeAllProjectsFile(new_project);
}



void OrganizationsWindow::loadAllOrgProjects()
{
    ui->projects_list_widget->clear();
    QString file_Path = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/" + this_org.orgGetName() + "/ORG_PROJECTS.json" ;

    QFile file(file_Path);
    if (!file.open(QIODevice::ReadOnly))
    {
        return;
    }

    QByteArray jsonData = file.readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);

    QJsonArray jsonArray = jsonDoc.array();

    foreach (const QJsonValue &value, jsonArray) {
        QJsonObject obj = value.toObject();
        QString teamName = obj.value("project_name").toString();
        bool exists = false;
        for (int i = 0; i < ui->projects_list_widget->count(); i++) {
            if (ui->projects_list_widget->item(i)->text() == teamName) {
                exists = true;
                break;
            }
        }

        if (!exists) {
            ui->projects_list_widget->addItem(teamName);
        }
    }
}


void OrganizationsWindow::on_edit_project_BTN_clicked()
{
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
}

void OrganizationsWindow::edit_project_in_organization(QString n)
{
    editProjectinOrganization(temp_project.projGetName(),n);
    loadAllOrgProjects();
}


void OrganizationsWindow::editProjectinOrganization(QString old_name, QString new_name)
{
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
}


void OrganizationsWindow::on_remove_project_BTN_clicked()
{
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
}

void OrganizationsWindow::makeAllProjectsFile(QString new_project)
{
    QString file_Path1 = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/"+ this_org.orgGetName()+ "/ORG_PROJECTS/" + new_project +"/PROJECT_PERSON.json" ;
    QString file_Path2 = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/"+ this_org.orgGetName()+ "/ORG_PROJECTS/" + new_project +"/PROJECT_TASKS.json" ;
    QString file_Path3 = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/"+ this_org.orgGetName()+ "/ORG_PROJECTS/" + new_project +"/PROJECT_TEAMS.json" ;
    QString file_Path4 = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/"+ this_org.orgGetName()+ "/ORG_PROJECTS/" + new_project +"/PROJECT_TASKS" ;

    QFile file1(file_Path1);
    if (!file1.exists()) {
        file1.open(QIODevice::WriteOnly);
    }
    file1.close();

    QFile file2(file_Path2);
    if (!file2.exists()) {
        file2.open(QIODevice::WriteOnly);
    }
    file2.close();

    QFile file3(file_Path3);
    if (!file3.exists()) {
        file3.open(QIODevice::WriteOnly);
    }
    file3.close();
    QDir sDir(file_Path4);
    if (!sDir.exists())
    {
        sDir.mkpath(".");
    }
}



void OrganizationsWindow::on_search_projects_line_edit_textChanged(const QString &arg1)
{
    search_org_project();
}


void OrganizationsWindow::on_sort_projects_BTN__clicked()
{
    ui->projects_list_widget->sortItems();
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

