#include "teamswindow.h"
#include "ui_teamswindow.h"
#include <QTimer>

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


void TeamsWindow::connectionMaker(QString ip, int port)
{
    this_ip = ip;
    this_port = port;
    socket = new QTcpSocket();
    socket->connectToHost(ip, port);
    connect(socket, &QTcpSocket::connected, this, &TeamsWindow::socket_connected);
    connect(socket, &QTcpSocket::readyRead, this, &TeamsWindow::socket_readyRead);
    connect(socket, &QTcpSocket::bytesWritten, this, &TeamsWindow::socket_bytesWritten);
    connect(socket, &QTcpSocket::disconnected, this, &TeamsWindow::socket_disconnected);
    qDebug() << "Team win connected!";

}

void TeamsWindow::socket_readyRead()
{
    QByteArray data = socket->readAll();
    responseChecker(QString(data));
}

void TeamsWindow::socket_connected()
{
}

void TeamsWindow::socket_bytesWritten()
{
}

void TeamsWindow::socket_disconnected()
{
}

void TeamsWindow::sendRequest(QString s)
{
    QByteArray data = s.toUtf8();
    socket->write(data);
    qDebug() << s;
}

void TeamsWindow::responseChecker(QString s)
{
    qDebug() << "response get is: " << s;
    QJsonDocument doc = QJsonDocument::fromJson(s.toUtf8());
    QString resState = doc.object().value("res-state").toString();

    if (resState =="team-persons-list-ok")
    {
        show_team_persons(s);
    }
    if (resState =="add-new-person-to-team-ok")
    {
    QString mess = doc.object().value("message").toString();
    QMessageBox::information(this, "Added", mess);
    loadTeamPersons();
    }
    if (resState =="change-person-role-in-team-ok")
    {
        QString mess = doc.object().value("message").toString();
        QMessageBox::information(this, "changed", mess);
        loadTeamPersons();
    }
    if (resState =="remove-person-from-team-ok")
    {
        QString mess = doc.object().value("message").toString();
        QMessageBox::information(this, "changed", mess);
        loadTeamPersons();
    }


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

void TeamsWindow::admin_access(bool a , bool b)
{
    is_admin = a;
    is_org_admin = b;
}

void TeamsWindow::this_team_id_maker(QString a)
{
    this_team.teamSetId(a);
}

void TeamsWindow::this_user_maker(QString username)
{
    this_user = username;
}

void TeamsWindow::loadTeamPersons()
{
    QJsonObject jsonObject;
    jsonObject["req-type"] = "team-persons-list";
    jsonObject["teamname"] = this_team.teamGetName();
    jsonObject["orgname"] = this_org;
    QJsonDocument jsonDocument(jsonObject);
    QString req = jsonDocument.toJson(QJsonDocument::Compact);
    sendRequest(req);
}

void TeamsWindow::show_team_persons(QString s)
{
    ui->team_persons_tree_widget->clear();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(s.toUtf8());
    QJsonObject jsonObj = jsonDoc.object();
    QJsonArray teamPersons = jsonObj["team-persons"].toArray();

    for (const QJsonValue &personValue : teamPersons) {
        QJsonObject person = personValue.toObject();
        QStringList personData;
        personData << person["name"].toString() << person["username"].toString() << person["org_id"].toString();
        QString isTeamAdmin = person["is_team_admin"].toBool() ? "Yes" : "No";
        personData << isTeamAdmin;
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->team_persons_tree_widget, personData);
        ui->team_persons_tree_widget->addTopLevelItem(item);
    }
}


void TeamsWindow::on_teams_add_person_BTN_clicked()
{
    if(is_admin == true)
    {
        OrgDialog * s = new OrgDialog(this);
        s->set_text("Enter Username of the Person: ");
        connect(s,SIGNAL(name_readed(QString)),this,SLOT(add_team_person(QString)));
        s->show();
    }
    else
    {
        QMessageBox::warning(this, "Not found", "You are NOT Team or Organization Admin!");
    }
}

void TeamsWindow::add_team_person(QString new_username)
{
    QJsonObject jsonObject;
    jsonObject["req-type"] = "add-new-person-to-team";
    jsonObject["teamname"] = this_team.teamGetName();
    jsonObject["orgname"] = this_org;
    jsonObject["username"] = new_username;
    QJsonDocument jsonDocument(jsonObject);
    QString req = jsonDocument.toJson(QJsonDocument::Compact);
    sendRequest(req);
}

void TeamsWindow::addNewTeamPerson(QString s)
{


}

void TeamsWindow::on_teams_change_role_BTN_clicked()
{
    if(is_admin == true)
    {
        QTreeWidgetItem *selectedItem = ui->team_persons_tree_widget->currentItem();
        if (selectedItem)
        {
            if(selectedItem->text(1) == this_user && is_org_admin == false)
            {
                QMessageBox::warning(this, "Self role", "You can NOT demote yourself!");
            }
            else
            {
                changeTeamPersonRole(ui->team_persons_tree_widget->currentItem()->text(1));
            }
        }
        else
            QMessageBox::warning(this, "Select a person", "select a person from the list!");
        ui->team_persons_tree_widget->clearSelection();
    }
    else
    {
        QMessageBox::warning(this, "Not found", "You are NOT Team or Organization Admin!");
    }
}

void TeamsWindow::changeTeamPersonRole(QString username)
{
    QJsonObject jsonObject;
    jsonObject["req-type"] = "change-person-role-in-team";
    jsonObject["teamname"] = this_team.teamGetName();
    jsonObject["orgname"] = this_org;
    jsonObject["username"] = username;
    QJsonDocument jsonDocument(jsonObject);
    QString req = jsonDocument.toJson(QJsonDocument::Compact);
    sendRequest(req);
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
    if(is_admin == true)
    {
        QTreeWidgetItem *selectedItem = ui->team_persons_tree_widget->currentItem();
        if (selectedItem)
        {
            if(selectedItem->text(1) == this_user && is_org_admin == false)
            {
                QMessageBox::warning(this, "Self Remove", "You can NOT remove yourself!");
            }
            else
            {
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, "Delete a person", "Are you sure?\nDeleteing is not returnable!", QMessageBox::Yes | QMessageBox::No);
                if (reply == QMessageBox::Yes)
                {
                    removeTeamPerson(ui->team_persons_tree_widget->currentItem()->text(1));
                }
            }
        }
        else
            QMessageBox::warning(this, "Select a person", "select a person from the list!");
        ui->team_persons_tree_widget->clearSelection();
    }
    else
    {
        QMessageBox::warning(this, "Not found", "You are NOT Team or Organization Admin!");
    }
}

void TeamsWindow::removeTeamPerson(QString user)
{
    QJsonObject jsonObject;
    jsonObject["req-type"] = "remove-person-from-team";
    jsonObject["teamname"] = this_team.teamGetName();
    jsonObject["orgname"] = this_org;
    jsonObject["username"] = user;
    QJsonDocument jsonDocument(jsonObject);
    QString req = jsonDocument.toJson(QJsonDocument::Compact);
    sendRequest(req);
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


void TeamsWindow::closeEvent(QCloseEvent *event)
{
    qDebug() << "team win Disconnected!";
    socket->close();
    emit reconnect(this_ip,this_port);
}

