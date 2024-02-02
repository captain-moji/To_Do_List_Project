#include "teamswindow.h"
#include "ui_teamswindow.h"
#include <QTimer>
#include <QJsonValue>

TeamsWindow::TeamsWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TeamsWindow)
{
    ui->setupUi(this);
    ui->team_persons_tree_widget->setCurrentItem(nullptr);
}

TeamsWindow::~TeamsWindow()
{
    delete ui;
}


void TeamsWindow::this_team_maker(QString a)
{
    this_team.teamSetName(a);
    ui->teams_team_name_text->setText(a);
}


void TeamsWindow::this_org_maker(QString a)
{
    this_org = a;
}


void TeamsWindow::this_team_id_maker(QString a)
{
    this_team.teamSetId(a);
}


void TeamsWindow::on_teams_add_person_BTN_clicked()
{
    OrganizationPersonsWindow *o = new OrganizationPersonsWindow(this);
    connect (o,SIGNAL(org_person_selected(QString)),this,SLOT(add_team_person(QString)));
    o->ORG = this_org;
    o->setWindowTitle("Add New Person");
    o->show();
    o->loadOrgPersons();
}


void TeamsWindow::add_team_person(QString new_username)
{
    addNewTeamPerson(new_username);
    loadTeamPersons();
}


void TeamsWindow::loadTeamPersons()
{
    ui->team_persons_tree_widget->clear();
    QString filePath = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/"+this_org+"/ORG_TEAMS/" +this_team.teamGetName()+ ".json";
    QFile file(filePath);
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
            qDebug() << "Invalid json object";
            continue;
        }
        QJsonObject jsonObject = jsonValue.toObject();

        QString name = jsonObject.value("name").toString();
        QString username = jsonObject.value("username").toString();
        QString orgId = jsonObject.value("org_id").toString();
        bool isTeamAdmin = jsonObject.value("is_team_admin").toBool();

        bool isUsernameUnique = true;
        for (int i = 0; i < ui->team_persons_tree_widget->topLevelItemCount(); ++i) {
            QTreeWidgetItem* item = ui->team_persons_tree_widget->topLevelItem(i);
            if (item->text(1) == username) {
                isUsernameUnique = false;
                break;
            }
        }

        if (isUsernameUnique) {
            QTreeWidgetItem* item = new QTreeWidgetItem(ui->team_persons_tree_widget);
            item->setText(0, name);
            item->setText(1, username);
            item->setText(2, orgId);
            item->setText(3, isTeamAdmin ? "YES" : "NO");
        }
    }
}


void TeamsWindow::addNewTeamPerson(QString new_username)
{
    QString file_Path = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/" + this_org + "/ORG_PERSONS.json";
    bool exsist = false;

    QFile filex(file_Path);
    filex.open(QIODevice::ReadOnly);
    QByteArray jsonDatax = filex.readAll();
    filex.close();
    QJsonDocument doc = QJsonDocument::fromJson(jsonDatax);
    QJsonArray jsonArrayx = doc.array();
    for (const auto& item : jsonArrayx) {
        QJsonObject obj = item.toObject();
        if (obj["username"].toString() == new_username)
        {
            exsist = true;
            break;
        }
    }

    if(exsist == false)
    {
        return;
    }

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
            temp_team_person.perSetName(jsonObject.value("name").toString());
            temp_team_person.perSetUsername(jsonObject.value("username").toString());
            temp_team_person.perSetID(jsonObject.value("id").toString());
            temp_team_person.orgPerSetID(jsonObject.value("org_id").toString());
            temp_team_person.orgPerSetIsAdmin(jsonObject.value("is_admin").toBool());
            temp_team_person.teamPerSetID(this_team.teamGetid());
            temp_team_person.teamPerSetIsAdmin(false);
        }
    }


    QString file_Path2 = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/"+this_org+"/ORG_TEAMS/" +this_team.teamGetName()+ ".json";

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
            QMessageBox ::warning(this, "Error!" , "This user already added to this team!");
            file.close();
            return;
        }
    }

    QJsonObject newJsonObject;
    newJsonObject["id"] = temp_team_person.perGetId();
    newJsonObject["org_id"] = temp_team_person.orgPerGetId();
    newJsonObject["name"] = temp_team_person.perGetName();
    newJsonObject["username"] = temp_team_person.perGetUsername();
    newJsonObject["is_org_admin"] = temp_team_person.orgPerGetIsAdmin();
    newJsonObject["team_id"] = temp_team_person.teamPerGetId();
    newJsonObject["is_team_admin"] = temp_team_person.teamPerGetIsAdmin();

    jsonArray2.append(newJsonObject);

    jsonDoc.setArray(jsonArray2);
    file2.resize(0);
    file2.write(jsonDoc.toJson());
    file2.close();
}


void TeamsWindow::removeTeamPerson(QString username)
{
    QString filePath = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/"+this_org+"/ORG_TEAMS/" +this_team.teamGetName()+ ".json";

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
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
    if (!file.open(QIODevice::WriteOnly)) {
        return;
    }
    file.write(jsonDoc.toJson());
    file.close();
}


void TeamsWindow::changeTeamPersonRole(QString username)
{
    QString filePath = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/"+this_org+"/ORG_TEAMS/" +this_team.teamGetName()+ ".json";
    QFile file(filePath);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        return;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonArray jsonArray = doc.array();

    for (int i = 0; i < jsonArray.size(); i++)
    {
        QJsonObject obj = jsonArray[i].toObject();
        if (obj["username"].toString() == username)
        {
            if(obj["is_team_admin"] == false)
            {
                obj["is_team_admin"] = true;
            }
            else
            {
                obj["is_team_admin"] = false;
            }
            jsonArray[i] = obj;
            break;
        }
    }

    doc.setArray(jsonArray);
    file.resize(0);
    file.write(doc.toJson());
    file.close();
}

void TeamsWindow::changeOrgPersonRole(QString username)
{
    QString filePath = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/"+this_org+"/ORG_TEAMS/" +this_team.teamGetName()+ ".json";
    QFile file(filePath);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        return;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonArray jsonArray = doc.array();

    for (int i = 0; i < jsonArray.size(); i++)
    {
        QJsonObject obj = jsonArray[i].toObject();
        if (obj["username"].toString() == username)
        {
            if(obj["is_org_admin"] == false)
            {
                obj["is_org_admin"] = true;
            }
            else
            {
                obj["is_org_admin"] = false;
            }
            jsonArray[i] = obj;
            break;
        }
    }

    doc.setArray(jsonArray);
    file.resize(0);
    file.write(doc.toJson());
    file.close();
}


void TeamsWindow::on_teams_remove_person_BTN_clicked()
{
    if (ui->team_persons_tree_widget->currentItem()!=nullptr)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Delete a person", "Are you sure?\nDeleteing is not returnable!", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            removeTeamPerson(ui->team_persons_tree_widget->currentItem()->text(1));
            loadTeamPersons();
        }
    }
    else
    {
        QMessageBox::warning(this, "Select a person", "select a person from the list!");
    }
    ui->team_persons_tree_widget->clearSelection();
}


void TeamsWindow::on_teams_change_role_BTN_clicked()
{
    QTreeWidgetItem *selectedItem = ui->team_persons_tree_widget->currentItem();
    if (selectedItem)
    {
        if(ui->team_persons_tree_widget->currentItem()->text(3)=="YES")
            QMessageBox::information(this, "demoted!", "User demoted to member!");
        else
            QMessageBox::information(this, "Promoted!", "User promoted to admin!");
        changeTeamPersonRole(ui->team_persons_tree_widget->currentItem()->text(1));
        loadTeamPersons();
    }
    else
        QMessageBox::warning(this, "Select a person", "select a person from the list!");
    ui->team_persons_tree_widget->clearSelection();
}




void TeamsWindow::search_user()
{
    QString input = ui->search_line_edit->text();
    QString search_type;

    for (int i = 0; i < ui->team_persons_tree_widget->topLevelItemCount(); ++i)
    {
        QTreeWidgetItem *item = ui->team_persons_tree_widget->topLevelItem(i);
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


void TeamsWindow::on_search_line_edit_textChanged(const QString &arg1)
{
    on_Admins_checkbox_stateChanged(0);
    on_members_checkbox_stateChanged(0);
    search_user();
}

void TeamsWindow::on_members_checkbox_stateChanged(int arg1)
{
    for (int i = 0; i < ui->team_persons_tree_widget->topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = ui->team_persons_tree_widget->topLevelItem(i);

        if (ui->members_checkbox->isChecked())
        {
            if (item->text(3)=="NO")
            {
                item->setHidden(false);
            }
        }
        else if (!ui->members_checkbox->isChecked())
        {
            if (item->text(3)=="NO")
            {
                item->setHidden(true);
            }
        }
    }
    search_user();
}


void TeamsWindow::on_Admins_checkbox_stateChanged(int arg1)
{
    for (int i = 0; i < ui->team_persons_tree_widget->topLevelItemCount(); ++i)
    {
        QTreeWidgetItem *item = ui->team_persons_tree_widget->topLevelItem(i);

        if (ui->Admins_checkbox->isChecked())
        {
            if (item->text(3)=="YES")
            {
                item->setHidden(false);
            }
        }
        else if (!ui->Admins_checkbox->isChecked())
        {
            if (item->text(3)=="YES")
            {
                item->setHidden(true);
            }
        }
    }
    search_user();
}

