#include "personsandteamswindow.h"
#include "ui_personsandteamswindow.h"

PersonsAndTeamsWindow::PersonsAndTeamsWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PersonsAndTeamsWindow)
{
    ui->setupUi(this);
}

PersonsAndTeamsWindow::~PersonsAndTeamsWindow()
{
    delete ui;
}


void PersonsAndTeamsWindow::orgSetter(QString t)
{
    this_org = t;
}

void PersonsAndTeamsWindow::projectSetter(QString t)
{
    this_project = t;
}



void PersonsAndTeamsWindow::loadProjectTeams()
{
    QString file_Path = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/"+ this_org +"/ORG_PROJECTS/"+ this_project + "/PROJECT_TEAMS.json";
    QFile file(file_Path);
    if (!file.open(QIODevice::ReadOnly))
    {
        return;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    if (!jsonDoc.isArray())
    {
        return ;
    }

    QJsonArray jsonArray = jsonDoc.array();

    foreach (const QJsonValue &value, jsonArray)
    {

            QJsonObject teamObject = value.toObject();
            QString teamName = teamObject["team_name"].toString();
            QString teamID = teamObject["team_id"].toString();
            QTreeWidgetItem *item = new QTreeWidgetItem(ui->teams_persons_tree_widget);
            item->setText(0, teamName);
            item->setText(1, teamID);
            ui->teams_persons_tree_widget->addTopLevelItem(item);
    }
}



void PersonsAndTeamsWindow::loadProjectPersons()
{
    QString file_Path = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/"+ this_org +"/ORG_PROJECTS/"+ this_project + "/PROJECT_PERSON.json";
    QFile file(file_Path);
    file.open(QIODevice::ReadOnly);

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    QJsonArray jsonArray = jsonDoc.array();

    for (const QJsonValue& jsonValue : jsonArray)
    {
        QJsonObject jsonObject = jsonValue.toObject();
        QString name = jsonObject.value("username").toString();
        QString Id = jsonObject.value("id").toString();

            QTreeWidgetItem* item = new QTreeWidgetItem(ui->teams_persons_tree_widget);
            item->setText(0, name);
            item->setText(1, Id);
            ui->teams_persons_tree_widget->addTopLevelItem(item);
    }
}


void PersonsAndTeamsWindow::on_teams_persons_tree_widget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    emit id_readed(item->text(1));
    emit name_readed(item->text(0));
    this->close();
}

