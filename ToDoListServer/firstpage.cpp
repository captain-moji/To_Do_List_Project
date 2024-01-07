#include "firstpage.h"
#include "ui_firstpage.h"

FirstPage::FirstPage(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FirstPage)
{
    ui->setupUi(this);
    make_data_folder();

}

FirstPage::~FirstPage()
{
    delete ui;
}

void FirstPage::on_first_page_signup_BTN_clicked()
{
    Signup * s = new Signup (this);
    connect(s,SIGNAL(username_completed(QString)),this,SLOT(username_setter(QString)));
    connect(s,SIGNAL(password_completed(QString)),this,SLOT(password_setter(QString)));
    connect(s,SIGNAL(name_completed(QString)),this,SLOT(name_setter(QString)));
    connect(s,SIGNAL(question_completed(QString)),this,SLOT(question_setter(QString)));
    connect(s,SIGNAL(answer_completed(QString)),this,SLOT(answer_setter(QString)));
    qDebug() <<"hello";
    s->show();
}

void FirstPage::username_setter(QString s)
{
    temp_person.perSetUsername(s);
    temp_person.perSetID(temp_person.idGenerator());
    qDebug() << temp_person.perGetUsername();
}

void FirstPage::password_setter(QString p)
{
    temp_person.perSetPassword(p);
    qDebug() << temp_person.perGetPassword();
}


void FirstPage::question_setter(QString q)
{
    temp_person.perSetQuestion(q);
    qDebug() << temp_person.perGetQuestion();

}

void FirstPage::answer_setter(QString a)
{
    temp_person.perSetAnswer(a);
    qDebug() <<temp_person.perGetAnswer();
}

void FirstPage::name_setter(QString n)
{
    temp_person.perSetName(n);
    qDebug() << temp_person.perGetName();
    qDebug() <<"id:" <<temp_person.perGetId();
    person_saver();
}

void FirstPage:: person_saver()
{
    QString per_file = QDir::currentPath() + "/APPDATA/ALL_PERSONS.json";

    QFile file(per_file);
    if (!file.exists()) {

        file.open(QIODevice::WriteOnly);
        file.close();
        qDebug() <<"PER.json crreated:";
    }
    else
    {
        qDebug() <<"PER.json exsist:";
    }

    if (file.open(QIODevice::ReadOnly)) {
        QByteArray jsonData = file.readAll();
        file.close();

        QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonData);
        QJsonArray jsonArray = jsonDocument.array();
        QJsonObject newPersonObject;
        newPersonObject["name"] = temp_person.perGetName();
        newPersonObject["id"] = temp_person.perGetId();
        newPersonObject["username"] = temp_person.perGetUsername();
        newPersonObject["password"] = temp_person.perGetPassword();
        newPersonObject["question"] = temp_person.perGetQuestion();
        newPersonObject["answer"] = temp_person.perGetAnswer();

        jsonArray.append(newPersonObject);

        QJsonDocument newJsonDocument(jsonArray);
        QByteArray newJsonData = newJsonDocument.toJson();

        if (file.open(QIODevice::WriteOnly)) {
            file.write(newJsonData);
            file.close();
        }
    }
    else {
        qDebug() << "Could not open file.";
    }
}



void FirstPage::make_data_folder()
{
    QString path = QDir::currentPath() + "/APPDATA";

    if (!QDir(path).exists()) {
        QDir().mkdir(path);
        qDebug() << "maked";
    } else {
        qDebug() << "allreay exsist.";
    }
}




