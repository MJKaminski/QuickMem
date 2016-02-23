/*
 * QuickMEM - program wspomagajacy nauke
 * Autorzy: Adrian Stachlewski, Michal Jerzy Kaminski
*/
#ifndef ADDCARDSDIALOG_H
#define ADDCARDSDIALOG_H

#include <QDialog>
#include <QStatusBar>
#include "courses.h"
#include "questions.h"

namespace Ui {
class AddCardsDialog;
}

/** Dialog zwiazany z dodawaniem kart
 *  \author Michal Kaminski
 */
class AddCardsDialog : public QDialog
{
    Q_OBJECT

public:
    /** AddCardsDialog potrzebuje wskazniki na zarzadcow kursow i pytan*/
    explicit AddCardsDialog(QWidget *parent, Courses *cman, Questions* qman, QString default_course = "");
    ~AddCardsDialog();

private slots:
    /** Zamyka dialog*/
    void on_closeButton_clicked();

    /** Dodaje karte do wybranego kursu*/
    void on_addCardButton_clicked();

private:
    Ui::AddCardsDialog *ui_; /**< Wskaznik na obiekt do zarzadzania elementami GUI*/
    Questions* const questionsManager_; /**< Wskaznik na zarzedce pytan*/
    Courses* const coursesManager_; /**< Wskaznik na zarzadce kursow*/
    QStatusBar* bar_; /**< Pasek Stanu Dialogu*/
};

#endif // ADDCARDSDIALOG_H
