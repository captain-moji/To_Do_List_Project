#include "signup.h"
#include "ui_signup.h"


Signup::Signup(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Signup)
{
    ui->setupUi(this);
    ui->reset_password_BTN->hide();
}

Signup::~Signup()
{
    delete ui;
}

void Signup::resetPasswordType()
{
    ui->signup_ok_BTN->hide();
    ui->signup_name_line_edit->hide();
    ui->signup_username_line_edit->hide();
    ui->signup_question_selector->hide();
    ui->signup_answer_line_edit->hide();
    ui->reset_password_BTN->show();
    ui->label->hide();
    ui->label_2->hide();
    ui->label_3->setText("New Password:");
    ui->label_4->hide();
    ui->label_5->hide();
    this->resize(500, 240);
}

bool Signup::isPassCorrect(QString pass)
{

    std::string str;
    str = pass.toStdString();

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
        QMessageBox ::warning(this, "Error!" , "All fields are required!");
    }

    else if (Signup::isPassCorrect (ui->signup_password_line_edit->text()) == false)
    {
        QMessageBox ::warning( this, "Error!" , "Password must Have: \na a-z letter\na A-Z letter\na Number\n8-12 characters");
    }
    else if(isUsernameExsist(ui->signup_username_line_edit->text()) == true)
    {
        QMessageBox ::warning( this, "Error!" , "This Username is allready exsist!\nplease take another!");
    }
    else
    {
        emit username_completed(ui->signup_username_line_edit->text());
        emit password_completed(ui->signup_password_line_edit->text());
        emit question_completed(ui->signup_question_selector->currentText());
        emit answer_completed(ui->signup_answer_line_edit->text());
        emit name_completed(ui->signup_name_line_edit->text());
        this->close();
    }
}


void Signup::on_signup_cancel_BTN_clicked()
{
    this->close();
}

void Signup::make_persons_file()
{
    QString file_Path = QDir::currentPath() + "/APPDATA/ALL_PERSONS.json";
    QFile file(file_Path);
    if (!file.exists())
    {
        QFile n_file(file_Path);
        n_file.open(QFile::WriteOnly);
        n_file.close();
    }
}



void Signup::on_reset_password_BTN_clicked()
{
    if (ui->signup_password_line_edit->text()=="")
    {
        QMessageBox ::warning(this, "Error!" , "password field is required!");
    }

    else if (Signup::isPassCorrect (ui->signup_password_line_edit->text()) == false)
    {
        QMessageBox ::warning( this, "Error!" , "Password must Have: \na a-z letter\na A-Z letter\na Number\n8-12 characters");
    }
    else
    {
        emit reset_password(ui->signup_password_line_edit->text());
        this->close();
    }
}

