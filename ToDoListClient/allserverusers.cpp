#include "allserverusers.h"
#include "ui_allserverusers.h"
#include <organizationswindow.h>

allServerUsers::allServerUsers(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::allServerUsers)
{
    ui->setupUi(this);
    all_persons_display();
}

allServerUsers::~allServerUsers()
{
    delete ui;
}

void allServerUsers::on_all_users_add_user_BTN_clicked()
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


void allServerUsers::username_setter(QString s)
{
    temp_person.perSetUsername(s);
    temp_person.perSetID(temp_person.idGenerator());
    qDebug() << temp_person.perGetUsername();
}

void allServerUsers::password_setter(QString p)
{
    temp_person.perSetPassword(p);
    qDebug() << temp_person.perGetPassword();
}


void allServerUsers::question_setter(QString q)
{
    temp_person.perSetQuestion(q);
    qDebug() << temp_person.perGetQuestion();

}

void allServerUsers::answer_setter(QString a)
{
    temp_person.perSetAnswer(a);
    qDebug() <<temp_person.perGetAnswer();
}

void allServerUsers::name_setter(QString n)
{
    temp_person.perSetName(n);
    qDebug() << temp_person.perGetName();
    qDebug() <<"id:" <<temp_person.perGetId();
    person_saver();
}

void allServerUsers:: person_saver()
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
        newPersonObject["question"] = temp_person.perGetQuestion();
        newPersonObject["answer"] = temp_person.perGetAnswer();

        QCryptographicHash hash(QCryptographicHash::Sha256);
        QByteArray textBytes = temp_person.perGetPassword().toUtf8();
        hash.addData(textBytes);
        QByteArray hashedText = hash.result();
        QString hashedHex = hashedText.toHex();

        newPersonObject["password"] = hashedHex;

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
    all_persons_display();
}

void allServerUsers::all_persons_display()
{
    ui->all_persons_tree->clear();
    QString per_file = QDir::currentPath() + "/APPDATA/ALL_PERSONS.json";
    QFile file(per_file);
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument doc(QJsonDocument::fromJson(jsonData));
    QJsonArray jsonArray = doc.array();

    QMap<QString, QTreeWidgetItem*> uniqueItems;

    for (int i = 0; i < jsonArray.size(); ++i) {
        QJsonObject jsonObject = jsonArray[i].toObject();

        QString name = jsonObject["name"].toString();
        QString id = jsonObject["id"].toString();
        QString username = jsonObject["username"].toString();

            QTreeWidgetItem *item = new QTreeWidgetItem(ui->all_persons_tree);
            item->setText(0, name);
            item->setText(2, id);
            item->setText(1, username);
            uniqueItems.insert(id, item);
            uniqueItems.insert(username, item);
        }
}


void allServerUsers::on_all_users_remove_user_BTN_clicked()
{
    QString deleted_user;
    if(ui->all_persons_tree->currentItem())
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Delete a person", "Are you sure?\nDeleteing is not returnable!\nUser will be delete from all organizations!", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            QString user= ui->all_persons_tree->currentItem()->text(1);
            delete_person_from_server(user);
            delete ui->all_persons_tree->currentItem();
        }
    }
}


void allServerUsers::on_all_persons_tree_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    emit user_selected(item->text(1));
    this->close();
}


void allServerUsers::delete_person_from_server(QString user)
{
    QString per_file = QDir::currentPath() + "/APPDATA/ALL_PERSONS.json";
    QFile file(per_file);

    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        qDebug() << "Could not open file for reading and writing.";
        return;
    }
    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);

    QJsonArray jsonArray = jsonDoc.array();

    bool found = false;
    for (int i = 0; i < jsonArray.size(); i++) {
        QJsonObject jsonObject = jsonArray[i].toObject();
        if (jsonObject.value("username").toString() == user) {
            jsonArray.removeAt(i);
            found = true;
            break;
        }
    }
    if (found)
    {
        QJsonDocument updatedJsonDoc(jsonArray);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
            return;
        }
        file.write(updatedJsonDoc.toJson());
        file.close();
    }




    QString per_file2 = QDir::currentPath() + "/APPDATA/ALL_ORGANIZATIONS.json";
    QFile file2(per_file2);
    if (!file2.open(QIODevice::ReadOnly)) {
        qWarning() << "Could not open file";
        return;
    }

    QByteArray jsonData2 = file2.readAll();
    file2.close();

    QJsonDocument doc2(QJsonDocument::fromJson(jsonData2));
    QJsonArray jsonArray2 = doc2.array();

    for (int i = 0; i < jsonArray2.size(); i++)
    {
        QString directoryName = jsonArray2[i].toString();
        OrganizationsWindow * w = new OrganizationsWindow();
        connect (this,SIGNAL(org_name_signal(QString)),w,SLOT(this_org_maker(QString)));
        emit org_name_signal(directoryName);
        w->removeUserFromOrganization(user);
    }

}


void allServerUsers::btn_disable()
{
ui->all_users_add_user_BTN->setHidden(true);
ui->all_users_remove_user_BTN->setHidden(true);
}


void allServerUsers::search_user()
{
    QString input = ui->all_users_search_line_edit->text();
    QString search_type;

    for (int i = 0; i < ui->all_persons_tree->topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = ui->all_persons_tree->topLevelItem(i);

        {
        if (ui->search_by_name_radio->isChecked())
            search_type = item->text(0);
        else if (ui->search_by_username_radio->isChecked())
            search_type = item->text(1);
        else if (ui->search_by_ID_radio->isChecked())
            search_type = item->text(2);
        }

        if (search_type.startsWith(input))
        {
            item->setHidden(false);
        } else {
            item->setHidden(true);
        }
    }
}

void allServerUsers::on_all_users_search_line_edit_textChanged(const QString &arg1)
{
    search_user();
}

