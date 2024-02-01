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
    void typeSetter(QString type);

private slots:
    void on_add_to_org_BTN_clicked();

signals:

    void name_selected(QString);

private:

    Person temp_person;
    Ui::allServerUsers *ui;

};

#endif // ALLSERVERUSERS_H
