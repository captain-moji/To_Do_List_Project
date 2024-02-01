#include "connection.h"
#include "ui_connection.h"
#include <signup.h>
#include <person.h>
#include <todolist.h>

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
    QByteArray data = s.toUtf8();
    qDebug() << "response is: " << data;
    socket->write(data);
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
    if (reqtype == "user-reset-pasword")
    {
        QString temp_username = jsonObject["username"].toString();
        send_user_question(temp_username);
    }
    if (reqtype == "user-new-pasword")
    {
        QString temp_username = jsonObject["username"].toString();
        QString new_password = jsonObject["new-password"].toString();
        change_user_password(temp_username, new_password);
    }
    if (reqtype == "add-new-org")
    {
        temp_org_person.perSetUsername(jsonObject["username"].toString());
        QString org_name = jsonObject["orgname"].toString();
        temp_org_person.perSetName(jsonObject["name"].toString());
        temp_org_person.perSetID(jsonObject["id"].toString());
        add_new_org(org_name);
    }
    if (reqtype == "remove-org")
    {
        QString temp_username = jsonObject["username"].toString();
        QString org_name = jsonObject["orgname"].toString();
        remove_org(temp_username,org_name);
    }
    if (reqtype == "user-check-admin")
    {
        QString temp_username = jsonObject["username"].toString();
        QString org_name = jsonObject["orgname"].toString();
        check_org_admin(temp_username,org_name);
    }
    if (reqtype == "org-persons-list")
    {
        QString org_name = jsonObject["orgname"].toString();
        org_persons_list_maker(org_name);
    }
    if (reqtype == "add-user-to-org" )
    {
        QString username = jsonObject["username"].toString();
        QString org_name = jsonObject["orgname"].toString();
        add_new_user_to_org(username,org_name);
    }
    if (reqtype == "remove-user-from-org")
    {
        QString username = jsonObject["username"].toString();
        QString org_name = jsonObject["orgname"].toString();
        remove_user_from_org(username,org_name);
    }
    if (reqtype == "change-user-role-in-org")
    {
        QString username = jsonObject["username"].toString();
        QString org_name = jsonObject["orgname"].toString();
        change_user_role_in_org(username,org_name);
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

void Connection::send_user_question (QString username)
{
    QString file_Path = QDir::currentPath() + "/APPDATA/ALL_PERSONS.json";
    QFile file(file_Path);
    file.open(QIODevice::ReadOnly);
    QByteArray rawData = file.readAll();
    file.close();

    QJsonDocument doc(QJsonDocument::fromJson(rawData));
    QJsonArray jsonArray = doc.array();

    QString question;
    QString answer;
    foreach (const QJsonValue & value, jsonArray) {
        QJsonObject obj = value.toObject();
        if (obj["username"].toString() == username) {
            question = obj["question"].toString();
            answer = obj["answer"].toString();
            break;
        }
    }

    if (question == "")
    {
        QJsonObject jsonObject3;
        jsonObject3["res-state"] = "reset-password-failed";
        jsonObject3["Message"] = "User Not Found!";
        QJsonDocument jsonDocument2(jsonObject3);
        resp = jsonDocument2.toJson(QJsonDocument::Compact);
        serverResponse(resp);
    }
    else
    {
        QJsonObject jsonObject3;
        jsonObject3["res-state"] = "reset-password-ok";
        jsonObject3["question"] = question;
        jsonObject3["answer"] = answer;
        jsonObject3["username"] = username;
        QJsonDocument jsonDocument2(jsonObject3);
        resp = jsonDocument2.toJson(QJsonDocument::Compact);
        serverResponse(resp);
    }
}

void Connection::change_user_password (QString username, QString new_password)
{
    QString file_Path = QDir::currentPath() + "/APPDATA/ALL_PERSONS.json";
    QFile file(file_Path);
    file.open(QIODevice::ReadOnly);
    QByteArray rawData = file.readAll();
    file.close();
    QJsonDocument doc(QJsonDocument::fromJson(rawData));
    QJsonArray jsonArray = doc.array();
    for (int i = 0; i < jsonArray.size(); i++) {
        QJsonObject obj = jsonArray[i].toObject();
        if (obj["username"].toString() == username) {
            obj["password"] = new_password;
            jsonArray[i] = obj;
            break;
        }
    }

    doc.setArray(jsonArray);
    file.open(QIODevice::WriteOnly);
    file.write(doc.toJson());
    file.close();
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

    QJsonDocument doc2 = QJsonDocument::fromJson(jsonString.toUtf8());
    QJsonArray array = doc2.array();
    QJsonObject object;
    object.insert("res-state", "orgs-list");
    object.insert("orgs", array);
    QString newJson = QJsonDocument(object).toJson();
    serverResponse(newJson);
}

void Connection::add_new_org(QString org)
{
    bool exsist = false;
    QString file_Path = QDir::currentPath() + "/APPDATA/ALL_ORGANIZATIONS.json";
    QFile file(file_Path);
    file.open(QIODevice::ReadOnly);
    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    QJsonArray array = doc.array();
    for (int i = 0; i < array.size(); i++)
    {
        if (org == array.at(i).toString())
        {
            exsist = true;
            break;
        }
    }

    if (exsist == true)
    {
        QJsonObject object;
        object.insert("res-state", "make-org-failed");
        object.insert("Message", "Organization with this name is exsist!");
        QString newJson = QJsonDocument(object).toJson();
        serverResponse(newJson);
    }

    else
    {
        ToDoList * s ;
        s->makeOrgFiles(org);

        file.open(QIODevice::ReadWrite);
        QByteArray jsonData = file.readAll();
        file.close();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
        QJsonArray jsonArray = jsonDoc.array();
        jsonArray.append(org);
        jsonDoc.setArray(jsonArray);
        file.open(QIODevice::WriteOnly);
        file.write(jsonDoc.toJson());
        file.close();

        add_user_to_org(true, org);
    }
}

void Connection::add_user_to_org(bool is_admin, QString org)
{
    temp_org_person.orgPerSetID(temp_org_person.orgIdGenerator());
    temp_org_person.orgPerSetIsAdmin(is_admin);

    QString file_Path2 = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/"+org +"/ORG_PERSONS.json";
    QFile file2(file_Path2);
    file2.open(QIODevice::ReadWrite);

    QByteArray jsonData2 = file2.readAll();
    QJsonDocument jsonDoc2 = QJsonDocument::fromJson(jsonData2);
    QJsonArray jsonArray2 = jsonDoc2.array();

    for (int i = 0; i < jsonArray2.size(); ++i)
    {
        QJsonObject jsonObject = jsonArray2.at(i).toObject();
        if (jsonObject.value("username").toString() == temp_org_person.perGetUsername())
        {
            file2.close();
            return;
        }
    }

    QJsonObject newJsonObject;
    newJsonObject["id"] = temp_org_person.perGetId();
    newJsonObject["org_id"] = temp_org_person.orgPerGetId();
    newJsonObject["name"] = temp_org_person.perGetName();
    newJsonObject["username"] = temp_org_person.perGetUsername();
    newJsonObject["is_admin"] = temp_org_person.orgPerGetIsAdmin();

    jsonArray2.append(newJsonObject);
    jsonDoc2.setArray(jsonArray2);
    file2.resize(0);
    file2.write(jsonDoc2.toJson());
    file2.close();


    QJsonObject object;
    object.insert("res-state", "make-org-ok");
    object.insert("Message", "Organization created!");
    object.insert("org-id", temp_org_person.orgPerGetId());
    QString newJson = QJsonDocument(object).toJson();
    serverResponse(newJson);
}

void Connection::remove_org(QString username,QString org)
{
    QString file_Path2 = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/"+org +"/ORG_PERSONS.json";
    QFile file(file_Path2);
    file.open(QIODevice::ReadOnly);
    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    QJsonArray jsonArray = doc.array();

    bool found = false;
    bool is_admin = false;
    for (int i = 0; i < jsonArray.size(); ++i)
    {
        QJsonObject obj = jsonArray[i].toObject();
        if (obj["username"].toString() == username)
        {
            found = true;
            if (obj["is_admin"].toBool())
            {
                is_admin = true;
            }
            else
                is_admin = false;

            break;
        }
    }

    if (!found)
    {
        QJsonObject object;
        object.insert("res-state", "remove_org_faild");
        object.insert("Message", "Not_found");
        QString newJson = QJsonDocument(object).toJson();
        serverResponse(newJson);
        return;
    }
    else
    {
        if (is_admin == true)
        {
            ToDoList *s = new ToDoList;
            s->removeOrgFiles(org);

            QString file_Path = QDir::currentPath() + "/APPDATA/ALL_ORGANIZATIONS.json";

            QFile file(file_Path);
            file.open(QIODevice::ReadOnly);
            QByteArray jsonData = file.readAll();
            file.close();

            QJsonDocument doc = QJsonDocument::fromJson(jsonData);
            QJsonArray jsonArray = doc.array();

            for (int i = 0; i < jsonArray.size(); ++i) {
                if (jsonArray[i].toString() == org) {
                    jsonArray.removeAt(i);
                    break;
                }
            }

            QFile saveFile(file_Path);
            saveFile.open(QIODevice::WriteOnly);
            QJsonDocument saveDoc(jsonArray);
            saveFile.write(saveDoc.toJson());
            saveFile.close();

            QJsonObject object2;
            object2.insert("res-state", "remove-org-ok");
            object2.insert("message", "Organization Deleted!");
            QString newJson = QJsonDocument(object2).toJson();
            serverResponse(newJson);
        }
        else
        {
            QJsonObject object;
            object.insert("res-state", "remove-org-failed");
            object.insert("message", "You do Not have permission!");
            QString newJson = QJsonDocument(object).toJson();
            serverResponse(newJson);
        }
    }
}

void Connection::check_org_admin(QString username,QString org)
{
    QString file_Path2 = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/"+org +"/ORG_PERSONS.json";
    QFile file(file_Path2);
    file.open(QIODevice::ReadOnly);
    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    QJsonArray jsonArray = doc.array();

    bool is_admin = false;
    for (int i = 0; i < jsonArray.size(); ++i)
    {
        QJsonObject obj = jsonArray[i].toObject();
        if (obj["username"].toString() == username)
        {
            if (obj["is_admin"].toBool())
            {
                is_admin = true;
            }

            break;
        }
    }

        if (is_admin == true)
        {
            QJsonObject object2;
            object2.insert("res-state", "is-admin-checked");
            object2.insert("is-admin", true);
            QString newJson = QJsonDocument(object2).toJson();
            serverResponse(newJson);
        }
        else
        {
            QJsonObject object;
            object.insert("res-state", "is-admin-checked");
            object.insert("is-admin", false);
            QString newJson = QJsonDocument(object).toJson();
            serverResponse(newJson);
        }
}

void Connection::org_persons_list_maker(QString org)
{
    QString Path = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/" + org + "/ORG_PERSONS.json";
    QFile file(Path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return ;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    QJsonObject obj;
    obj["res-state"] = "org-persons-list-ok";
    obj["org-persons"] = doc.array();

    QJsonDocument resultDoc(obj);
    QString resultString = resultDoc.toJson();
    serverResponse(resultString);
}

void Connection::add_new_user_to_org(QString user, QString org)
{
    bool exsist = check_user_exsist_in_server(user,org);
    if (exsist == false)
    {
        QJsonObject object;
        object.insert("res-state", "user-not-found-in-server");
        object.insert("message", "user with this username is not found");
        QString newJson = QJsonDocument(object).toJson();
        serverResponse(newJson);
    }
    else
    {
        QString per_file = QDir::currentPath() + "/APPDATA/ALL_PERSONS.json";
        QFile file(per_file);
        if (!file.open(QIODevice::ReadOnly))
        {
            return;
        }

        QByteArray jsonData = file.readAll();
        file.close();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
        QJsonArray jsonArray = jsonDoc.array();

        for (int i = 0; i < jsonArray.size(); ++i)
        {
            QJsonObject jsonObject = jsonArray.at(i).toObject();

            QString username = jsonObject.value("username").toString();
            QString name = jsonObject.value("name").toString();
            QString id = jsonObject.value("id").toString();

            if (username == user)
            {
                temp_org_person.perSetID(id);
                temp_org_person.perSetName(name);
                temp_org_person.perSetUsername(username);
                temp_org_person.orgPerSetID(temp_org_person.orgIdGenerator());
                temp_org_person.orgPerSetIsAdmin(false);
                break;
            }
        }

        QString file_Path2 = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/"+org+"/ORG_PERSONS.json";
        QFile file2(file_Path2);
        file2.open(QIODevice::ReadWrite);

        QByteArray jsonData2 = file2.readAll();
        QJsonDocument jsonDoc2 = QJsonDocument::fromJson(jsonData2);
        QJsonArray jsonArray2 = jsonDoc2.array();
        bool already_exsist = false;
        for (int i = 0; i < jsonArray2.size(); ++i)
        {
            QJsonObject jsonObject = jsonArray2.at(i).toObject();
            if (jsonObject.value("username").toString() == user)
            {
                file.close();
                already_exsist = true;
            }
        }

        if (already_exsist == false)
        {
            QJsonObject newJsonObject;
            newJsonObject["id"] = temp_org_person.perGetId();
            newJsonObject["org_id"] = temp_org_person.orgPerGetId();
            newJsonObject["name"] = temp_org_person.perGetName();
            newJsonObject["username"] = temp_org_person.perGetUsername();
            newJsonObject["is_admin"] = temp_org_person.orgPerGetIsAdmin();

            jsonArray2.append(newJsonObject);
            jsonDoc.setArray(jsonArray2);
            file2.resize(0);
            file2.write(jsonDoc.toJson());
            file2.close();


            QJsonObject object;
            object.insert("res-state", "user-added-to-org");
            object.insert("message", "The user Has been added to organization");
            QString newJson = QJsonDocument(object).toJson();
            serverResponse(newJson);
        }
        else
        {
            QJsonObject object;
            object.insert("res-state", "user-not-found-in-server");
            object.insert("message", "this user allready exsist in organization");
            QString newJson = QJsonDocument(object).toJson();
            serverResponse(newJson);
        }
    }
}

bool Connection::check_user_exsist_in_server(QString username, QString org)
{
    QString file_Path = QDir::currentPath() + "/APPDATA/ALL_PERSONS.json";
    QFile file(file_Path);
    file.open(QIODevice::ReadOnly);

    QByteArray jsonData = file.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(jsonData));
    QJsonArray jsonArray = doc.array();

    for (const QJsonValue& value : jsonArray) {
        QJsonObject obj = value.toObject();
        if (obj["username"].toString() == username) {
            return true;
        }
    }
    return false;
}

void Connection::remove_user_from_org(QString user, QString org)
{
    OrganizationsWindow * t = new OrganizationsWindow;
    t->this_org_maker(org);
    t->removeUserFromOrganization(user);
    QJsonObject object;
    object.insert("res-state", "user-remove-from-org-ok");
    object.insert("message", "user has been deleted from org!");
    QString newJson = QJsonDocument(object).toJson();
    serverResponse(newJson);
}

void Connection::change_user_role_in_org(QString user, QString org)
{
    OrganizationsWindow * t = new OrganizationsWindow;
    t->this_org_maker(org);
    t->promoteToAdmin(user);
    QJsonObject object;
    object.insert("res-state", "user-role-changed-ok");
    object.insert("message", "changed!");
    QString newJson = QJsonDocument(object).toJson();
    serverResponse(newJson);
}



void Connection::on_pushButton_clicked()
{
    serverResponse("HELLO");
}

