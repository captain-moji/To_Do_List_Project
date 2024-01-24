#ifndef PERSONSANDTEAMSWINDOW_H
#define PERSONSANDTEAMSWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QFile>
#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

namespace Ui {
class PersonsAndTeamsWindow;
}

class PersonsAndTeamsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PersonsAndTeamsWindow(QWidget *parent = nullptr);
    ~PersonsAndTeamsWindow();
    void typeSetter (QString type);
    void orgSetter(QString);
    void projectSetter(QString);
    void loadProjectTeams();
    void loadProjectPersons();
private slots:

    void on_teams_persons_tree_widget_itemDoubleClicked(QTreeWidgetItem *item, int column);

signals:

    void id_readed(QString);
    void name_readed(QString);

private:


    QString type;
    QString this_org;
    QString this_project;
    Ui::PersonsAndTeamsWindow *ui;
};

#endif // PERSONSANDTEAMSWINDOW_H
