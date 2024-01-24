#ifndef FIRSTPAGE_H
#define FIRSTPAGE_H

#include <QMainWindow>
#include <signup.h>
#include <login.h>
#include <person.h>
#include <connection.h>
#include <QJsonArray>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>
#include <QDebug>
#include <QMessageBox>
#include <todolist.h>
#include <QCryptographicHash>


namespace Ui {
class FirstPage;
}

class FirstPage : public QMainWindow
{
    Q_OBJECT

public:
    explicit FirstPage(QWidget *parent = nullptr);
    ~FirstPage();

private slots:

    void on_first_page_login_BTN_clicked();
    void username_checker(QString u);
    void password_checker(QString p);

    void on_first_page_signin_BTN_clicked();

private:
    bool user_correct;
    QString username;
    QString password;

    Ui::FirstPage *ui;
    Connection channel;

signals:
    void who_logged_in(QString id);
};

#endif // FIRSTPAGE_H
