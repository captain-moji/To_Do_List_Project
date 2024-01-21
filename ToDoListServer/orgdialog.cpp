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

void OrgDialog::on_page_BTN_accepted()
{
    if (ui->new_organization_line_edit->text() == "")
    {
        QMessageBox::warning(this, "Error!" , "Enter a name for organization!");
    }
    else if (is_org_exsist(ui->new_organization_line_edit->text()) == true )
    {
        QMessageBox::warning(this, "Error!" , "This organization is exsist, try another name!");
    }
    else
    {
        for (auto a : ui->new_organization_line_edit->text()) {
            if(a == '/'){
                QMessageBox::warning(this, "Error!" , "Invalid name for organization!");
                return;
            }
        }
        emit organization_readed(ui->new_organization_line_edit->text());
        this->close();
    }

}

bool OrgDialog::is_org_exsist(QString new_name)
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

