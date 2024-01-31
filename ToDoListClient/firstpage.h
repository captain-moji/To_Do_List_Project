#ifndef FIRSTPAGE_H
#define FIRSTPAGE_H

#include <QMainWindow>
#include <signup.h>
#include <login.h>
#include <person.h>
#include <QJsonArray>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>
#include <QDebug>
#include <QMessageBox>
#include <todolist.h>
#include <QCryptographicHash>
#include <qtcpsocket.h>

namespace Ui {
class FirstPage;
}

class FirstPage : public QMainWindow
{
    Q_OBJECT

public:
    explicit FirstPage(QWidget *parent = nullptr);
    ~FirstPage();

public slots:
    void socket_readyRead();
    void socket_connected();
    void socket_bytesWritten();
    void socket_disconnected();

private slots:

    void on_first_page_login_BTN_clicked();
    void username_checker(QString);
    void password_checker(QString);

    void on_first_page_signin_BTN_clicked();
    void password_setter(QString);
    void username_setter(QString);
    void name_setter(QString);
    void question_setter(QString);
    void answer_setter(QString);

    void on_set_server_BTN_clicked();
    void sendRequest(QString s);
    void responseChecker(QString);

private:

    Ui::FirstPage *ui;
    QTcpSocket *socket = nullptr;

    QString username;
    QString password;

    QString response;
    QString ip;
    int port;

    QString temp_username;
    QString temp_password;
    QString temp_answer;
    QString temp_question;
    QString temp_name;

signals:

    void who_logged_in(QString id);
    void socket_signal(QTcpSocket *socket);
};

#endif // FIRSTPAGE_H
