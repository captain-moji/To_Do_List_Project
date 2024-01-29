#include "todolist.h"
#include "firstpage.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FirstPage f;
    f.setWindowTitle("Login To To Do List M&M Server");
    f.show();

    //ToDoList t;
    //t.setWindowTitle("To Do List M&M");
    //t.show();

    return a.exec();
}
