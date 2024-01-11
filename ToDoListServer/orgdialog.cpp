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
    } else
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

