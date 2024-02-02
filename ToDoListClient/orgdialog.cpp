#include "orgdialog.h"
#include "ui_orgdialog.h"
#include <QMessageBox>

OrgDialog::OrgDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OrgDialog)
{
    ui->setupUi(this);
}

OrgDialog::~OrgDialog()
{
    delete ui;
}

void OrgDialog::set_text(QString a)
{
    ui->new_organization_text->setText(a);
}

void OrgDialog::on_page_BTN_accepted()
{
    if (ui->new_organization_line_edit->text() == "")
    {
        QMessageBox::warning(this, "Error!" , "Enter a name for organization!");
    }
    else
    {
        for (auto a : ui->new_organization_line_edit->text()) {
            if(a == '/'){
                QMessageBox::warning(this, "Error!" , "Invalid name!");
                return;
            }
        }
        emit name_readed(ui->new_organization_line_edit->text());
        this->close();
    }
}


bool OrgDialog::is_exsist(QString new_name)
{
    if (type == "ORG_DIALOG")
    {
        QString file_Path = QDir::currentPath() + "/APPDATA/ALL_ORGANIZATIONS.json";
        QFile file(file_Path);
        if (!file.open(QIODevice::ReadOnly))
        {
            return false;
        }
        QByteArray jsonData = file.readAll();
        file.close();

        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
        QJsonArray jsonArray = jsonDoc.array();

        for (const QJsonValue& value : jsonArray)
        {
            if (value.isString() && value.toString() == new_name)
            {
                return true;
            }
        }
        return false;
    }

    if (type == "TEAM_DIALOG")
    {

        return false;
    }

    if (type == "PROJECT_DIALOG")
    {
        QString file_Path = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/" + ORG + "/ORG_PROJECTS.json" ;

        QFile file(file_Path);
        if (!file.open(QIODevice::ReadOnly))
        {
            return true;
        }

        QByteArray jsonData = file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(jsonData);
        QJsonArray jsonArray = doc.array();
        for (const QJsonValue& value : jsonArray) {
            QJsonObject obj = value.toObject();
            QString currentName = obj.value("project_name").toString();
            if (currentName == new_name)
            {
                return true;
            }
        }
        return false;
    }

    if (type == "RESET_REQUEST")
    {
        return false;
    }
}
