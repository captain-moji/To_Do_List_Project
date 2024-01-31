#include "connection.h"
#include "ui_connection.h"
#include <signup.h>
#include <person.h>

Connection::Connection(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Connection)
{
    ui->setupUi(this);
}

Connection::~Connection()
{
    delete ui;
}


void Connection::serverTurnOn()
{
    if ( server != nullptr )
    {
        qDebug ()<< "server allready listening" ;

    }

    else {
        server = new QTcpServer();
        server->listen(QHostAddress::Any, 1234);

        if ( server->isListening() ) {
            connect(server, &QTcpServer::newConnection, this, &Connection::server_newConnection);
            qDebug ()<< "server is listening" ;
        }
    }
}

void Connection::serverTurnOff()
{
    if ( server == nullptr )
    {
        //ui.text->append("haven't connected yet");
    }

    else {
        if ( socket != nullptr )
            delete socket;
        delete server;
        socket = nullptr;
        server = nullptr;
        //ui.text->append("disconnected");
    }
}


void Connection::serverResponse(QString s)
{
    //if ( socket != nullptr )
    //{
    QByteArray data = s.toUtf8();
    qDebug() << "response is: " << data;
    socket->write(data);
    // }
}


void Connection::serverReqActions(QString received_message)
{
    qDebug() << received_message;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(received_message.toUtf8());
    QJsonObject jsonObject = jsonDocument.object();
    QString reqtype = jsonObject["req-type"].toString();

    if(reqtype == "user-login")
    {
    QString temp_user = jsonObject["username"].toString();
    QString temp_pass = jsonObject["password"].toString();
    login_user(temp_user, temp_pass);
    }
    if(reqtype == "user-signup")
    {
        QString temp_username = jsonObject["username"].toString();
        QString temp_pass = jsonObject["password"].toString();
        QString temp_name =jsonObject["name"].toString();
        QString temp_question =jsonObject["question"].toString();
        QString temp_answer = jsonObject["answer"].toString();
        qDebug () << "user for check:" <<temp_username;
        signup_user(temp_username, temp_pass , temp_name, temp_question, temp_answer);
    }
    if(reqtype == "user-load-orgs")
    {
        QString temp_username = jsonObject["username"].toString();
        load_user_organizations(temp_username);
    }



}


void Connection::server_newConnection()
{
    socket = new QTcpSocket();
    socket = server->nextPendingConnection();
    QObject::connect(socket, &QTcpSocket::readyRead, this, &Connection::socket_readyRead);
    QObject::connect(socket, &QTcpSocket::bytesWritten, this, &Connection::socket_bytesWritten);
    QObject::connect(socket, &QTcpSocket::disconnected, this, &Connection::socket_disConnected);
}

void Connection::socket_readyRead()
{
    QByteArray data = socket->readAll();
    qDebug () << "data that get in server" <<QString (data);
    serverReqActions(QString(data));
}


void Connection::socket_bytesWritten()
{

}

void Connection::socket_disConnected()
{



}


void Connection::login_user(QString username, QString password)
{
    QString file_Path = QDir::currentPath() + "/APPDATA/ALL_PERSONS.json";
    QFile file(file_Path);
    file.open(QFile::ReadOnly);
    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    QString usernameValue;
    QString nameValue;
    QString idValue;
    QString passwordValue;
    bool is_exsist = false;

    QJsonArray jsonArray = doc.array();
    foreach (const QJsonValue& value, jsonArray) {
        QJsonObject obj = value.toObject();
        if (obj["username"].toString() == username) {
            nameValue = obj["name"].toString();
            idValue = obj["id"].toString();
            passwordValue=obj["password"].toString();
            usernameValue=obj["username"].toString();
            is_exsist = true;
            break;
        }
    }

    if (is_exsist == true  )
    {
        if (passwordValue == password)
        {
            QJsonObject jsonObject3;
            jsonObject3["res-state"] = "login-ok";
            jsonObject3["username"] = usernameValue;
            jsonObject3["name"] = nameValue;
            jsonObject3["id"] = idValue;
            QJsonDocument jsonDocument2(jsonObject3);
            resp = jsonDocument2.toJson(QJsonDocument::Compact);
            serverResponse(resp);
        }
        else
        {
            QJsonObject jsonObject3;
            jsonObject3["res-state"] = "password-incorrect";
            jsonObject3["message"] = "Password is not correct!";
            QJsonDocument jsonDocument2(jsonObject3);
            resp = jsonDocument2.toJson(QJsonDocument::Compact);
            serverResponse(resp);
        }
    }
    else
    {
        QJsonObject jsonObject3;
        jsonObject3["res-state"] = "username-not-found";
        jsonObject3["message"] = "Username is not found!";
        QJsonDocument jsonDocument2(jsonObject3);
        resp = jsonDocument2.toJson(QJsonDocument::Compact);
        serverResponse(resp);
    }
}

void Connection::signup_user(QString username, QString password, QString name, QString question, QString answer)
{
    Signup * s;
    if (s->isUsernameExsist(username) == true )
    {
        QJsonObject jsonObject3;
        jsonObject3["res-state"] = "username-is-exsist";
        jsonObject3["message"] = "Username is already exsist, Take anotherone!";
        QJsonDocument jsonDocument2(jsonObject3);
        resp = jsonDocument2.toJson(QJsonDocument::Compact);
        serverResponse(resp);
    }
    else
    {
        QString per_file = QDir::currentPath() + "/APPDATA/ALL_PERSONS.json";
        QFile file(per_file);
        file.open(QIODevice::ReadOnly);
        QByteArray jsonData = file.readAll();
        file.close();
        QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonData);
        QJsonArray jsonArray = jsonDocument.array();
        QJsonObject newPersonObject;
        newPersonObject["name"] = name;
        newPersonObject["username"] = username;
        newPersonObject["question"] = question;
        newPersonObject["answer"] = answer;
        newPersonObject["password"] = password;
        Person*p;
        QString id = p->idGenerator();
        newPersonObject["id"] = id ;

        jsonArray.append(newPersonObject);

        QJsonDocument newJsonDocument(jsonArray);
        QByteArray newJsonData = newJsonDocument.toJson();
        file.open(QIODevice::WriteOnly);
        file.write(newJsonData);
        file.close();
        QJsonObject jsonObject3;
        jsonObject3["res-state"] = "signup-ok";
        jsonObject3["message"] = "sign up successful";
        QJsonDocument jsonDocument2(jsonObject3);
        resp = jsonDocument2.toJson(QJsonDocument::Compact);
        serverResponse(resp);
    }
}

void Connection::load_user_organizations(QString username)
{

    QString file_Path = QDir::currentPath() + "/APPDATA/ALL_ORGANIZATIONS.json";
    QFile file(file_Path);
    file.open(QIODevice::ReadOnly);

    QByteArray jsonData = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    QJsonArray jsonArray = doc.array();

    QJsonArray newJsonArray;
    foreach (const QJsonValue &value, jsonArray) {
        if (value.isString())
        {
            QString strValue = value.toString();
            QString Path = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/" +strValue + "/ORG_PERSONS.json";
            QFile file(Path);
            file.open(QIODevice::ReadOnly);
            QByteArray jsonData = file.readAll();
            QJsonDocument doc = QJsonDocument::fromJson(jsonData);
            QJsonArray jsonArray = doc.array();
            for (int i = 0; i < jsonArray.size(); ++i)
            {
            QJsonObject obj = jsonArray[i].toObject();
                if (obj["username"].isString() && obj["username"].toString() == username)
                {
                newJsonArray.append(strValue);
                    break;
                }
            }
        }
    }
    QJsonDocument newDoc(newJsonArray);
    QString jsonString = QString::fromUtf8(newDoc.toJson());
    qDebug() << "output organizations is:" << jsonString;
    //serverResponse(jsonString);
}


void Connection::on_pushButton_clicked()
{
    serverResponse("HELLO");
}

