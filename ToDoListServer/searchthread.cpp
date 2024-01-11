#include "searchthread.h"

SearchThread::SearchThread() {}

SearchThread::SearchThread(ToDoList *todolist) : todolist(todolist)
{

}

void SearchThread::run()
{
    while (true) {
        QMetaObject::invokeMethod(todolist, "searchOrganizations", Qt::QueuedConnection);
        QThread::sleep(1);  // adjust the sleep duration as needed
    }
}
