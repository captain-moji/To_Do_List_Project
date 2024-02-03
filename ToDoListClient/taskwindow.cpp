#include "taskwindow.h"
#include "ui_taskwindow.h"
#include <QStringList>
#include <QTreeWidgetItem>

TaskWindow::TaskWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TaskWindow)
{
    ui->setupUi(this);
    ui->person_label->hide();
    ui->persons_list_widget->hide();
    ui->team_label->hide();
    ui->teams_list_widget->hide();
}

TaskWindow::~TaskWindow()
{
    delete ui;
}

void TaskWindow::typeSetter(QString type)
{
    if(type == "NEW_TASK")
    {
        ui->save_task_BTN->hide();
        ui->owner_label->hide();
        ui->owner_text->hide();
        ui->archive_combobox->hide();
        ui->archive_label->hide();
    }
    if(type == "EDIT_TASK")
    {
        ui->make_new_task_BTN->hide();
    }
}


void TaskWindow::ownerListSetter(QStringList team_list, QStringList person_list)
{
    for (int i = 0; i < team_list.size(); ++i) {
        QListWidgetItem *item = new QListWidgetItem(team_list.at(i));
        ui->teams_list_widget->addItem(item);
    }
    for (int i = 0; i < person_list.size(); ++i) {
        QListWidgetItem *item = new QListWidgetItem(person_list.at(i));
        ui->persons_list_widget->addItem(item);
    }
}

void TaskWindow::thisOrgProjectSetter(QString t, QString p)
{
    this_org = t;
    this_project = p;
}

void TaskWindow::on_make_new_task_BTN_clicked()
{
    this_task.taskSetTitle(ui->title_line_edit->text());
    this_task.taskSetDescription(ui->description_text_edit->toPlainText());
    this_task.taskSetIsArchived(false);
    this_task.taskSetDate(ui->date_edit->date().toString("yyyy-MM-dd"));
    this_task.taskSetOwnerType(ui->Owner_type_combobox->currentText());
    this_task.taskSetPriority(ui->Priority_combobox->currentText());
    this_task.taskSetOwnerID(temp_owner_id);
    this_task.taskSetOwnerName(temp_owner_name);
    emit new_task_maked(this_task);
    this->close();
}

void TaskWindow::on_save_task_BTN_clicked()
{
    this_task.taskSetTitle(ui->title_line_edit->text());
    this_task.taskSetDescription(ui->description_text_edit->toPlainText());
    this_task.taskSetDate(ui->date_edit->date().toString("yyyy-MM-dd"));
    this_task.taskSetOwnerType(ui->Owner_type_combobox->currentText());
    this_task.taskSetPriority(ui->Priority_combobox->currentText());
    if(ui->archive_combobox->currentText() == "Yes")
        this_task.taskSetIsArchived(true);
    else
        this_task.taskSetIsArchived(false);

    if (temp_owner_id !="")
        this_task.taskSetOwnerID(temp_owner_id);
    if (temp_owner_name !="")
        this_task.taskSetOwnerName(temp_owner_name);

    emit task_edited(this_task);
    this->close();
}

void TaskWindow::this_task_maker(Task task)
{
    this_task.taskSetTitle(task.taskGetTitle());
    this_task.taskSetDate(task.taskGetDate());
    this_task.taskSetDescription(task.taskGetDescription());
    this_task.taskSetIsArchived(task.taskGetIsArchived());
    this_task.taskSetOwnerID(task.taskGetOwnerID());
    this_task.taskSetOwnerName(task.taskGetOwnerName());
    this_task.taskSetOwnerType(task.taskGetOwnerType());
    this_task.taskSetPriority(task.taskGetPriority());

    ui->archive_combobox->setCurrentText(this_task.taskGetIsArchived() ? "Yes" : "No");
    ui->title_line_edit->setText(this_task.taskGetTitle());
    ui->description_text_edit->appendPlainText(this_task.taskGetDescription());
    ui->Priority_combobox->setCurrentText(this_task.taskGetPriority());
    ui->owner_text->setText(this_task.taskGetOwnerName());
    ui->Owner_type_combobox->setCurrentText(this_task.taskGetOwnerType());
    QDate date = QDate::fromString(this_task.taskGetDate(), "yyyy-MM-dd");
    ui->date_edit->setDate(date);
}

void TaskWindow::owner_name_setter(QString n)
{
    temp_owner_name = n;
}

void TaskWindow::owner_id_setter(QString i)
{
    temp_owner_id = i;
}

void TaskWindow::on_teams_list_widget_itemClicked(QListWidgetItem *item)
{
    owner_name_setter(item->text());
}


void TaskWindow::on_persons_list_widget_itemClicked(QListWidgetItem *item)
{
    owner_name_setter(item->text());
}


void TaskWindow::on_Owner_type_combobox_currentTextChanged(const QString &arg1)
{
    if(arg1 == "Team")
    {
        ui->person_label->hide();
        ui->persons_list_widget->hide();
        ui->team_label->show();
        ui->teams_list_widget->show();
    }
    if(arg1 == "Person")
    {
        ui->person_label->show();
        ui->persons_list_widget->show();
        ui->team_label->hide();
        ui->teams_list_widget->hide();
    }
    if(arg1 == "Select ...")
    {
        ui->person_label->hide();
        ui->persons_list_widget->hide();
        ui->team_label->hide();
        ui->teams_list_widget->hide();
    }
}

