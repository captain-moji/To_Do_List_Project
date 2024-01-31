#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

signals:
    void username_readed(QString);
    void password_readed(QString);
    void req_reset_pass(QString);

private slots:
    void on_login_page_BTN_accepted();


    void on_login_forget_password_BTN_clicked();

    void on_login_page_BTN_rejected();

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
