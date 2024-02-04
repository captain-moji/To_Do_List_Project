#include "todolist.h"
#include "firstpage.h"
#include <QApplication>
#include <QSplashScreen>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSplashScreen* creating_server;
    creating_server = new QSplashScreen;
    creating_server->setPixmap(QPixmap(":/Server_start_page_frame.png"));
    creating_server->show();
    QTimer::singleShot(3000,creating_server,SLOT(close()));
    _sleep(5000);

    FirstPage f;
    f.setWindowTitle("Login To To Do List M&M Server");
    f.show();

    return a.exec();
}
