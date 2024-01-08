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



private:
    Ui::ToDoList *ui;

};
#endif // TODOLIST_H
