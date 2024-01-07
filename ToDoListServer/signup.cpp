#include "signup.h"
#include "ui_signup.h"


Signup::Signup(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Signup)
{
    ui->setupUi(this);
}

Signup::~Signup()
{
    delete ui;
}

bool Signup::isPassCorrect(QString pass)
{

    std::string str;
    str = pass.toStdString();

    //std::string str = "Hello world";

    //QString qstr = QString::fromStdString(str);
    //std::string str =qs.toUtf8().constData();

    if (str.length() < 8) {
        return false;
    }

    std::regex lowercase_regex("[a-z]");
    if (!regex_search(str, lowercase_regex)) {
        return false;
    }

    std::regex uppercase_regex("[A-Z]");
    if (!regex_search(str, uppercase_regex)) {
        return false;
    }

    std::regex digit_regex("[0-9]");
    if (!regex_search(str, digit_regex)) {
        return false;
    }
    return true;
}

bool Signup::isUsernameExsist(QString username)
{
    QString per_file = QDir::currentPath() + "/APPDATA/ALL_PERSONS.json";
    QFile file(per_file);

        if (!file.open(QIODevice::ReadOnly)) {
            return false;
        }

        QByteArray jsonData = file.readAll();
        file.close();

        QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonData);
        if (!jsonDocument.isArray()) {
            return false;
        }

        QJsonArray jsonArray = jsonDocument.array();

        for (const QJsonValue& value : jsonArray) {
            if (value.isObject()) {
                QJsonObject jsonObject = value.toObject();
                QString jsonUsername = jsonObject.value("username").toString();

                if (jsonUsername == username) {
                    return true;
                }
            }
        }
        return false;
    }

void Signup::on_signup_ok_BTN_clicked()
{
    if (ui->signup_username_line_edit->text()=="" || ui->signup_password_line_edit->text()=="" || ui->signup_name_line_edit->text()=="" || ui->signup_answer_line_edit->text() ==""
        || ui->signup_question_selector->currentText() == "select ...")
    {
        QMessageBox ::information( this, "Error!" , "All fields are required!");
    }

    else if (Signup::isPassCorrect (ui->signup_password_line_edit->text()) == false)
    {
        QMessageBox ::information( this, "Error!" , "Password must Have: \na a-z letter\na A-Z letter\na Number\n8-12 digits");
    }
    else if(isUsernameExsist(ui->signup_username_line_edit->text()) == true)
    {
        QMessageBox ::information( this, "Error!" , "This Username is allready exsist!\nplease take another!");
    }
    else
    {
        emit username_completed(ui->signup_username_line_edit->text());
        emit password_completed(ui->signup_password_line_edit->text());
        emit question_completed(ui->signup_question_selector->currentText());
        emit answer_completed(ui->signup_answer_line_edit->text());
        emit name_completed(ui->signup_name_line_edit->text());
        QMessageBox ::information( this, "OK" , "sign up successful!");
        this->close();
    }
}


void Signup::on_signup_cancel_BTN_clicked()
{
    this->close();
}

