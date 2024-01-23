#include "taskwindow.h"
#include "ui_taskwindow.h"

TaskWindow::TaskWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TaskWindow)
{
    ui->setupUi(this);
}

TaskWindow::~TaskWindow()
{
    delete ui;
}

void TaskWindow::typeSetter(QString type)
{
    if(type == "NEW_TASK")
    {
        ui->comments_BTN->hide();
        ui->save_task_BTN->hide();
        ui->owner_label->hide();
        ui->owner_text->hide();
    }
    if(type == "EDIT_TASK")
    {
        ui->make_new_task_BTN->hide();
    }
}

void TaskWindow::on_make_new_task_BTN_clicked()
{
    this_task.taskSetTitle(ui->title_line_edit->text());
    this_task.taskSetDescription(ui->description_text_edit->toPlainText());
    this_task.taskSetIsArchived(false);
    this_task.taskSetDate(ui->date_edit->date().toString("yyyy-MM-dd"));
    this_task.taskSetOwnerType(ui->owner_type_combobox->currentText());
    this_task.taskSetPriority(ui->Priority_combobox->currentText());
    this_task.taskSetOwnerID("1234");
    this_task.taskSetOwnerName("ali");
    emit new_task_maked(this_task);
    this->close();
}

