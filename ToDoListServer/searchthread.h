#ifndef SEARCHTHREAD_H
#define SEARCHTHREAD_H

#include <QThread>
#include <todolist.h>

class SearchThread : public QThread
{
    Q_OBJECT

public:
    SearchThread();
    SearchThread(ToDoList *todolist);

protected:
    void run() override;

private:
    ToDoList *todolist;
};
#endif // SEARCHTHREAD_H
