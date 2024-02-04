#ifndef SETSERVERINFO_H
#define SETSERVERINFO_H

#include <QDialog>

namespace Ui {
class SetServerinfo;
}

class SetServerinfo : public QDialog
{
    Q_OBJECT

public:
    explicit SetServerinfo(QWidget *parent = nullptr);
    ~SetServerinfo();

private slots:
    void on_setuserpass_BTN_accepted();

private:
    Ui::SetServerinfo *ui;
signals:
    void set_user_pass(QString,QString);
};

#endif // SETSERVERINFO_H
