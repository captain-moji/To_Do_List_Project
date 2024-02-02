#include "todolist.h"
#include "ui_todolist.h"

ToDoList::ToDoList(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ToDoList)
{
    ui->setupUi(this);
}

ToDoList::~ToDoList()
{
    delete ui;
}

void ToDoList::connectionMaker(QString ip , int port)
{
    this_port = port;
    this_ip = ip;
    connection = new QTcpSocket();
    connection->connectToHost(ip, port);
    connect(connection, &QTcpSocket::connected, this, &ToDoList::socket_connected);
    connect(connection, &QTcpSocket::readyRead, this, &ToDoList::socket_readyRead);
    connect(connection, &QTcpSocket::bytesWritten, this, &ToDoList::socket_bytesWritten);
    connect(connection, &QTcpSocket::disconnected, this, &ToDoList::socket_disconnected);
}

void ToDoList::thisUserMaker(QString username, QString id, QString name)
{
    this_user.perSetID(id);
    this_user.perSetName(name);
    this_user.perSetUsername(username);
    ui->user_name_label->setText(this_user.perGetName());
}

void ToDoList::socket_readyRead()
{
    QByteArray data = connection->readAll();
    responseChecker(QString(data));
}

void ToDoList::socket_connected()
{

}

void ToDoList::socket_bytesWritten()
{

}

void ToDoList::socket_disconnected()
{

}

void ToDoList::sendRequest(QString s)
{
    QByteArray data = s.toUtf8();
    connection->write(data);
}

void ToDoList::responseChecker(QString s)
{
    QJsonDocument doc = QJsonDocument::fromJson(s.toUtf8());
    QString resState = doc.object().value("res-state").toString();
    if (resState == "orgs-list")
    {
        showUserOrgs(s);
    }
    if (resState == "make-org-failed")
    {
        QMessageBox ::warning( this, "Error" ,"This name is exsist!\nTry another name!");
    }
    if (resState == "make-org-ok")
    {
        this_user_org_id = doc.object().value("org-id").toString();
        QMessageBox ::information(this, "OK" ,"Organization created!\nYou Are the admin!");
        loadOrganizations();
    }
    if (resState == "remove-org-failed")
    {
        QString message = doc.object().value("message").toString();
        QMessageBox ::warning(this, "Error" ,message);
    }
    if (resState == "remove-org-ok")
    {
        QMessageBox ::information(this, "OK" ,"The organization Deleted!");
        QListWidgetItem *item = ui->todolist_organizations_list->currentItem();
        if (item!=nullptr) {
            delete item;
        }
        item=nullptr;
        loadOrganizations();
    }
    if (resState =="edit-org-failed")
    {
        QMessageBox ::warning(this, "Errot" ,"You Do NOT have permission!");
    }
    if (resState == "edit-org-ok")
    {
        QMessageBox ::information(this, "OK" ,"The organization Edited!");
        loadOrganizations();
    }
}


void ToDoList::showUserOrgs(QString list)
{
    qDebug () << "list is:" << list;
    QJsonDocument doc = QJsonDocument::fromJson(list.toUtf8());
        QJsonArray array = doc.object().value("orgs").toArray();
        for (int i = 0; i < array.size(); i++) {
            ui->todolist_organizations_list->addItem(array.at(i).toString());
        }
}

void ToDoList::on_add_organization_BTN_clicked()
{
    OrgDialog *temp_org_dialog = new OrgDialog(this);
    temp_org_dialog->type="ORG_DIALOG";
    temp_org_dialog->set_text("Enter Name of New organization:");
    connect(temp_org_dialog, SIGNAL(name_readed(QString)), SLOT(add_organization(QString)));
    temp_org_dialog->show();
}

void ToDoList::add_organization(QString orgname)
{
    QJsonObject jsonObject;
    jsonObject["req-type"] = "add-new-org";
    jsonObject["username"] = this_user.perGetUsername();
    jsonObject["orgname"] = orgname;
    jsonObject["id"] = this_user.perGetId();
    jsonObject["name"] = this_user.perGetName();
    QJsonDocument jsonDocument(jsonObject);
    QString req = jsonDocument.toJson(QJsonDocument::Compact);
    sendRequest(req);
}

void ToDoList::on_edit_organization_BTN_clicked()
{
    QListWidgetItem *item = ui->todolist_organizations_list->currentItem();
    if (item != nullptr)
    {
        OrgDialog *temp_org_dialog = new OrgDialog(this);
        temp_org_dialog->type="ORG_DIALOG";
        temp_org_dialog->set_text("Enter New Name for this organization:");
        connect(temp_org_dialog, SIGNAL(name_readed(QString)), SLOT(edit_organization(QString)));
        temp_org_dialog->show();
    }
}

void ToDoList::edit_organization(QString a)
{
    QJsonObject jsonObject;
    jsonObject["req-type"] = "edit-org";
    jsonObject["username"] = this_user.perGetUsername();
    jsonObject["old_orgname"] = ui->todolist_organizations_list->currentItem()->text();
    jsonObject["new_orgname"] = a;
    QJsonDocument jsonDocument(jsonObject);
    QString req = jsonDocument.toJson(QJsonDocument::Compact);
    sendRequest(req);
}

void ToDoList::on_remove_organization_BTN_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Delete a Organization", "Are you sure?\nDeleteing is not returnable!", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
    QListWidgetItem *item = ui->todolist_organizations_list->currentItem();
    QJsonObject jsonObject;
    jsonObject["req-type"] = "remove-org";
    jsonObject["username"] = this_user.perGetUsername();
    jsonObject["orgname"] = ui->todolist_organizations_list->currentItem()->text();
    QJsonDocument jsonDocument(jsonObject);
    QString req = jsonDocument.toJson(QJsonDocument::Compact);
    sendRequest(req);
    }
}

void ToDoList::saveOrganizations()
{
    QJsonArray jsonArray;
    for (int i = 0; i < ui->todolist_organizations_list->count(); ++i)
    {
            QString itemText = ui->todolist_organizations_list->item(i)->text();
            jsonArray.append(itemText);
    }
    QJsonDocument jsonDoc(jsonArray);
    QString file_Path = QDir::currentPath() + "/APPDATA/ALL_ORGANIZATIONS.json";
    QFile file(file_Path);
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(jsonDoc.toJson());
        file.close();
    }

}

void ToDoList::loadOrganizations()
{
    ui->todolist_organizations_list->clear();
    QJsonObject jsonObject;
    jsonObject["req-type"] = "user-load-orgs";
    jsonObject["username"] = this_user.perGetUsername();
    QJsonDocument jsonDocument(jsonObject);
    QString req = jsonDocument.toJson(QJsonDocument::Compact);
    sendRequest(req);
}


void ToDoList::searchOrganizations()
{
    QString s = ui->search_organizations_qstring->text();
    for (int i = 0; i < ui->todolist_organizations_list->count(); ++i)
    {
        QListWidgetItem *item = ui->todolist_organizations_list->item(i);
        if (item->text().startsWith(s)) {
            item->setHidden(false);
        } else {
            item->setHidden(true);
        }
    }
}

void ToDoList::on_search_organizations_qstring_textChanged(const QString &arg1)
{
    searchOrganizations();
}

void ToDoList::on_sort_organizations_BTN_clicked()
{
    ui->todolist_organizations_list->sortItems();
}

void ToDoList::on_actionSort_Organizations_triggered()
{
    on_sort_organizations_BTN_clicked();
}

void ToDoList::on_actionExit_triggered()
{
    this->close();
}

void ToDoList::on_actionHelp_triggered()
{
    QMessageBox ::information( this, "Help" ,"You can add, edit or remove Organizations that you want.\nAlso, when you close the program, the organizations will not destroyed!");;
}

void ToDoList::on_actionAbout_triggered()
{
    QMessageBox ::information( this, "About" ,"ToDoList Manager\nVersion 1.0.0\nCreated by Moji & Mammad");;
}

void ToDoList::on_todolist_organizations_list_itemDoubleClicked(QListWidgetItem *item)
{
    connection->disconnect();
    OrganizationsWindow * w = new OrganizationsWindow;
    w->setWindowTitle("Organization Management");
    this->close();
    w->show();
    w->connectionMaker(this_ip,this_port);
    w->this_org_maker(ui->todolist_organizations_list->currentItem()->text());
    w->this_user_maker(this_user,this_user_org_id);
}

void ToDoList::on_add_new_user_BTN_clicked()
{
    allServerUsers* s = new allServerUsers(this);
    s->show();
}

void ToDoList::on_Refresh_orgs_BTN_clicked()
{
    loadOrganizations();
}

