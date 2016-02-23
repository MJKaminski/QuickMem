/*
 * QuickMEM - program wspomagajacy nauke
 * Autorzy: Adrian Stachlewski, Michal Jerzy Kaminski
*/
#include "startcoursedialog.h"
#include "ui_startcoursedialog.h"

/** StarCourseDialog potrzebuje wskaznik na zarzadce kursow*/
StartCourseDialog::StartCourseDialog(QWidget *parent, Courses *cman):
    QDialog(parent),
    ui_(new Ui::StartCourseDialog), coursesManager_(cman)
{
    ui_->setupUi(this);
    QStringList courses = coursesManager_->getCoursesForCurrentUser();

    for(auto it = courses.begin(); it!= courses.end(); ++it)
    {
        QString course_name = *it;
        int id = coursesManager_->getID(course_name);
        int count = coursesManager_->countReadyQuestions(id);
        if(count > 0 )
            ui_->listWidget->addItem(course_name);
    }

    ui_->startCourseButton->setDisabled(true);
}

StartCourseDialog::~StartCourseDialog()
{
    delete ui_;
}

/** Aktuwuje przycisk wyboru kursu*/
void StartCourseDialog::on_listWidget_clicked()
{
    ui_->startCourseButton->setDisabled(false);
}

/** Ustawia aktywny kurs do nauki i zamyka dialog z sygnalem akceptacji*/
void StartCourseDialog::on_startCourseButton_clicked()
{
    QListWidgetItem* item = ui_->listWidget->currentItem();
    QString course_name = item->text();
    coursesManager_->set(course_name);
    accept();
}
