#include "firstpage.h"
#include "ui_firstpage.h"

FirstPage::FirstPage(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FirstPage)
{
    channel.setIp(ui->ip->text());
    channel.setPort(ui->port->text().toInt());
    ui->setupUi(this);
    make_data_folder();
    make_admin_file();
}


FirstPage::~FirstPage()
{
    delete ui;
}


void FirstPage::make_data_folder()
{
    ;
}

void FirstPage::make_admin_file()
{
    ;
}


void FirstPage::on_first_page_login_BTN_clicked()
{

}

void FirstPage::on_first_page_signin_BTN_clicked()
{

}

