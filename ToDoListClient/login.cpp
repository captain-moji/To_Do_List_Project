#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}

void Login::on_login_page_BTN_accepted()
{
    emit username_readed(ui->login_username_line_edit->text());
    emit password_readed(ui->login_password_line_edit->text());
}

void Login::on_login_forget_password_BTN_clicked()
{
    emit req_reset_pass("reset");
    on_login_page_BTN_rejected();
}

void Login::on_login_page_BTN_rejected()
{

}

