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
    getCommentFromTree();
    QString req = saveTaskComments();

    this_task.taskSetTitle(ui->title_line_edit->text());
    this_task.taskSetDescription(ui->description_text_edit->toPlainText());
    this_task.taskSetIsArchived(false);
    this_task.taskSetDate(ui->date_edit->date().toString("yyyy-MM-dd"));
    this_task.taskSetOwnerType(ui->Owner_type_combobox->currentText());
    this_task.taskSetPriority(ui->Priority_combobox->currentText());
    this_task.taskSetOwnerID(temp_owner_id);
    this_task.taskSetOwnerName(temp_owner_name);
    //emit new_task_maked(req, this_task);
    emit new_task_maked(this_task);

    this->close();
}

void TaskWindow::on_save_task_BTN_clicked()
{
    getCommentFromTree();
    QString req = saveTaskComments();

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

    //emit task_edited(req, this_task);
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

void TaskWindow::thisOrgSetter(QString o)
{
    this_org = o;
}

void TaskWindow::thisprojectSetter(QString p)
{
    this_project = p;
}

void TaskWindow::thistaskSetter(const Task &t)
{
    this_task = t;
}

void TaskWindow::thisCommentSetter(Comment &c)
{
    this_comment = c;
}

QVector<Comment> &TaskWindow::commentsGetter()
{
    return comments;
}

void TaskWindow::thisPersonSetter(const OrgPerson & p)
{
    this_person = p;
}

Comment TaskWindow::getCommentFromItem(QTreeWidgetItem *item) {
    Comment comment;
    comment.commentSetPerson(item->text(0));
    comment.commentSetText(item->text(1));
    comment.commentSetDate(item->text(2));

    for (int i = 0; i < item->childCount(); ++i) {
        comment.commentGetReplies().append(getCommentFromItem(item->child(i)));
    }

    return comment;
}

void TaskWindow::getCommentFromTree() {
    comments.clear();
    for (int i = 0; i < ui->task_comments_tree_widget->topLevelItemCount(); ++i) {
        comments.append(getCommentFromItem(ui->task_comments_tree_widget->topLevelItem(i)));
    }
}

void TaskWindow::on_task_add_new_comment_BTN_clicked()
{
    OrgDialog *temp_comment_dialog = new OrgDialog(this);
    temp_comment_dialog->windowTitle() = "Comment";
    temp_comment_dialog->set_text("Enter a new comment");
    connect(temp_comment_dialog, SIGNAL(name_readed(QString)), SLOT(add_comment(QString)));

    temp_comment_dialog->show();
}

void TaskWindow::add_comment(QString text)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(ui->task_comments_tree_widget);
    item->setText(0, this_person.perGetName());
    item->setText(1, text);
    item->setText(2, QDateTime::currentDateTime().toString());
}


void TaskWindow::loadTaskComments(QString s)
{
    ui->task_comments_tree_widget->clear();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(s.toUtf8());
    QJsonObject jsonObj = jsonDoc.object();
    QJsonArray commentsJson = jsonObj["task-comments"].toArray();

    for (int i = 0; i < commentsJson.size(); ++i) {
        Comment comment;
        comment.fromJson(commentsJson[i].toObject());
        comments.append(comment);
    }

}

void TaskWindow::addCommentsToTree()
{
    for (Comment &comment : comments) {
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->task_comments_tree_widget);
        item->setText(0, comment.commentGetPerson());
        item->setText(1, comment.commentGetText());
        item->setText(2, comment.commentGetDate());

        for(Comment &reply : comment.commentGetReplies()){
            addCommentToTree(reply, item);
        }

    }

}

QString TaskWindow::saveTaskComments()
{
    // QString Path = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/" + org_name + "/ORG_PROJECTS/" + proj_name + "/PROJECT_TASKS/" + task_title + "TASK_COMMENTS.json";
    // QFile file(Path);
    // file.open(QIODevice::ReadOnly);
    // QByteArray jsonData = file.readAll();
    // file.close();
    // QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    // QJsonObject obj;
    // obj["res-state"] = "load-task_comments-ok";
    // obj["task_comments"] = doc.array();
    // QJsonDocument newDoc(obj);
    // QString jsonString = QString::fromUtf8(newDoc.toJson());


    // QString file_Path = QDir::currentPath() + "/APPDATA/ORGANIZATIONS/" + this_org + "/ORG_PROJECTS/" + this_project + "/PROJECT_TASKS/" + this_task.taskGetTitle() + "/TASK_COMMENTS.json";
    QJsonArray commentsJson;
    for (const Comment &comment : comments) {
        commentsJson.append(comment.toJson());
    }
    QJsonDocument doc(commentsJson);

    QJsonObject obj;
    obj["req-state"] = "save-task_comments";
    obj["orgname"] = this_org;
    obj["projname"] = this_project;
    obj["tasktitle"] = this_task.taskGetTitle();
    obj["task_comments"] = doc.array();
    QJsonDocument newDoc(obj);
    QString jsonString = QString::fromUtf8(newDoc.toJson());
    return jsonString;

    // QFile file(file_Path);
    // if (file.open(QFile::WriteOnly)) {
    //     file.write(doc.toJson());
    // }

}

void TaskWindow::addCommentToTree(Comment &comment, QTreeWidgetItem *parent) {
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0, comment.commentGetPerson());
    item->setText(1, comment.commentGetText());
    item->setText(2, comment.commentGetDate());

    for (Comment &reply : comment.commentGetReplies()) {
        addCommentToTree(reply, item);
    }
}


void TaskWindow::on_task_add_new_reply_BTN_clicked()
{
    OrgDialog *temp_comment_dialog = new OrgDialog(this);
    temp_comment_dialog->windowTitle() = "Reply";
    temp_comment_dialog->set_text("Enter a new reply");
    connect(temp_comment_dialog, SIGNAL(name_readed(QString)), SLOT(add_reply(QString)));

    temp_comment_dialog->show();
}

void TaskWindow::add_reply(QString text)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(ui->task_comments_tree_widget->currentItem());
    item->setText(0, this_person.perGetName());
    item->setText(1, text);
    item->setText(2, QDateTime::currentDateTime().toString());
}

void TaskWindow::on_task_remove_comment_BTN_clicked()
{
    delete ui->task_comments_tree_widget->currentItem();
}

