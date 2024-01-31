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

void ToDoList::connectionMaker(QTcpSocket * s) //QString ip, int port
{
    connection = s;
    qDebug () << " we are here! ";
    qDebug () << connection->isReadable();
    qDebug () << connection->isWritable();
    qDebug () << connection->peerAddress();
    connect(connection, &QTcpSocket::connected, this, &ToDoList::socket_connected);
    connect(connection, &QTcpSocket::readyRead, this, &ToDoList::socket_readyRead);
    connect(connection, &QTcpSocket::bytesWritten, this, &ToDoList::socket_bytesWritten);
    connect(connection, &QTcpSocket::disconnected, this, &ToDoList::socket_disconnected);
    qDebug() << "socket created" ;
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
    QJsonDocument jsonDocument2 = QJsonDocument::fromJson(s.toUtf8());
    QJsonObject jsonObject2 = jsonDocument2.object();
    QString reqState = jsonObject2["res-state"].toString();

    if (reqState == "user-org-list-ok")
    {


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

void ToDoList::add_organization(QString item)
{


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
    QString old_name ;
    QListWidgetItem *item = ui->todolist_organizations_list->currentItem();
    if (item != nullptr)
    {
        old_name= item->text();
        item->setText(a);
    }

    QString path = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/" + old_name;
    QString newpath = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/" + a;

    QDir sDir(path);
    if(sDir.exists())
    {
        sDir.rename(path,newpath);
    }
    saveOrganizations();
}

void ToDoList::on_remove_organization_BTN_clicked()
{
    QListWidgetItem *item = ui->todolist_organizations_list->currentItem();
    QString filename;
    if (item!=nullptr) {
        filename = item->text();
        delete item;
    }
    saveOrganizations();
    item=nullptr;
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
    OrganizationsWindow * w = new OrganizationsWindow(this);
    connect (this,SIGNAL(org_name_signal(QString)),w,SLOT(this_org_maker(QString)));
    emit org_name_signal(item->text());
    w->setWindowTitle("Organization Management");
    w->loadAllOrgPersons();
    w->loadAllOrgTeams();
    w->loadAllOrgProjects();
    w->show();
}

void ToDoList::on_add_new_user_BTN_clicked()
{
    allServerUsers* s = new allServerUsers(this);
    s->show();
}


