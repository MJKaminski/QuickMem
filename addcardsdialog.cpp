/*
 * QuickMEM - program wspomagajacy nauke
 * Autorzy: Adrian Stachlewski, Michal Jerzy Kaminski
*/
#include "addcardsdialog.h"
#include "ui_addcardsdialog.h"
#include <QStringListModel>
#include <QMessageBox>

/** AddCardsDialog potrzebuje wskazniki na zarzadcow kursow i pytan*/
AddCardsDialog::AddCardsDialog(QWidget *parent, Courses* cman, Questions *qman, QString default_course) :
    QDialog(parent),
    ui_(new Ui::AddCardsDialog), questionsManager_(qman), coursesManager_(cman)
{
    ui_->setupUi(this);
    ui_->comboBox->addItems(cman->getCoursesForCurrentUser());
    if(default_course != "")
     ui_->comboBox->setCurrentText(default_course);
    bar_ = new QStatusBar(this);
    ui_->dialogLayout->addWidget(bar_);
    bar_->setSizeGripEnabled(false);
}

AddCardsDialog::~AddCardsDialog()
{
    delete ui_;
}

/** Zamyka dialog*/
void AddCardsDialog::on_closeButton_clicked()
{
    close();
}

/** Dodaje karte do wybranego kursu*/
void AddCardsDialog::on_addCardButton_clicked()
{
    QString course_name = ui_->comboBox->currentText();
    QString question = ui_->questionEdit->toPlainText().trimmed();
    QString answer = ui_->answerEdit->toPlainText().trimmed();
    if(question == "" || answer == "")
        QMessageBox::critical(this,"Błąd","Proszę wypełnić oba pola!");
    else
    {
        int course_id = coursesManager_->getID(course_name);
        questionsManager_->addQuestion(question,answer,course_id);
        ui_->questionEdit->setPlainText("");
        ui_->answerEdit->setPlainText("");
        bar_->showMessage("Dodano karte", 2000);
    }
}
