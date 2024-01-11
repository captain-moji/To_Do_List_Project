QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    orgdialog.cpp \
    orgperson.cpp \
    searchthread.cpp \
    task.cpp \
    comment.cpp \
    firstpage.cpp \
    login.cpp \
    main.cpp \
    organization.cpp \
    person.cpp \
    project.cpp \
    signup.cpp \
    team.cpp \
    teamperson.cpp \
    todolist.cpp \

HEADERS += \
    orgdialog.h \
    orgperson.h \
    searchthread.h \
    task.h \
    comment.h \
    firstpage.h \
    login.h \
    organization.h \
    person.h \
    project.h \
    signup.h \
    team.h \
    teamperson.h \
    todolist.h \

FORMS += \
    firstpage.ui \
    login.ui \
    orgdialog.ui \
    signup.ui \
    todolist.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
