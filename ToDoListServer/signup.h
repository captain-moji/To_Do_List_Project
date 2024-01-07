#ifndef SIGNUP_H
#define SIGNUP_H

#include <QDialog>
#include <regex>
#include <string>
#include <QString>
#include <QMessageBox>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

namespace Ui {
class Signup;
}

class Signup : public QDialog
{
    Q_OBJECT

public:
    explicit Signup(QWidget *parent = nullptr);
    ~Signup();

signals:
    void username_completed (QString);
    void password_completed (QString);
    void question_completed (QString);
    void name_completed (QString);
    void answer_completed (QString);

private slots:
    bool isPassCorrect(QString);
    bool isUsernameExsist(QString);
    void on_signup_ok_BTN_clicked();

    void on_signup_cancel_BTN_clicked();

private:
    Ui::Signup *ui;
};

#endif // SIGNUP_H
