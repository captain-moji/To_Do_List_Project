#include "allserverusers.h"
#include "ui_allserverusers.h"
#include <organizationswindow.h>

allServerUsers::allServerUsers(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::allServerUsers)
{
    ui->setupUi(this);
}

allServerUsers::~allServerUsers()
{
    delete ui;
}

void allServerUsers::typeSetter(QString type)
{
    if(type=="NEW_USER")
    {
    ui->enter_text->setText("Enter username:");

    }

}



void allServerUsers::on_add_to_org_BTN_clicked()
{
    emit name_selected(ui->name_line_edit->text());
    this->close();
}

