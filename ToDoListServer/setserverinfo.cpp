#include "setserverinfo.h"
#include "ui_setserverinfo.h"

SetServerinfo::SetServerinfo(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SetServerinfo)
{
    ui->setupUi(this);
}

SetServerinfo::~SetServerinfo()
{
    delete ui;
}

void SetServerinfo::on_setuserpass_BTN_accepted()
{
    emit set_user_pass(ui->user->text(),ui->pass->text());
}

