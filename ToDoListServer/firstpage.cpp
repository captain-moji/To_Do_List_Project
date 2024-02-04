#include "firstpage.h"
#include "ui_firstpage.h"

FirstPage::FirstPage(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FirstPage)
{
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
    QString path = QDir::currentPath() + "/APPDATA";

    if (!QDir(path).exists()) {
        QDir().mkdir(path);
    }
}

void FirstPage::make_admin_file()
{
    QString per_file = QDir::currentPath() + "/APPDATA/SERVER_USER.json";
    if (!QFile(per_file).exists())
    {
        QFile jsonFile(per_file);
        QJsonObject json;
        json["username"] = "admin";
        QString pass = "ADMINadmin0";
        QCryptographicHash hash(QCryptographicHash::Sha256);
        QByteArray textBytes = pass.toUtf8();
        hash.addData(textBytes);
        QByteArray hashedText = hash.result();
        QString hashedHex = hashedText.toHex();
        QString hashed_pass=hashedHex;
        json["password"] = hashed_pass;

        QJsonDocument jsonDoc(json);
        QByteArray jsonData = jsonDoc.toJson();

        if (jsonFile.open(QFile::WriteOnly))
        {
            jsonFile.write(jsonData);
            jsonFile.close();
        }
    }
}


void FirstPage::on_first_page_login_BTN_clicked()
{
    Login * l = new Login(this);
    connect(l,SIGNAL(username_readed(QString)),this,SLOT(username_checker(QString)));
    connect(l,SIGNAL(password_readed(QString)),this,SLOT(password_checker(QString)));
    l->show();
}


void FirstPage::username_checker(QString f_user)
{
    QString file_Path = QDir::currentPath() + "/APPDATA/SERVER_USER.json";
    QFile jsonFile(file_Path);
    if (jsonFile.open(QFile::ReadOnly))
    {
        QByteArray jsonData = jsonFile.readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
        QJsonObject json = jsonDoc.object();


        if (json["username"].toString() == f_user)
            {
            username = json["username"].toString();
            password = json["password"].toString();
            }
        jsonFile.close();
        if (!username.isEmpty())
        {
            user_correct = 1;
        }
        else
        {
            user_correct = 0;
            QMessageBox ::warning( this, "Error!" , "this user not found!");
        }
    }
}


void FirstPage::password_checker(QString p)
{
    QCryptographicHash hash(QCryptographicHash::Sha256);
    QByteArray textBytes = p.toUtf8();
    hash.addData(textBytes);
    QByteArray hashedText = hash.result();
    QString hashedHex = hashedText.toHex();
    QString hashed_pass=hashedHex;

    if (password == hashed_pass  && user_correct == 1 )
    {
        QMessageBox ::information( this, "OK" , "Login successful!");
        ToDoList * td = new ToDoList();
        td->show();
        Connection * connection = new Connection;
        connection->SetPort(ui->port_text_edit->text().toInt());
        connection->serverTurnOn();
        this->close();
    }
    else if (password != hashed_pass  && user_correct == 1 )
    {
        user_correct = 0;
        username = "";
        password = "";
        QMessageBox ::warning( this, "Error!" , "Password is NOT Correct!");
    }
}


void FirstPage::on_set_server_BTN_clicked()
{
    port = ui->port_text_edit->text().toInt();
    QMessageBox ::information( this, "connected!" ,"connected!");
}

