#include "todolist.h"
#include "ui_todolist.h"

ToDoList::ToDoList(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ToDoList)
{
    ui->setupUi(this);
}

ToDoList::~ToDoList()
{
    delete ui;
}

void ToDoList::thisUserMaker(QString t)
{
    QString per_file = QDir::currentPath() + "/APPDATA/ALL_PERSONS.json";

    QFile file(per_file);
    file.open(QIODevice::ReadOnly);
    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);

    if (jsonDoc.isArray()) {
        QJsonArray jsonArray = jsonDoc.array();

        for (int i = 0; i < jsonArray.size(); ++i)
        {
            QJsonObject jsonObject = jsonArray.at(i).toObject();

            if (jsonObject.value("id").toString() == t)
            {
                this_user.perSetUsername(jsonObject.value("username").toString());
                this_user.perSetName(jsonObject.value("name").toString());
                this_user.perSetID(jsonObject.value("id").toString());
                break;
            }
        }
    }


    ui->textEdit->append(this_user.perGetUsername());
    ui->textEdit->append(this_user.perGetName());
    ui->textEdit->append(this_user.perGetId());
    ui->label->setText("ID: " + this_user.perGetId());
}
