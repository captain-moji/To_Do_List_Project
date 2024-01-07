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

    void on_first_page_signup_BTN_clicked();
    void username_setter(QString);
    void password_setter(QString);
    void name_setter(QString);
    void question_setter(QString);
    void answer_setter(QString);
    void person_saver();

private:
    Person temp_person;
    void make_data_folder();
    Ui::FirstPage *ui;

};

#endif // FIRSTPAGE_H
