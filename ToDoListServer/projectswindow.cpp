#include "projectswindow.h"
#include "ui_projectswindow.h"

ProjectsWindow::ProjectsWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ProjectsWindow)
{
    setWindowTitle("Project Management");
    ui->setupUi(this);
}

ProjectsWindow::~ProjectsWindow()
{
    delete ui;
}



void ProjectsWindow::this_org_maker(QString a)
{
    this_org = a;
}

void ProjectsWindow::this_project_maker(QString a)
{
    this_project.projSetName(a);
    ui->projects_name_text->setText(a);
}

void ProjectsWindow::thisProjectShowAdmin()
{
    QString filePath = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/"+this_org+"/ORG_PROJECTS.json";

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    QJsonArray array = doc.array();
    QString projectAdminId;

    for (int i = 0; i < array.size(); i++)
    {
        QJsonObject obj = array.at(i).toObject();
        if (obj.value("project_name").toString() == this_project.projGetName())
        {
            projectAdminId = obj.value("project_admin_id").toString();
            break;
        }
    }


    QString username;
    QString filePath2 = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/"+this_org+"/ORG_PROJECTS/" +this_project.projGetName() + "/PROJECT_PERSON.json";
    QFile file2(filePath2);
    if (!file2.open(QIODevice::ReadOnly))
    {
        return ;
    }

    QByteArray jsonData = file2.readAll();
    file2.close();
    QJsonDocument doc2 = QJsonDocument::fromJson(jsonData);

    QJsonArray jsonArray = doc2.array();

    for (int i = 0; i < jsonArray.size(); ++i) {
        QJsonObject jsonObject = jsonArray.at(i).toObject();
        if (jsonObject.value("id").toString() == projectAdminId) {
            username= jsonObject.value("username").toString();
        }
    }
  ui->Project_admin_text->setText(username);
}


void ProjectsWindow::on_projects_add_person_BTN_clicked()
{
    OrganizationPersonsWindow *o = new OrganizationPersonsWindow(this);
    connect (o,SIGNAL(org_person_selected(QString)),this,SLOT(add_project_person(QString)));
    o->ORG = this_org;
    o->setWindowTitle("Add New Person");
    o->show();
    o->loadOrgPersons();
}

void ProjectsWindow::add_project_person(QString new_username)
{
    addNewProjectPerson(new_username);
    loadProjectPersons();
}


void ProjectsWindow::addNewProjectPerson(QString new_username)
{
    QString file_Path = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/" + this_org + "/ORG_PERSONS.json";
    QFile file(file_Path);
    file.open(QIODevice::ReadOnly);

    QByteArray jsonData = file.readAll();
    QJsonDocument jsonDoc(QJsonDocument::fromJson(jsonData));
    QJsonArray jsonArray = jsonDoc.array();

    for (const QJsonValue& jsonValue : jsonArray)
    {
        QJsonObject jsonObject = jsonValue.toObject();
        if (jsonObject.value("username").toString() == new_username)
        {
            temp_person.perSetName(jsonObject.value("name").toString());
            temp_person.perSetUsername(jsonObject.value("username").toString());
            temp_person.perSetID(jsonObject.value("id").toString());
            temp_person.orgPerSetID(jsonObject.value("org_id").toString());
            temp_person.orgPerSetIsAdmin(jsonObject.value("is_admin").toBool());
        }
    }


    QString file_Path2 = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/"+ this_org + "/ORG_PROJECTS/" + this_project.projGetName() + "/PROJECT_PERSON.json" ;

    QFile file2(file_Path2);
    file2.open(QIODevice::ReadWrite);

    QByteArray jsonData2 = file2.readAll();

    QJsonDocument jsonDoc2 = QJsonDocument::fromJson(jsonData2);

    QJsonArray jsonArray2 = jsonDoc2.array();

    for (int i = 0; i < jsonArray2.size(); ++i)
    {
        QJsonObject jsonObject = jsonArray2.at(i).toObject();
        if (jsonObject.value("username").toString() == new_username)
        {
            QMessageBox ::warning(this, "Error!" , "This user already added to this Project!");
            file.close();
            return;
        }
    }

    QJsonObject newJsonObject;
    newJsonObject["id"] = temp_person.perGetId();
    newJsonObject["org_id"] = temp_person.orgPerGetId();
    newJsonObject["name"] = temp_person.perGetName();
    newJsonObject["username"] = temp_person.perGetUsername();
    newJsonObject["is_org_admin"] = temp_person.orgPerGetIsAdmin();

    jsonArray2.append(newJsonObject);

    jsonDoc.setArray(jsonArray2);
    file2.resize(0);
    file2.write(jsonDoc.toJson());
    file2.close();
}



void ProjectsWindow::loadProjectPersons()
{
    ui->project_persons_tree_widget->clear();
    QString file_Path = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/"+ this_org+ "/ORG_PROJECTS/" + this_project.projGetName() +"/PROJECT_PERSON.json" ;
    QFile file(file_Path);
    if (!file.open(QIODevice::ReadOnly))
    {
        return;
    }
    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    QJsonArray jsonArray = jsonDoc.array();

    for (const QJsonValue& jsonValue : jsonArray) {
        if (!jsonValue.isObject()) {
            continue;
        }
        QJsonObject jsonObject = jsonValue.toObject();

        QString name = jsonObject.value("name").toString();
        QString username = jsonObject.value("username").toString();
        QString orgId = jsonObject.value("org_id").toString();

        bool isUsernameUnique = true;
        for (int i = 0; i < ui->project_persons_tree_widget->topLevelItemCount(); ++i) {
            QTreeWidgetItem* item = ui->project_persons_tree_widget->topLevelItem(i);
            if (item->text(1) == username) {
                isUsernameUnique = false;
                break;
            }
        }

        if (isUsernameUnique) {
            QTreeWidgetItem* item = new QTreeWidgetItem(ui->project_persons_tree_widget);
            item->setText(0, name);
            item->setText(1, username);
            item->setText(2, orgId);
        }
    }
}


void ProjectsWindow::on_projects_remove_person_BTN_clicked()
{
    if (ui->project_persons_tree_widget->currentItem()!=nullptr)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Delete a person", "Are you sure?\nDeleteing is not returnable!", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            removeProjectPerson(ui->project_persons_tree_widget->currentItem()->text(1));
            thisProjectShowAdmin();
            loadProjectPersons();
        }
    }
    else
    {
        QMessageBox::warning(this, "Select a person", "select a person from the list!");
    }
    ui->project_persons_tree_widget->clearSelection();
}


void ProjectsWindow::removeProjectPerson(QString username)
{
    QString filePath = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/"+this_org+"/ORG_PROJECTS/" +this_project.projGetName() + "/PROJECT_PERSON.json";

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        return;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);

    QJsonArray jsonArray = jsonDoc.array();

    for (int i = 0; i < jsonArray.size(); ++i) {
        QJsonValue jsonValue = jsonArray.at(i);
        QJsonObject jsonObject = jsonValue.toObject();

        if (jsonObject.value("username").toString() == username) {
            jsonArray.removeAt(i);
            break;
        }
    }
    jsonDoc.setArray(jsonArray);
    if (!file.open(QIODevice::WriteOnly))
    {
        return;
    }
    file.write(jsonDoc.toJson());
    file.close();
}


void ProjectsWindow::on_projects_set_admin_BTN_clicked()
{

        QTreeWidgetItem *selectedItem = ui->project_persons_tree_widget->currentItem();
        if (selectedItem)
        {
        QMessageBox::information(this, "Promoted!", "User promoted to admin!");
        changeProjectAdmin(ui->project_persons_tree_widget->currentItem()->text(1));
        thisProjectShowAdmin();
        }
        else
            QMessageBox::warning(this, "Select a person", "select a person from the list!");
        ui->project_persons_tree_widget->clearSelection();
}



void ProjectsWindow::changeProjectAdmin(QString username)
{
    QString id;
    QString filePath = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/"+this_org+"/ORG_PROJECTS/" +this_project.projGetName() + "/PROJECT_PERSON.json";
    QFile file(filePath);
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
        QJsonValue jsonValue = jsonArray.at(i);
        QJsonObject jsonObject = jsonValue.toObject();

        if (jsonObject.value("username").toString() == username)
        {
            id = jsonObject.value("id").toString();
            break;
        }
    }

    QString filePath2 = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/"+this_org+"/ORG_PROJECTS.json";
    QFile file2(filePath2);
    if (!file2.open(QIODevice::ReadWrite | QIODevice::Text)) {
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file2.readAll());
    file2.close();

    QJsonArray array = doc.array();
    for (int i = 0; i < array.size(); i++) {
        QJsonObject obj = array.at(i).toObject();
        if (obj.value("project_name").toString() == this_project.projGetName())
        {
            obj["project_admin_id"] = id;
            this_project.projSetAdminId(id);
            array.replace(i, obj);
            break;
        }
    }

    doc.setArray(array);

    if (!file2.open(QIODevice::WriteOnly))
    {
        return;
    }

    file2.write(doc.toJson());
    file2.close();
}

void ProjectsWindow::search_project_persons()
{
    QString input = ui->search_line_edit->text();
    QString search_type;

    for (int i = 0; i < ui->project_persons_tree_widget->topLevelItemCount(); ++i)
    {
        QTreeWidgetItem *item = ui->project_persons_tree_widget->topLevelItem(i);
        {
            if (ui->search_by_name_radio->isChecked())
                search_type = item->text(0);
            else if (ui->search_by_username_radio->isChecked())
                search_type = item->text(1);
            else if (ui->search_by_org_id_radio->isChecked())
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

void ProjectsWindow::on_search_line_edit_textChanged(const QString &arg1)
{
    loadProjectPersons();
    search_project_persons();
}



void ProjectsWindow::loadOrgTeamsComboBox()
{
    QString file_Path = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/"+this_org+"/ORG_TEAMS.json";
    QFile file(file_Path);
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument jsonDoc(QJsonDocument::fromJson(jsonData));

    QJsonArray jsonArray = jsonDoc.array();

    for (int i = 0; i < jsonArray.size(); ++i) {
        QJsonObject jsonObject = jsonArray.at(i).toObject();
        QString teamName = jsonObject.value("team_name").toString();
        ui->org_teams_combo_box->addItem(teamName);
    }
}


void ProjectsWindow::loadProjectTeams()
{
    ui->project_teams_list_widget->clear();
    QString file_Path = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/"+ this_org +"/ORG_PROJECTS/"+ this_project.projGetName() + "/PROJECT_TEAMS.json";
    QFile file(file_Path);
    if (!file.open(QIODevice::ReadOnly))
    {
        return;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    if (!jsonDoc.isArray())
    {
        return ;
    }

    QJsonArray jsonArray = jsonDoc.array();

    foreach (const QJsonValue &value, jsonArray)
    {
        if (value.isObject())
        {
            QJsonObject teamObject = value.toObject();
            if (teamObject.contains("team_name"))
            {
                QString teamName = teamObject["team_name"].toString();
                ui->project_teams_list_widget->addItem(teamName);
            }
        }
    }
}


void ProjectsWindow::on_project_teams_add_BTN_clicked()
{
    if(ui->org_teams_combo_box->currentText() != "select...")
    {
        addTeamToProject(ui->org_teams_combo_box->currentText());
        loadProjectTeams();
    }
    else
    QMessageBox::warning(this, "Select a team", "select a team from the list!");
}


void ProjectsWindow::addTeamToProject(QString teamName)
{
    QString file_Path = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/"+this_org+"/ORG_TEAMS.json";

    QFile file1(file_Path);
    if (!file1.open(QIODevice::ReadOnly)) {
        return ;
    }

    QJsonDocument doc1 = QJsonDocument::fromJson(file1.readAll());
    file1.close();

    QString file_Path2 = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/"+ this_org +"/ORG_PROJECTS/"+ this_project.projGetName() + "/PROJECT_TEAMS.json";
    QFile file2(file_Path2);
    if (!file2.open(QIODevice::ReadOnly)) {
        return;
    }

    QJsonDocument doc2 = QJsonDocument::fromJson(file2.readAll());
    file2.close();


    QJsonArray jsonArray1 = doc1.array();
    QJsonObject foundObject;

    for (int i = 0; i < jsonArray1.size(); ++i) {
        QJsonObject jsonObject = jsonArray1.at(i).toObject();
        if (jsonObject.value("team_name").toString() == teamName) {
            foundObject = jsonObject;
            break;
        }
    }

    if (foundObject.isEmpty()) {
        return ;
    }

    QJsonArray jsonArray2 = doc2.array();
    for (int i = 0; i < jsonArray2.size(); ++i) {
        QJsonObject jsonObject = jsonArray2.at(i).toObject();
        if (jsonObject.value("team_name").toString() == teamName) {
            return ;
        }
    }

    jsonArray2.append(foundObject);
    doc2.setArray(jsonArray2);

    QFile file2Write(file_Path2);
    if (!file2Write.open(QIODevice::WriteOnly )) {
        return;
    }
    file2Write.write(doc2.toJson());
    file2Write.close();
}




void ProjectsWindow::on_project_teams_remove_BTN_clicked()
{

    if (ui->project_teams_list_widget->currentItem()!=nullptr)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Delete a team", "Are you sure?\nDeleteing is not returnable!", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            removeTeamfromProject(ui->project_teams_list_widget->currentItem()->text());
            loadProjectTeams();
        }
    }
    else
    {
        QMessageBox::warning(this, "Select a team", "select a team from the list!");
    }
    ui->project_teams_list_widget->clearSelection();
}


void ProjectsWindow::removeTeamfromProject(QString teamname)
{
    QString file_Path = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/"+ this_org +"/ORG_PROJECTS/"+ this_project.projGetName() + "/PROJECT_TEAMS.json";
    QFile file(file_Path);
    if (!file.open(QIODevice::ReadOnly))
    {
        return ;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    QJsonArray jsonArray = doc.array();
    for (int i = 0; i < jsonArray.size(); ++i) {
        QJsonObject jsonObject = jsonArray.at(i).toObject();
        if (jsonObject.value("team_name").toString() == teamname) {
            jsonArray.removeAt(i);
            break;
        }
    }

    doc.setArray(jsonArray);
    file.open(QIODevice::WriteOnly);
    file.write(doc.toJson());
    file.close();
}

void ProjectsWindow::on_project_teams_sort_BTN_clicked()
{
    ui->project_teams_list_widget->sortItems();
}

void ProjectsWindow::on_project_teams_search_line_edit_textChanged(const QString &arg1)
{
    search_project_teams();
}

void ProjectsWindow::search_project_teams()
{
    QString s = ui->project_teams_search_line_edit->text();
    for (int i = 0; i < ui->project_teams_list_widget->count(); ++i) {
        QListWidgetItem *item = ui->project_teams_list_widget->item(i);
        if (item->text().startsWith(s)) {
            item->setHidden(false);
        } else {
            item->setHidden(true);
        }
    }
}


void ProjectsWindow::on_add_new_task_BTN_clicked()
{
    TaskWindow * w = new TaskWindow (this);
    connect (w,SIGNAL(new_task_maked(Task)),this,SLOT(project_task_maker(Task)));
    w->typeSetter("NEW_TASK");
    w->show();

}

void ProjectsWindow::project_task_maker(Task maked_task)
{
    AddNewTaskToProject(maked_task);
    QTreeWidgetItem *item = new QTreeWidgetItem(ui->project_tasks_tree_widget);
    item->setText(0, maked_task.taskGetTitle());
    item->setText(1, maked_task.taskGetOwnerName());
    item->setText(2, maked_task.taskGetDate());
    item->setText(3, maked_task.taskGetPriority());
    item->setText(4, maked_task.taskGetIsArchived() ? "YES" : "NO");

    ui->project_tasks_tree_widget->addTopLevelItem(item);
}


void ProjectsWindow::AddNewTaskToProject(Task new_task)
{
    QString folder_Path = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/"+ this_org + "/ORG_PROJECTS/" + this_project.projGetName() +"/PROJECT_TASKS/"+ new_task.taskGetTitle();
    QDir sDir(folder_Path);
    if (!sDir.exists())
    {
        sDir.mkpath(".");
    }

    QString file_Path = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/"+ this_org+ "/ORG_PROJECTS/" + this_project.projGetName() +"/PROJECT_TASKS.json" ;

    QFile file(file_Path);
    file.open(QIODevice::ReadWrite);
    QByteArray jsonData2 = file.readAll();
    QJsonDocument jsonDoc2 = QJsonDocument::fromJson(jsonData2);
    QJsonArray jsonArray2 = jsonDoc2.array();

    for (int i = 0; i < jsonArray2.size(); ++i)
    {
        QJsonObject jsonObject = jsonArray2.at(i).toObject();
        if (jsonObject.value("task_title").toString() == new_task.taskGetTitle())
        {
            file.close();
            return;
        }
    }

    QJsonObject newTaskObject;
    newTaskObject["task_title"] = new_task.taskGetTitle();
    newTaskObject["task_description"] = new_task.taskGetDescription();
    newTaskObject["task_owner_type"] = new_task.taskGetOwnerType();
    newTaskObject["task_owner_name"] = new_task.taskGetOwnerName();
    newTaskObject["task_owner_id"] = new_task.taskGetOwnerID();
    newTaskObject["task_date"] = new_task.taskGetDate();
    newTaskObject["task_is_archived"] = new_task.taskGetIsArchived();

    jsonArray2.append(newTaskObject);
    jsonDoc2.setArray(jsonArray2);
    file.resize(0);
    file.write(jsonDoc2.toJson());
    file.close();

}

