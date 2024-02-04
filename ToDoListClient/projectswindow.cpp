#include "projectswindow.h"
#include "ui_projectswindow.h"
#include <QStringList>
#include <QJsonValue>

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


void ProjectsWindow::connectionMaker(QString ip, int port)
{
    this_ip = ip;
    this_port = port;
    socket = new QTcpSocket();
    socket->connectToHost(ip, port);
    connect(socket, &QTcpSocket::connected, this, &ProjectsWindow::socket_connected);
    connect(socket, &QTcpSocket::readyRead, this, &ProjectsWindow::socket_readyRead);
    connect(socket, &QTcpSocket::bytesWritten, this, &ProjectsWindow::socket_bytesWritten);
    connect(socket, &QTcpSocket::disconnected, this, &ProjectsWindow::socket_disconnected);
    qDebug() << "Project win connected!";
}

void ProjectsWindow::thisUserIsTeamMemberSetter(bool a)
{
    is_team_member = a;
}

void ProjectsWindow::socket_readyRead()
{
    QByteArray data = socket->readAll();
    responseChecker(QString(data));
}

void ProjectsWindow::socket_connected()
{
}

void ProjectsWindow::socket_bytesWritten()
{
}

void ProjectsWindow::socket_disconnected()
{
}

void ProjectsWindow::sendRequest(QString s)
{
    QByteArray data = s.toUtf8();
    socket->write(data);
    qDebug() << s;
}

void ProjectsWindow::responseChecker(QString s)
{
    qDebug() << "response get is: " << s;
    QJsonDocument doc = QJsonDocument::fromJson(s.toUtf8());
    QString resState = doc.object().value("res-state").toString();

    if (resState =="project-persons-list-ok")
    {
        show_project_persons(s);
    }
    if (resState =="project-teams-list-ok")
    {
        show_project_teams(s);
    }
    if (resState =="project-tasks-list-ok")
    {
        show_project_tasks(s);
    }
    if (resState =="add-person-to-project-ok")
    {
    QString mess = doc.object().value("message").toString();
    QMessageBox::information(this, "added", mess);
    loadProjectPersons();
    }
    if (resState =="remove-person-from-project-ok")
    {
        QString mess = doc.object().value("message").toString();
        QMessageBox::information(this, "removed", mess);
        loadProjectPersons();
    }
    if (resState =="change-person-role-in-project-ok")
    {
        QString mess = doc.object().value("message").toString();
        QMessageBox::information(this, "changed", mess);
        thisProjectShowAdmin(this_project_admin);
    }
    if (resState == "load-org-teams-in-project-ok")
    {
        show_teams_in_combobox(s);
    }
    if (resState == "add-team-to-project-ok")
    {
        QString mess = doc.object().value("message").toString();
        QMessageBox::information(this, "added", mess);
        loadProjectTeams();
    }
    if (resState == "remove-team-from-project-ok")
    {
        QString mess = doc.object().value("message").toString();
        QMessageBox::information(this, "Removed", mess);
        loadProjectTeams();
    }
    if (resState == "add-task-to-project-ok")
    {
        QString mess = doc.object().value("message").toString();
        QMessageBox::information(this, "added", mess);
        loadProjectTasks();
    }
    if (resState == "remove-task-from-project-ok")
    {
        QString mess = doc.object().value("message").toString();
        QMessageBox::information(this, "removed", mess);
        loadProjectTasks();
    }
    if (resState == "edit-task-archive-in-project-ok")
    {
        QString mess = doc.object().value("message").toString();
        QMessageBox::information(this, "Changed", mess);
        loadProjectTasks();
    }
    if (resState == "project-task-details-ok")
    {
        edit_project_task(s);
    }
    if (resState == "edit-task-in-project-ok")
    {
        QString mess = doc.object().value("message").toString();
        QMessageBox::information(this, "Edited", mess);
        loadProjectTasks();
    }
    if (resState == "load-task_comments-ok")
    {
        maked_taskwindow(s);
    }
    if(resState == "save-task-comments-ok")
    {
        EditAnSpecialTaskInProject(again_temp_task);
    }
}

void ProjectsWindow::this_org_maker(QString a)
{
    this_org = a;
}

void ProjectsWindow::project_this_person_maker(OrgPerson a)
{
    this_person.perSetID(a.perGetId());
    this_person.perSetName(a.perGetName());
    this_person.perSetUsername(a.perGetUsername());
    this_person.orgPerSetID(a.orgPerGetId());
}

void ProjectsWindow::project_admin_access_maker(bool a, bool b)
{
    is_admin = a;
    is_org_admin = b;
}

void ProjectsWindow::this_project_maker(QString a)
{
    this_project.projSetName(a);
    ui->projects_name_text->setText(a);
}

void ProjectsWindow::thisProjectShowAdmin(QString username)
{
  this_project_admin = username;
  ui->Project_admin_text->setText(username);
}

void ProjectsWindow::loadProjectPersons()
{
    QJsonObject jsonObject;
    jsonObject["req-type"] = "poject_persons_list";
    jsonObject["projname"] = this_project.projGetName();
    jsonObject["orgname"] = this_org;
    QJsonDocument jsonDocument(jsonObject);
    QString req = jsonDocument.toJson(QJsonDocument::Compact);
    sendRequest(req);
}

void ProjectsWindow::show_project_persons(QString s)
{
    ui->project_persons_tree_widget->clear();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(s.toUtf8());
    QJsonObject jsonObject = jsonResponse.object();
    QJsonArray personsArray = jsonObject["project-persons"].toArray();

    for (int i = 0; i < personsArray.size(); ++i) {
        QJsonObject person = personsArray[i].toObject();
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->project_persons_tree_widget);
        item->setText(0, person["name"].toString());
        item->setText(1, person["username"].toString());
        item->setText(2, person["org_id"].toString());
    }
    loadProjectTeams();
}

void ProjectsWindow::loadProjectTeams()
{
    QJsonObject jsonObject;
    jsonObject["req-type"] = "poject_teams_list";
    jsonObject["projname"] = this_project.projGetName();
    jsonObject["orgname"] = this_org;
    QJsonDocument jsonDocument(jsonObject);
    QString req = jsonDocument.toJson(QJsonDocument::Compact);
    sendRequest(req);
}

void ProjectsWindow::show_project_teams(QString s)
{
    ui->project_teams_list_widget->clear();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(s.toUtf8());
    QJsonObject jsonObject = jsonResponse.object();
    QJsonArray teamsArray = jsonObject["project-teams"].toArray();

    for (int i = 0; i < teamsArray.size(); ++i) {
        QJsonObject team = teamsArray[i].toObject();
        QString teamName = team["team_name"].toString();
        QListWidgetItem *item = new QListWidgetItem(teamName, ui->project_teams_list_widget);
        ui->project_teams_list_widget->addItem(item);
    }
    loadProjectTasks();
}

void ProjectsWindow::loadProjectTasks()
{
    QJsonObject jsonObject;
    jsonObject["req-type"] = "poject_tasks_list";
    jsonObject["projname"] = this_project.projGetName();
    jsonObject["orgname"] = this_org;
    QJsonDocument jsonDocument(jsonObject);
    QString req = jsonDocument.toJson(QJsonDocument::Compact);
    sendRequest(req);
}

void ProjectsWindow::show_project_tasks(QString s)
{
    ui->project_tasks_tree_widget->clear();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(s.toUtf8());
    QJsonObject jsonObject = jsonResponse.object();
    QJsonArray personsArray = jsonObject["project-tasks"].toArray();

    for (int i = 0; i < personsArray.size(); ++i) {
        QJsonObject person = personsArray[i].toObject();
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->project_tasks_tree_widget);
        item->setText(0, person["task_title"].toString());
        item->setText(1, person["task_owner_name"].toString());
        item->setText(2, person["task_date"].toString());
        item->setText(3, person["task_priority"].toString());
        item->setText(4, person["task_is_archived"].toBool() ? "YES" : "NO");
    }
    loadOrgTeamsComboBox();
}

void ProjectsWindow::on_projects_add_person_BTN_clicked()
{
    if(is_admin == true)
    {
        OrgDialog * s = new OrgDialog(this);
        s->set_text("Enter Username of the Person: ");
        connect(s,SIGNAL(name_readed(QString)),this,SLOT(add_project_person(QString)));
        s->show();
    }
    else
    {
        QMessageBox::warning(this, "Not admin", "You are NOT Project or Organization Admin!");
    }
}

void ProjectsWindow::add_project_person(QString new_username)
{
    QJsonObject jsonObject;
    jsonObject["req-type"] = "add-new-person-to-project";
    jsonObject["projname"] = this_project.projGetName();
    jsonObject["orgname"] = this_org;
    jsonObject["username"] = new_username;
    QJsonDocument jsonDocument(jsonObject);
    QString req = jsonDocument.toJson(QJsonDocument::Compact);
    sendRequest(req);

}

void ProjectsWindow::addNewProjectPerson(QString new_username)
{

}

void ProjectsWindow::on_projects_remove_person_BTN_clicked()
{
    if(is_admin == true)
    {
        QTreeWidgetItem *selectedItem = ui->project_persons_tree_widget->currentItem();
        if (selectedItem)
        {
            if(selectedItem->text(1) == this_person.perGetName() && is_org_admin == false)
            {
                QMessageBox::warning(this, "Self Remove", "You can NOT remove yourself!");
            }
            else
            {
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, "Delete a person", "Are you sure?\nDeleteing is not returnable!", QMessageBox::Yes | QMessageBox::No);
                if (reply == QMessageBox::Yes)
                {
                    removeProjectPerson(ui->project_persons_tree_widget->currentItem()->text(1));
                }
            }
        }
        else
            QMessageBox::warning(this, "Select a person", "select a person from the list!");
        ui->project_persons_tree_widget->clearSelection();
    }
    else
    {
        QMessageBox::warning(this, "Not found", "You are NOT Project or Organization Admin!");
    }
}

void ProjectsWindow::removeProjectPerson(QString username)
{
    QJsonObject jsonObject;
    jsonObject["req-type"] = "remove-person-from-project";
    jsonObject["projname"] = this_project.projGetName();
    jsonObject["orgname"] = this_org;
    jsonObject["username"] = username;
    QJsonDocument jsonDocument(jsonObject);
    QString req = jsonDocument.toJson(QJsonDocument::Compact);
    sendRequest(req);

}

void ProjectsWindow::on_projects_set_admin_BTN_clicked()
{
    if(is_admin == true)
    {
        QTreeWidgetItem *selectedItem = ui->project_persons_tree_widget->currentItem();
        if (selectedItem)
        {
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, "promotr a person", "Are you sure?", QMessageBox::Yes | QMessageBox::No);
                if (reply == QMessageBox::Yes)
                {
                    changeProjectAdmin(ui->project_persons_tree_widget->currentItem()->text(1));
                }
        }
        else
            QMessageBox::warning(this, "Select a person", "select a person from the list!");
        ui->project_persons_tree_widget->clearSelection();
    }
    else
    {
        QMessageBox::warning(this, "Not found", "You are NOT Project or Organization Admin!");
    }
}

void ProjectsWindow::changeProjectAdmin(QString username)
{
    QJsonObject jsonObject;
    jsonObject["req-type"] = "change-person-role-in-project";
    jsonObject["projname"] = this_project.projGetName();
    jsonObject["orgname"] = this_org;
    jsonObject["username"] = username;
    QJsonDocument jsonDocument(jsonObject);
    QString req = jsonDocument.toJson(QJsonDocument::Compact);
    sendRequest(req);

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
    for (int i = 0; i < ui->project_persons_tree_widget->topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = ui->project_persons_tree_widget->topLevelItem(i);
        item->setHidden(false);
        for (int j = 0; j < item->childCount(); ++j) {
            item->child(j)->setHidden(false);
        }
    }
    search_project_persons();
}

void ProjectsWindow::loadOrgTeamsComboBox()
{
    QJsonObject jsonObject;
    jsonObject["req-type"] = "load-org-teams-in-project";
    jsonObject["orgname"] = this_org;
    QJsonDocument jsonDocument(jsonObject);
    QString req = jsonDocument.toJson(QJsonDocument::Compact);
    sendRequest(req);
}

void ProjectsWindow::show_teams_in_combobox(QString s)
{
    ui->org_teams_combo_box->clear();
    ui->org_teams_combo_box->addItem("Select ...");
    QJsonDocument jsonDoc = QJsonDocument::fromJson(s.toUtf8());
    QJsonObject jsonObj = jsonDoc.object();
    QJsonArray jsonArray = jsonObj["org-teams"].toArray();
    for (int i = 0; i < jsonArray.size(); ++i) {
        QJsonObject jsonObject = jsonArray.at(i).toObject();
        QString teamName = jsonObject.value("team_name").toString();
        ui->org_teams_combo_box->addItem(teamName);
    }
}

void ProjectsWindow::on_project_teams_add_BTN_clicked()
{       
    if(ui->org_teams_combo_box->currentText() != "Select ...")
    {
        if(is_admin == true)
        {

                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, "promotr a person", "Are you sure?", QMessageBox::Yes | QMessageBox::No);
                if (reply == QMessageBox::Yes)
                {
                    addTeamToProject(ui->org_teams_combo_box->currentText());
                }
        }
        else
        {
            QMessageBox::warning(this, "Not access", "You are NOT Project or Organization Admin!");
        }
    }
    else
    QMessageBox::warning(this, "Select a team", "select a team from the list!");
}


void ProjectsWindow::addTeamToProject(QString s)
{
    QJsonObject jsonObject;
    jsonObject["req-type"] = "add-teams-to-project";
    jsonObject["orgname"] = this_org;
    jsonObject["teamname"] = s;
    jsonObject["projname"] = this_project.projGetName();
    QJsonDocument jsonDocument(jsonObject);
    QString req = jsonDocument.toJson(QJsonDocument::Compact);
    sendRequest(req);
}


void ProjectsWindow::on_project_teams_remove_BTN_clicked()
{
if(is_admin == true)
{
    if (ui->project_teams_list_widget->currentItem()!=nullptr)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Delete a team", "Are you sure?\nDeleteing is not returnable!", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            removeTeamfromProject(ui->project_teams_list_widget->currentItem()->text());
        }
    }
    else
    {
            QMessageBox::warning(this, "Select a team", "select a team from the list!");
    }
       ui->project_teams_list_widget->clearSelection();
    }
    else
    {
        QMessageBox::warning(this, "Not access", "You are NOT Project or Organization Admin!");
    }

}

void ProjectsWindow::removeTeamfromProject(QString teamname)
{
    QJsonObject jsonObject;
    jsonObject["req-type"] = "remove-team-from-project";
    jsonObject["orgname"] = this_org;
    jsonObject["teamname"] = teamname;
    jsonObject["projname"] = this_project.projGetName();
    QJsonDocument jsonDocument(jsonObject);
    QString req = jsonDocument.toJson(QJsonDocument::Compact);
    sendRequest(req);
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
    if(is_admin == true)
    {
        QStringList teams_list;
        for (int i = 0; i < ui->project_teams_list_widget->count(); ++i)
        {
            teams_list << ui->project_teams_list_widget->item(i)->text();
        }
        QStringList persons_list;
        for (int i = 0; i < ui->project_persons_tree_widget->topLevelItemCount(); ++i) {
            QTreeWidgetItem *item = ui->project_persons_tree_widget->topLevelItem(i);
            persons_list << item->text(1);
        }

        TaskWindow * w = new TaskWindow (this);
        connect (w,SIGNAL(new_task_maked(Task)),this,SLOT(project_task_maker(Task)));
        w->ownerListSetter(teams_list,persons_list);
        w->typeSetter("NEW_TASK");
        w->show();
    }
    else
    {
        QMessageBox::warning(this, "Not access", "You are NOT Project or Organization Admin!");
    }
}

void ProjectsWindow::project_task_maker(Task maked_task)
{
    QJsonObject jsonObject;
    jsonObject["req-type"] = "add-task-to-project";
    jsonObject["orgname"] = this_org;
    jsonObject["projname"] = this_project.projGetName();
    jsonObject["tasktitle"] = maked_task.taskGetTitle();
    jsonObject["taskownername"] = maked_task.taskGetOwnerName();
    jsonObject["taskdate"] = maked_task.taskGetDate();
    jsonObject["taskpriority"] = maked_task.taskGetPriority();
    jsonObject["taskarchived"] = maked_task.taskGetIsArchived();
    jsonObject["taskdescription"] = maked_task.taskGetDescription();

    QJsonDocument jsonDocument(jsonObject);
    QString req = jsonDocument.toJson(QJsonDocument::Compact);
    sendRequest(req);

}

void ProjectsWindow::AddNewTaskToProject(Task new_task)
{

}

void ProjectsWindow::on_remove_task_BTN_clicked()
{
    if(is_admin == true)
    {
        if (ui->project_tasks_tree_widget->currentItem()!=nullptr)
        {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Delete a Task", "Are you sure?\nDeleteing is not returnable!", QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::Yes)
            {
                removeTaskFromProject(ui->project_tasks_tree_widget->currentItem()->text(0));
            }
        }
        else
        {
            QMessageBox::warning(this, "Select a Task", "select a Task from the list!");
        }
        ui->project_teams_list_widget->clearSelection();
    }
    else
    {
        QMessageBox::warning(this, "Not access", "You are NOT Project or Organization Admin!");
    }

}

void ProjectsWindow::removeTaskFromProject(QString task_name)
{
    QJsonObject jsonObject;
    jsonObject["req-type"] = "remove-task-from-project";
    jsonObject["orgname"] = this_org;
    jsonObject["projname"] = this_project.projGetName();
    jsonObject["taskname"] = task_name;
    QJsonDocument jsonDocument(jsonObject);
    QString req = jsonDocument.toJson(QJsonDocument::Compact);
    sendRequest(req);
}

void ProjectsWindow::on_archive_unarchive_BTN_clicked()
{
    if(is_admin == true)
    {
        QTreeWidgetItem *item = ui->project_tasks_tree_widget->currentItem();
        if (item)
        {
            QString selecteduser = item->text(0);
            EditTaskArchive(selecteduser);
        }
        else
            QMessageBox::warning(this, "Select a task", "select a task from the list!");
    }
    else
    {
        QMessageBox::warning(this, "Not access", "You are NOT Project or Organization Admin!");
    }
}

void ProjectsWindow::save_comments(QString req)
{
    sendRequest(req);
}

void ProjectsWindow::EditTaskArchive(QString task_title)
{
    QJsonObject jsonObject;
    jsonObject["req-type"] = "edit-task-archive-in-project";
    jsonObject["orgname"] = this_org;
    jsonObject["projname"] = this_project.projGetName();
    jsonObject["taskname"] = task_title;
    QJsonDocument jsonDocument(jsonObject);
    QString req = jsonDocument.toJson(QJsonDocument::Compact);
    sendRequest(req);
}

void ProjectsWindow::on_project_tasks_tree_widget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
        QJsonObject jsonObject;
        jsonObject["req-type"] = "edit-task-in-project";
        jsonObject["orgname"] = this_org;
        jsonObject["projname"] = this_project.projGetName();
        jsonObject["taskname"] = item->text(0);
        QJsonDocument jsonDocument(jsonObject);
        QString req = jsonDocument.toJson(QJsonDocument::Compact);
        sendRequest(req);
}

void ProjectsWindow::maked_taskwindow(QString s)
{
    TaskWindow * w = new TaskWindow (this);
    connect (this,SIGNAL(this_task_maked(Task)),w,SLOT(this_task_maker(Task)));
    connect (w,SIGNAL(task_edited(QString,Task)),this,SLOT(edited_project_task(QString,Task)));
    w->typeSetter("EDIT_TASK");
    w->ownerListSetter(temp_teams_list,temp_persons_list);
    w->thisOrgProjectSetter(this_org,this_project.projGetName());
    emit this_task_maked(another_temp_task);
    w->thisOrgSetter(this_org);
    w->thisprojectSetter(this_project.projGetName());
    w->thistaskSetter(this_task);
    OrgPerson p;
    p.perSetName(this_person.perGetName());
    w->thisPersonSetter(p);
    w->loadTaskComments(s);
    w->addCommentsToTree();
    w->show();
}

void ProjectsWindow::edit_project_task(QString s)
{
    QJsonDocument jsonResponse = QJsonDocument::fromJson(s.toUtf8());
    QJsonObject jsonObject = jsonResponse.object();
    Task editing_task;
    if (jsonObject.contains("object")) {
        QJsonArray objectArray = jsonObject["object"].toArray();
        for (const QJsonValue &value : objectArray)
        {
            QJsonObject taskObject = value.toObject();
            editing_task.taskSetDate(taskObject["task_date"].toString());
            editing_task.taskSetDescription(taskObject["task_description"].toString());
            editing_task.taskSetTitle(taskObject["task_title"].toString());
            editing_task.taskSetOwnerName(taskObject["task_owner_name"].toString());
            editing_task.taskSetPriority(taskObject["task_priority"].toString());
            editing_task.taskSetIsArchived(taskObject["task_is_archived"].toBool());
            editing_task.taskSetOwnerType(taskObject["task_owner_type"].toString());
            editing_task.taskSetOwnerID(taskObject["task_owner_id"].toString());
            break;
        }
    }

    another_temp_task = editing_task;

    QStringList teams_list;
    for (int i = 0; i < ui->project_teams_list_widget->count(); ++i)
    {
        teams_list << ui->project_teams_list_widget->item(i)->text();
    }

    temp_teams_list = teams_list;

    QStringList persons_list;
    for (int i = 0; i < ui->project_persons_tree_widget->topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = ui->project_persons_tree_widget->topLevelItem(i);
        persons_list << item->text(1);
    }

    temp_persons_list = persons_list;

    QJsonObject jsonObject2;
    jsonObject2["req-type"] = "load-task-comments";
    jsonObject2["orgname"] = this_org;
    jsonObject2["projname"] = this_project.projGetName();
    jsonObject2["tasktitle"] = this_task.taskGetTitle();

    QJsonDocument jsonDocument(jsonObject2);
    QString req = jsonDocument.toJson(QJsonDocument::Compact);
    sendRequest(req);
    // TaskWindow * w = new TaskWindow (this);
    // connect (this,SIGNAL(this_task_maked(Task)),w,SLOT(this_task_maker(Task)));
    // connect (w,SIGNAL(task_edited(Task)),this,SLOT(edited_project_task(Task)));
    // w->typeSetter("EDIT_TASK");
    // w->ownerListSetter(teams_list,persons_list);
    // w->thisOrgProjectSetter(this_org,this_project.projGetName());
    // emit this_task_maked(editing_task);
    // w->show();
}

void ProjectsWindow::edited_project_task(QString req,Task edited_task)
{
    again_temp_task = edited_task;
    sendRequest(req);

}

void ProjectsWindow::EditTaskInProject(QString old_task , Task edited_task)
{
    QJsonObject jsonObject;
    jsonObject["req-type"] = "save-edited-task-project";
    jsonObject["orgname"] = this_org;
    jsonObject["projname"] = this_project.projGetName();
    jsonObject["oldtasktitle"] = old_task;

    jsonObject["tasktitle"] = edited_task.taskGetTitle();
    jsonObject["taskownername"] = edited_task.taskGetOwnerName();
    jsonObject["taskdate"] = edited_task.taskGetDate();
    jsonObject["taskpriority"] = edited_task.taskGetPriority();
    jsonObject["taskarchived"] = edited_task.taskGetIsArchived();
    jsonObject["taskdescription"] = edited_task.taskGetDescription();
    jsonObject["taskownertype"] = edited_task.taskGetOwnerType();
    jsonObject["taskownerid"] = edited_task.taskGetOwnerID();

    QJsonDocument jsonDocument(jsonObject);
    QString req = jsonDocument.toJson(QJsonDocument::Compact);
    sendRequest(req);

}

void ProjectsWindow::EditAnSpecialTaskInProject(Task edited_task)
{
    EditTaskInProject(ui->project_tasks_tree_widget->currentItem()->text(0) , edited_task);
}

void ProjectsWindow::search_task()
{
    QString input = ui->search_task_line_edit->text();
    QString search_type;

    for (int i = 0; i < ui->project_tasks_tree_widget->topLevelItemCount(); ++i)
    {
        QTreeWidgetItem *item = ui->project_tasks_tree_widget->topLevelItem(i);
        {
            if (ui->task_search_by_title->isChecked())
                search_type = item->text(0);
            else if (ui->task_search_by_owner->isChecked())
                search_type = item->text(1);
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

void ProjectsWindow::on_search_task_line_edit_textChanged(const QString &arg1)
{
    for (int i = 0; i < ui->project_tasks_tree_widget->topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = ui->project_tasks_tree_widget->topLevelItem(i);
        item->setHidden(false);
        for (int j = 0; j < item->childCount(); ++j) {
            item->child(j)->setHidden(false);
        }
    }
    on_archived_checkbox_stateChanged(0);
    on_not_archived_checkbox_stateChanged(0);
    search_task();
}

void ProjectsWindow::on_archived_checkbox_stateChanged(int arg1)
{
    for (int i = 0; i < ui->project_tasks_tree_widget->topLevelItemCount(); ++i)
    {
        QTreeWidgetItem *item = ui->project_tasks_tree_widget->topLevelItem(i);

        if (ui->archived_checkbox->isChecked())
        {
            if (item->text(4)=="YES")
            {
                item->setHidden(false);
            }
        }
        else if (!ui->archived_checkbox->isChecked())
        {
            if (item->text(4)=="YES")
            {
                item->setHidden(true);
            }
        }
    }
    search_task();
}

void ProjectsWindow::on_not_archived_checkbox_stateChanged(int arg1)
{
    for (int i = 0; i < ui->project_tasks_tree_widget->topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = ui->project_tasks_tree_widget->topLevelItem(i);

        if (ui->not_archived_checkbox->isChecked())
        {
            if (item->text(4)=="NO")
            {
                item->setHidden(false);
            }
        }
        else if (!ui->not_archived_checkbox->isChecked())
        {
            if (item->text(4)=="NO")
            {
                item->setHidden(true);
            }
        }
    }
    search_task();
}



void ProjectsWindow::closeEvent(QCloseEvent *event)
{
    qDebug() << "project win Disconnected!";
    socket->close();
    emit reconnect(this_ip,this_port);
}



