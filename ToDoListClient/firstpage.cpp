#include "firstpage.h"
#include "ui_firstpage.h"

FirstPage::FirstPage(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FirstPage)
{
    ui->setupUi(this);
    ui->ip_text_edit->setText("127.0.0.1");
    ui->port_text_edit->setText("1234");
}


FirstPage::~FirstPage()
{
    delete ui;
}

void FirstPage::socket_readyRead()
{
    QByteArray data = socket->readAll();
    QString s = QString (data);
    qDebug() << "respons from server is:" << s;
    response = s;
    responseChecker(s);
}


void FirstPage::socket_connected()
{

}

void FirstPage::socket_bytesWritten()
{

}

void FirstPage::socket_disconnected()
{

}

void FirstPage::sendRequest(QString s)
{
    if (socket != nullptr)
    {
        qDebug() << "rec that send is : " << s.toUtf8();
        socket->write(s.toUtf8());
    }
}


void FirstPage::responseChecker(QString response)
{
        QJsonDocument jsonDocument = QJsonDocument::fromJson(response.toUtf8());
        QJsonObject jsonObject = jsonDocument.object();
        QString reqState = jsonObject["res-state"].toString();

        if (reqState == "login-ok")
        {
            QMessageBox ::information( this, "OK!" ,"Welcome!");
            ToDoList * t = new ToDoList();
            this->close();
            t->show();
            connect(this,SIGNAL(socket_signal(QTcpSocket*)) ,t, SLOT(connectionMaker(QTcpSocket*)));
            emit socket_signal(socket);
            //t->connectionMaker(ip,port);
            t->thisUserMaker(jsonObject["username"].toString(),jsonObject["id"].toString(),jsonObject["name"].toString());
            //t->loadOrganizations();
        }
        if (reqState == "username-not-found" || reqState =="password-incorrect")
            QMessageBox::warning(this, "Error!" ,"Username or password is not correct!");

        if (reqState == "signup-ok")
        {
            QMessageBox::warning(this, "OK!" ,"You can Loggin Now!");
        }
        if (reqState =="username-is-exsist" )
        {
            QMessageBox::warning(this, "Username Exist!" ,"Try another username!");
        }
}


void FirstPage::on_first_page_login_BTN_clicked()
{
    if(socket == nullptr)
    {
        QMessageBox ::warning( this, "Not connected" ,"Please Set port and ip first!");
    }
    else
    {
        Login * l = new Login(this);
        connect(l,SIGNAL(username_readed(QString)),this,SLOT(username_checker(QString)));
        connect(l,SIGNAL(password_readed(QString)),this,SLOT(password_checker(QString)));
        l->show();
    }
}

void FirstPage::username_checker(QString u)
{
    username = u;
}


void FirstPage::password_checker(QString p)
{
    QCryptographicHash hash(QCryptographicHash::Sha256);
    QByteArray textBytes = p.toUtf8();
    hash.addData(textBytes);
    QByteArray hashedText = hash.result();
    QString hashedHex = hashedText.toHex();
    QString hashed_pass=hashedHex;
    password = hashed_pass;

    QJsonObject jsonObject;
    jsonObject["req-type"] = "user-login";
    jsonObject["username"] = username;
    jsonObject["password"] = password;

    QJsonDocument jsonDocument(jsonObject);
    QString req = jsonDocument.toJson(QJsonDocument::Compact);
    ui->textEdit->append(req);
    sendRequest(req);

}


void FirstPage::on_first_page_signin_BTN_clicked()
{
    if(socket == nullptr)
    {
        QMessageBox ::warning( this, "Not connected" ,"Please Set port and ip first!");
    }
    else
    {
        Signup * s = new Signup (this);
        connect(s,SIGNAL(username_completed(QString)),this,SLOT(username_setter(QString)));
        connect(s,SIGNAL(password_completed(QString)),this,SLOT(password_setter(QString)));
        connect(s,SIGNAL(name_completed(QString)),this,SLOT(name_setter(QString)));
        connect(s,SIGNAL(question_completed(QString)),this,SLOT(question_setter(QString)));
        connect(s,SIGNAL(answer_completed(QString)),this,SLOT(answer_setter(QString)));
        s->show();
    }
}


void FirstPage::username_setter(QString u)
{
    temp_username = u;
}


void FirstPage::password_setter(QString u)
{
    temp_password = u;
}


void FirstPage::question_setter(QString u)
{
    temp_question = u;
}


void FirstPage::answer_setter(QString u)
{
    temp_answer = u;
}


void FirstPage::name_setter(QString u)
{
    temp_name = u;

    QCryptographicHash hash(QCryptographicHash::Sha256);
    QByteArray textBytes = temp_password.toUtf8();
    hash.addData(textBytes);
    QByteArray hashedText = hash.result();
    QString hashedHex = hashedText.toHex();
    QString hashed_pass=hashedHex;

    QJsonObject jsonObject;
    jsonObject["req-type"] = "user-signup";
    jsonObject["username"] = temp_username;
    jsonObject["password"] = hashed_pass;
    jsonObject["name"] = temp_name;
    jsonObject["question"] = temp_question;
    jsonObject["answer"] = temp_answer;

    QJsonDocument jsonDocument(jsonObject);
    QString req = jsonDocument.toJson(QJsonDocument::Compact);
    ui->textEdit->append(req);
    sendRequest(req);
}


void FirstPage::on_set_server_BTN_clicked()
{
    if (ui->ip_text_edit->text() =="" || ui->port_text_edit->text() == "" )
    {
        QMessageBox ::warning( this, "set ip and port" ,"port and ip can not be empty!");
    }
    else
    {
        ip = ui->ip_text_edit->text();
        port = ui->port_text_edit->text().toInt();

        if (socket != nullptr )
        {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Connection Exsist!", "Already connected!\nReplace it?", QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::Yes)
            {
                socket->connectToHost(ip, port);
                connect(socket, &QTcpSocket::connected, this, &FirstPage::socket_connected);
                connect(socket, &QTcpSocket::readyRead, this, &FirstPage::socket_readyRead);
                connect(socket, &QTcpSocket::bytesWritten, this, &FirstPage::socket_bytesWritten);
                connect(socket, &QTcpSocket::disconnected, this, &FirstPage::socket_disconnected);
                QMessageBox ::information(this, "Replaced!" ,"Connected to new server!");
            }
        }

        else {
            socket = new QTcpSocket();
            socket->connectToHost(ip, port);
            connect(socket, &QTcpSocket::connected, this, &FirstPage::socket_connected);
            connect(socket, &QTcpSocket::readyRead, this, &FirstPage::socket_readyRead);
            connect(socket, &QTcpSocket::bytesWritten, this, &FirstPage::socket_bytesWritten);
            connect(socket, &QTcpSocket::disconnected, this, &FirstPage::socket_disconnected);
            QMessageBox ::information( this, "connection started!" ,"Connected!");
        }
    }
}


