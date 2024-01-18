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

    QString file_Path2 = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/"+this_org+"/ORG_PERSONS.json";

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
    all_org_persons_display();
}


void OrganizationsWindow::removeUserFromOrganization(QString user)
{
    QString file_Path = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/"+this_org+"/ORG_PERSONS.json";
    QFile file(file_Path);
    if (!file.open(QIODevice::ReadWrite)) {
        qWarning() << "Could not open file";
        return;
    }

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
    all_org_persons_display();
}


void OrganizationsWindow::promoteToAdmin(QString user)
{
    QString file_Path = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/"+this_org+"/ORG_PERSONS.json";
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
                    QMessageBox::information(this, "demoted!", "User demoted to member!");
                }
                else
                {
                    jsonObject["is_admin"] = true;
                    QMessageBox::information(this, "Promoted!", "User promoted to admin!");
                }
                jsonArray[i] = jsonObject;
                break;
            }
        }

        QByteArray updatedData = QJsonDocument(jsonArray).toJson(QJsonDocument::Indented);

        file.open(QIODevice::WriteOnly | QIODevice::Text);
        file.write(updatedData);
        file.close();
    }
    all_org_persons_display();
    on_org_users_search_line_edit_textChanged(0);
}


void OrganizationsWindow::this_org_maker(QString a)
{
    this_org = a;
    ui->org_name_lable->setText(this_org);
}


void OrganizationsWindow::makeAllpersonsFile()
{
    QString file_Path = QDir::currentPath() + "/APPDATA/ALL_PERSONS.json";
    QFile file(file_Path);
    if (!file.exists())
    {
        QFile n_file(file_Path);
        n_file.open(QFile::WriteOnly);
        n_file.close();
    }
}


void OrganizationsWindow::on_org_page_add_person_BTN_clicked()
{
    all_org_persons_display();
    allServerUsers * a = new allServerUsers(this);
    connect (a,SIGNAL(user_selected(QString)),this,SLOT(addUserToOrganization(QString)));
    a->btn_disable();
    a->show();
}


void OrganizationsWindow::all_org_persons_display()
{

    ui->org_persons_tree_widget->clear();
    QString file_Path = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/"+this_org+"/ORG_PERSONS.json";
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
            QString selecteduser = selectedItem->text(1);
            promoteToAdmin(selecteduser);
    }
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
        }
    }
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

