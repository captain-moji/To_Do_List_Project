#ifndef ORGDIALOG_H
#define ORGDIALOG_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class OrgDialog;
}

class OrgDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OrgDialog(QWidget *parent = nullptr);
    ~OrgDialog();

signals:
    void organization_readed(QString);

private slots:
    void on_page_BTN_accepted();

private:
    Ui::OrgDialog *ui;
};

#endif // ORGDIALOG_H
