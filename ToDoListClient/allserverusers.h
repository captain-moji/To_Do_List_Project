#ifndef ALLSERVERUSERS_H
#define ALLSERVERUSERS_H

#include <QMainWindow>
#include <person.h>
#include <signup.h>
#include <QJsonArray>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>
#include <QDebug>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QTreeWidgetItem>
#include <QMap>

namespace Ui {
class allServerUsers;
}

class allServerUsers : public QMainWindow
{
    Q_OBJECT

public:
    explicit allServerUsers(QWidget *parent = nullptr);
    ~allServerUsers();
    void btn_disable();

private slots:
    void username_setter(QString);
    void password_setter(QString);
    void name_setter(QString);
    void question_setter(QString);
    void answer_setter(QString);
    void person_saver();

    void on_all_users_add_user_BTN_clicked();

    void on_all_users_remove_user_BTN_clicked();

    void on_all_persons_tree_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void delete_person_from_server(QString);

    void on_all_users_search_line_edit_textChanged(const QString &arg1);

signals:
    void org_name_signal(QString);
    void user_selected(QString);

private:
    void search_user();
    void all_persons_display();
    Person temp_person;
    Ui::allServerUsers *ui;

};

#endif // ALLSERVERUSERS_H
