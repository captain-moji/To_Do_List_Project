#ifndef TODOLIST_H
#define TODOLIST_H

#include <QMainWindow>
#include <person.h>
#include <QString>
#include <person.h>
#include <QJsonArray>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>
#include <QDebug>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui {
class ToDoList;
}
QT_END_NAMESPACE

class ToDoList : public QMainWindow
{
    Q_OBJECT

public:
    ToDoList(QWidget *parent = nullptr);
    ~ToDoList();

private slots:
    void thisUserMaker(QString);

private:
    Ui::ToDoList *ui;
    QString this_user_id;
    Person this_user;
};
#endif // TODOLIST_H
