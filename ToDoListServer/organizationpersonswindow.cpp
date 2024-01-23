#include "organizationpersonswindow.h"
#include "ui_organizationpersonswindow.h"

OrganizationPersonsWindow::OrganizationPersonsWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::OrganizationPersonsWindow)
{
    ui->setupUi(this);
    loadOrgPersons();
}

OrganizationPersonsWindow::~OrganizationPersonsWindow()
{
    delete ui;
}

void OrganizationPersonsWindow::loadOrgPersons()
{
    QString file_Path = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/" + ORG + "/ORG_PERSONS.json";
    QFile file(file_Path);
    file.open(QIODevice::ReadOnly);

    QByteArray jsonData = file.readAll();
    QJsonDocument jsonDoc(QJsonDocument::fromJson(jsonData));
    QJsonArray jsonArray = jsonDoc.array();

    for (const QJsonValue& jsonValue : jsonArray)
    {
        QJsonObject jsonObject = jsonValue.toObject();

        QString name = jsonObject.value("name").toString();
        QString username = jsonObject.value("username").toString();
        QString orgId = jsonObject.value("org_id").toString();
        bool isAdmin = jsonObject.value("is_admin").toBool();

        QTreeWidgetItem* item = new QTreeWidgetItem(ui->org_persons_tree_widget);
        item->setText(0, name);
        item->setText(1, username);
        item->setText(2, orgId);
        item->setText(3, isAdmin ? "YES" : "NO");
    }
}


void OrganizationPersonsWindow::on_org_persons_tree_widget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    emit org_person_selected(item->text(1));
    this->close();
}

void OrganizationPersonsWindow::search_user()
{
    QString input = ui->search_line_edit->text();
    QString search_type;

    for (int i = 0; i < ui->org_persons_tree_widget->topLevelItemCount(); ++i)
    {
        QTreeWidgetItem *item = ui->org_persons_tree_widget->topLevelItem(i);
        {
            if (ui->search_by_name_radio->isChecked())
                search_type = item->text(0);
            else if (ui->search_by_username_radio->isChecked())
                search_type = item->text(1);
            else if (ui->search_by_org_Id_radio->isChecked())
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


void OrganizationPersonsWindow::on_search_line_edit_textChanged(const QString &arg1)
{
    on_Admins_checkbox_stateChanged(0);
    on_members_checkbox_stateChanged(0);
    search_user();
}

void OrganizationPersonsWindow::on_members_checkbox_stateChanged(int arg1)
{
    for (int i = 0; i < ui->org_persons_tree_widget->topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = ui->org_persons_tree_widget->topLevelItem(i);

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


void OrganizationPersonsWindow::on_Admins_checkbox_stateChanged(int arg1)
{
    for (int i = 0; i < ui->org_persons_tree_widget->topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = ui->org_persons_tree_widget->topLevelItem(i);

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

