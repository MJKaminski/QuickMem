/*
 * QuickMEM - program wspomagajacy nauke
 * Autorzy: Adrian Stachlewski, Michal Jerzy Kaminski
*/

#include "mainwindow.h"
#include "database.h"
#include <QApplication>
#include "users.h"
#include "courses.h"
#include "questions.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Database database;
    Users users(&database);
    Courses courses(&database);
    Questions questions(&database);
    MainWindow w(&courses, &users, &questions);

    w.show();

    return a.exec();
}
