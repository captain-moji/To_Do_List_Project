#include "todolist.h"
#include "ui_todolist.h"

ToDoList::ToDoList(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ToDoList)
{
    ui->setupUi(this);
    makeOrganizationsFile();
    loadOrganizations();
}

ToDoList::~ToDoList()
{
    delete ui;
}

void ToDoList::makeOrgFiles(QString o)
{
    QString sPath2 = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/" + o + "/ORG_TEAMS" ;
    QDir sDir2(sPath2);
    if (!sDir2.exists())
    {
        sDir2.mkpath(".");
    }

    QString sPath3 = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/" + o + "/ORG_PROJECTS" ;
    QDir sDir3(sPath3);
    if (!sDir3.exists())
    {
        sDir3.mkpath(".");
    }

    QString per_file = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/" + o + "/ORG_PERSONS.json";

    QFile file(per_file);
    if (!file.exists()) {
        file.open(QIODevice::WriteOnly);
    }
    file.close();

    QString team_file = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/" + o + "/ORG_TEAMS.json";
    QFile file2(team_file);
    if (!file2.exists()) {
        file2.open(QIODevice::WriteOnly);
        file2.close();
    }

    QString pro_file = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/" + o + "/ORG_PROJECTS.json";
    QFile file3(pro_file);
    if (!file3.exists()) {
        file3.open(QIODevice::WriteOnly);
        file3.close();
    }
}

void ToDoList::removeOrgFiles(QString o)
{
    QString path = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/" + o;
    QDir sDir(path);
    if(sDir.exists())
    {
        sDir.removeRecursively();
    }
}

void ToDoList::makeOrganizationsFile()
{
    QString file_Path = QDir::currentPath() + "/APPDATA/ALL_ORGANIZATIONS.json";
    QFile file(file_Path);
    if (!file.exists())
    {
        QFile n_file(file_Path);
        n_file.open(QFile::WriteOnly);
        n_file.close();
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
    ui->todolist_organizations_list->addItem(item);
    saveOrganizations();
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
    removeOrgFiles(filename);
    saveOrganizations();
    item=nullptr;
}


void ToDoList::saveOrganizations()
{
    QJsonArray jsonArray;
    for (int i = 0; i < ui->todolist_organizations_list->count(); ++i)
    {
            QString itemText = ui->todolist_organizations_list->item(i)->text();
            makeOrgFiles(itemText);
            jsonArray.append(itemText);
    }
    QJsonDocument jsonDoc(jsonArray);
    QString file_Path = QDir::currentPath() + "/APPDATA/ALL_ORGANIZATIONS.json";
    QFile file(file_Path);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        file.write(jsonDoc.toJson());
        file.close();
    }

}


void ToDoList::loadOrganizations()
{
    QString file_Path = QDir::currentPath() + "/APPDATA/ALL_ORGANIZATIONS.json";
    QFile file(file_Path);
    file.open(QIODevice::ReadOnly);
    QString jsonContent = file.readAll();
    file.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonContent.toUtf8());
    QJsonArray jsonArray = jsonDoc.array();

    ui->todolist_organizations_list->clear();
    for (const QJsonValue& value : jsonArray) {
        if (value.isString()) {
            ui->todolist_organizations_list->addItem(value.toString());
        }
    }
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
    w->show();
    w->loadAllOrgPersons();
    w->loadAllOrgTeams();
}


void ToDoList::on_add_new_user_BTN_clicked()
{
    allServerUsers* s = new allServerUsers(this);
    s->show();
}


