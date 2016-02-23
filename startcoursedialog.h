/*
 * QuickMEM - program wspomagajacy nauke
 * Autorzy: Adrian Stachlewski, Michal Jerzy Kaminski
*/
#ifndef STARTCOURSEDIALOG_H
#define STARTCOURSEDIALOG_H

#include <QDialog>
#include "courses.h"

namespace Ui {
class StartCourseDialog;
}

/** Dialog do wyboru kursu do nauki
 *  \author Michal Kaminski
 */
class StartCourseDialog : public QDialog
{
    Q_OBJECT

public:
    /** StarCourseDialog potrzebuje wskaznik na zarzadce kursow*/
    explicit StartCourseDialog(QWidget *parent, Courses* cman);
    ~StartCourseDialog();

private slots:
    /** Aktuwuje przycisk wyboru kursu*/
    void on_listWidget_clicked();

    /** Ustawia aktywny kurs do nauki i zamyka dialog z sygnalem akceptacji*/
    void on_startCourseButton_clicked();

private:
    Ui::StartCourseDialog *ui_; /**< Wskaznik na obiekt do zarzadzania elementami GUI*/
    Courses* coursesManager_; /**< Wskaznik na zarzadce kursow*/
};

#endif // STARTCOURSEDIALOG_H
