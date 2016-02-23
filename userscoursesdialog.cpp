/*
 * QuickMEM - program wspomagajacy nauke
 * Autorzy: Adrian Stachlewski, Michal Jerzy Kaminski
*/
#include "userscoursesdialog.h"
#include "ui_userscoursesdialog.h"
#include <QInputDialog>
#include <QMessageBox>

/** UsersCoursesDialog potrzebuje wskaznik na zarzadce kursow*/
UsersCoursesDialog::UsersCoursesDialog(QWidget *parent, Courses *crs) :
    QDialog(parent),
    ui_(new Ui::UsersCoursesDialog), coursesManager_(crs)
{
    ui_->setupUi(this);
    ui_->listWidget->addItems(coursesManager_->getCoursesForCurrentUser());
    ui_->unsubscibeButton->setDisabled(true);
}

UsersCoursesDialog::~UsersCoursesDialog()
{
    delete ui_;
}

/** UsersCoursesDialog potrzebuje wskaznik na zarzadce kursow*/
void UsersCoursesDialog::on_createCourseButton_clicked()
{
    QString course_name;
    QInputDialog i(this);
    i.setInputMode(QInputDialog::TextInput);
    i.setWindowTitle("Nowy Kurs");
    i.setLabelText("Podaj nazwę kursu");
    i.setCancelButtonText("Anuluj");
    if(i.exec() == QDialog::Accepted)
    {
        course_name = i.textValue();
        if(coursesManager_->add(course_name))
        {
            coursesManager_->subscribeCourseForCurrentUser(course_name);
            ui_->listWidget->addItem(course_name);
        }

        else
        {
            QMessageBox::critical(this,"Błąd","Kurs o podanej nazwie już istnieje!");
        }

    }
}

/** Pozwala na zapisanie sie na jeden z istniejacych kursow*/
void UsersCoursesDialog::on_addExistingButton_clicked()
{
    QStringList unsubsribed_courses = coursesManager_->getUnsubcribeCoursesForCurrentUser();
    if(unsubsribed_courses.count() !=0 )
    {
        QInputDialog i(this);
        i.setInputMode(QInputDialog::TextInput);
        i.setWindowTitle("Dodaj Istniejący Kurs");
        i.setComboBoxItems(unsubsribed_courses);
        i.setComboBoxEditable(false);
        i.setLabelText("Podaj nazwę kursu");
        i.setCancelButtonText("Anuluj");
        if(i.exec() == QDialog::Accepted)
        {
            coursesManager_->subscribeCourseForCurrentUser(i.textValue());
            ui_->listWidget->addItem(i.textValue());
        }
    }
    else
        QMessageBox::information(this,"Uwaga", "Nie znaleziono żadnych nieprzypisanych kursów");

}

/** Wypisuje uzytkownika z wybranego kursu*/
void UsersCoursesDialog::on_unsubscibeButton_clicked()
{
    QListWidgetItem* item = ui_->listWidget->currentItem();
    QString course_name = item->text();
    ui_->listWidget->removeItemWidget(item);
    coursesManager_->unsubscribeCurrentUserFromCourse(course_name);
    delete item;

}

/** Zamyka dialog*/
void UsersCoursesDialog::on_closeButton_clicked()
{
    close();
}

/** Po wybraniu kursu aktywuje przycisk wypisz sie*/
void UsersCoursesDialog::on_listWidget_clicked()
{
    ui_->unsubscibeButton->setDisabled(false);
}
