#include "todolist.h"
#include "firstpage.h"
#include "signup.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //FirstPage f;
    //f.show();

    ToDoList t;
    t.show();

    return a.exec();
}
