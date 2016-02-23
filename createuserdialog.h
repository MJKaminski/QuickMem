/*
 * QuickMEM - program wspomagajacy nauke
 * Autorzy: Adrian Stachlewski, Michal Jerzy Kaminski
*/
#ifndef CREATEUSERDIALOG_H
#define CREATEUSERDIALOG_H

#include <QDialog>
#include "courses.h"

namespace Ui {
class CreateUserDialog;
}

/** Dialog zwiazany z tworzeniem nowego uzytkownika
 *  \author Michal Kaminski
 */
class CreateUserDialog : public QDialog
{
    Q_OBJECT

public:
    /** CreateUserDialog potrzebuje wskaznika na liste kursow oraz zmienna do zapisu nazwy uzytkownika*/
    explicit CreateUserDialog(QWidget *parent, QString* s, QStringList* cList);
    ~CreateUserDialog();

private slots:

    /** Zamyka dialog z sygnalem anuluj*/
    void on_rejectButton_clicked();

    /** Zamyka dialog z sygnalem akceptacji
     * nazwa nowego uzytkownika znajduje sie po adresem userName*/
    void on_acceptButton_clicked();

private:
    Ui::CreateUserDialog *ui_; /**< Wskaznik na obiekt do zarzadzania elementami GUI*/
    QString* userName_;  /**< Wskaznik zmienna, ktorej zostanie zapisana nazwa nowego uzytkownika*/
    QStringList* courseList_; /**< Wskaznik liste kursow*/
};

#endif // CREATEUSERDIALOG_H
