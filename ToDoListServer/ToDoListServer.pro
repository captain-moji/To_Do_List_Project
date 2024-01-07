QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    firstpage.cpp \
    login.cpp \
    main.cpp \
    person.cpp \
    signup.cpp \
    todolist.cpp

HEADERS += \
    firstpage.h \
    login.h \
    person.h \
    signup.h \
    todolist.h

FORMS += \
    firstpage.ui \
    login.ui \
    signup.ui \
    todolist.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
