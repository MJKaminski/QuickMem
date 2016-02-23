/*
 * QuickMEM - program wspomagajacy nauke
 * Autorzy: Adrian Stachlewski, Michal Jerzy Kaminski
*/
#ifndef USERSCOURSESDIALOG_H
#define USERSCOURSESDIALOG_H

#include <QDialog>
#include "courses.h"

namespace Ui {
class UsersCoursesDialog;
}

/** Dialog zwiazany z zarzadzaniem kursami aktywnego uzytkownika
 *  \author Michal Kaminski
 */
class UsersCoursesDialog : public QDialog
{
    Q_OBJECT

public:
    /** UsersCoursesDialog potrzebuje wskaznik na zarzadce kursow*/
    explicit UsersCoursesDialog(QWidget *parent, Courses *crs);
    ~UsersCoursesDialog();

private slots:
    /** Pozwala na stworzenie nowego kursu*/
    void on_createCourseButton_clicked();

    /** Pozwala na zapisanie sie na jeden z istniejacych kursow*/
    void on_addExistingButton_clicked();

    /** Wypisuje uzytkownika z wybranego kursu*/
    void on_unsubscibeButton_clicked();

    /** Zamyka dialog*/
    void on_closeButton_clicked();

    /** Po wybraniu kursu aktywuje przycisk wypisz sie*/
    void on_listWidget_clicked();

private:
    Ui::UsersCoursesDialog *ui_; /**< Wskaznik na obiekt do zarzadzania elementami GUI*/
    Courses* coursesManager_; /**< Wskaznik na zarzadce kursow*/
};

#endif // USERSCOURSESDIALOG_H
