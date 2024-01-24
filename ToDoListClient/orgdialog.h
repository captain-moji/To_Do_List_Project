#ifndef ORGDIALOG_H
#define ORGDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDir>
#include <QSet>
#include <QJsonObject>


namespace Ui {
class OrgDialog;
}

class OrgDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OrgDialog(QWidget *parent = nullptr);
    ~OrgDialog();
    void set_text(QString);
    QString type;
    QString ORG;

signals:
    void name_readed(QString);
private slots:
    void on_page_BTN_accepted();
    bool is_exsist(QString);
private:
    Ui::OrgDialog *ui;
};

#endif // ORGDIALOG_H
