#-------------------------------------------------
#
# Project created by QtCreator 2015-04-11T20:38:21
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
TARGET = QuickMEM
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    database.cpp \
    users.cpp \
    question.cpp \
    createuserdialog.cpp \
    courses.cpp \
    questions.cpp \
    addcardsdialog.cpp \
    userscoursesdialog.cpp \
    startcoursedialog.cpp

HEADERS  += mainwindow.h \
    database.h \
    users.h \
    question.h \
    createuserdialog.h \
    courses.h \
    questions.h \
    addcardsdialog.h \
    userscoursesdialog.h \
    startcoursedialog.h

FORMS    += mainwindow.ui \
    createuserdialog.ui \
    addcardsdialog.ui \
    userscoursesdialog.ui \
    startcoursedialog.ui

RESOURCES += \
    resources.qrc
