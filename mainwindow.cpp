/*
 * QuickMEM - program wspomagajacy nauke
 * Autorzy: Adrian Stachlewski, Michal Jerzy Kaminski
*/


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "createuserdialog.h"
#include "addcardsdialog.h"
#include "userscoursesdialog.h"
#include "startcoursedialog.h"
#include <QInputDialog>

/** Konstruktor klasy przymujacy wskazniki
* na kontrolerow kursow, pytan i uzytkownikow
*/
MainWindow::MainWindow(Courses* courses, Users* users,  Questions* questions)
    : QMainWindow(nullptr), ui_(new Ui::MainWindow), coursesManager_(courses), usersManager_(users), questionManager_(questions)
{
    ui_->setupUi(this);
    setCentralWidget(ui_->viewStack);
    setWindowTitle("QuickMem");


    signalMapper_ = new QSignalMapper(this); //podlaczanie przyciskow oceny karty
    connect(ui_->grade0Button,SIGNAL(clicked()), signalMapper_, SLOT(map()));
    connect(ui_->grade1Button,SIGNAL(clicked()), signalMapper_, SLOT(map()));
    connect(ui_->grade2Button,SIGNAL(clicked()), signalMapper_, SLOT(map()));
    connect(ui_->grade3Button,SIGNAL(clicked()), signalMapper_, SLOT(map()));
    connect(ui_->grade4Button,SIGNAL(clicked()), signalMapper_, SLOT(map()));
    connect(ui_->grade5Button,SIGNAL(clicked()), signalMapper_, SLOT(map()));

    signalMapper_->setMapping(ui_->grade0Button, 0);
    signalMapper_->setMapping(ui_->grade1Button, 1);
    signalMapper_->setMapping(ui_->grade2Button, 2);
    signalMapper_->setMapping(ui_->grade3Button, 3);
    signalMapper_->setMapping(ui_->grade4Button, 4);
    signalMapper_->setMapping(ui_->grade5Button, 5);

    connect(signalMapper_, SIGNAL(mapped(int)),this, SLOT(gradeButtonClicked(int)));




    userListModel_ = new QStringListModel(this);
    userListModel_->setStringList(users->getUsers());

    ui_->userListView->setModel(userListModel_); //nastawianie modeli i przyciskow
    ui_->userPageButton->setDisabled(true);
    ui_->deleteUserButton->setDisabled(true);
    ui_->actionChangeUser->setEnabled(false);
    ui_->actionCourseEditor->setEnabled(false);
    ui_->actionUserPage->setEnabled(false);
    ui_->actionDeleteCourse->setEnabled(false);
    ui_->answerText->setVisible(false);
    ui_->buttonStack->setCurrentIndex(SHOW_ANSWER);

    setView(USER_SELECT);
}

MainWindow::~MainWindow()
{
    delete ui_;
}

/** Funkcja zmieniajaca aktywny widok okna*/
void MainWindow::setView(View v)
{
    currView_ = v;
    ui_->viewStack->setCurrentIndex(v);
    if(currView_ == USER_PAGE)
    {
        ui_->actionChangeUser->setEnabled(true);
        ui_->actionCourseEditor->setEnabled(true);
        ui_->actionUserPage->setEnabled(true);
        ui_->actionDeleteCourse->setEnabled(true);
    }

    else if(currView_ == USER_SELECT)
    {
        ui_->actionChangeUser->setEnabled(false);
        ui_->actionCourseEditor->setEnabled(false);
        ui_->actionUserPage->setEnabled(false);
        ui_->actionDeleteCourse->setEnabled(false);
    }

    else if(currView_ == COURSE_EDITOR)
    {
        ui_->actionChangeUser->setEnabled(true);
        ui_->actionCourseEditor->setEnabled(true);
        ui_->actionUserPage->setEnabled(true);
        ui_->actionDeleteCourse->setEnabled(false);
    }

    else
    {
        ui_->actionChangeUser->setEnabled(true);
        ui_->actionCourseEditor->setEnabled(true);
        ui_->actionUserPage->setEnabled(true);
    }
}

/** Funkcja ustawiajaca liste uzytkownikow*/
void MainWindow::setUserList(QStringList l)
{
    userListModel_->setStringList(l);
}

/** Funkcja aktualizujaca statystyki aktywnego uzytkownika*/
void MainWindow::updateUserStatistics()
{
    int course_number = coursesManager_->countSubscribedCourses();
    int ready_courses =  coursesManager_->countCoursesWithReadyQuestion();
    ui_->noOfCoursesLabel->setText(QString::number(course_number));
    ui_->noOfActiveCourses->setText(QString::number(ready_courses));
    if(course_number == 0)
    {
        ui_->addCardsDialogButton->setDisabled(true);
        ui_->courseStartButton->setDisabled(true);
    }
    else if(ready_courses == 0)
    {
        ui_->addCardsDialogButton->setDisabled(false);
        ui_->courseStartButton->setDisabled(true);
    }
    else
    {
        ui_->addCardsDialogButton->setDisabled(false);
        ui_->courseStartButton->setDisabled(false);
    }
}

/** Ocen pytanie w skali od 0 do 5 i powiadom kontroler pytan*/
void MainWindow::gradeButtonClicked(int number)
{
    questionManager_->rateCurrentQuestion(number);
    Question current_quest = questionManager_->getQuestion();
    ui_->questionText->setPlainText(current_quest.question);
    ui_->answerText->setPlainText(current_quest.answer);
    ui_->answerText->setVisible(false);
    ui_->buttonStack->setCurrentIndex(SHOW_ANSWER);

    if(current_quest.id ==0)
        ui_->showAnswerButton->setDisabled(true);
    else
        ui_->showAnswerButton->setDisabled(false);
}

/** Pokaz odpowiedz na pytanie*/
void MainWindow::on_showAnswerButton_clicked()
{
    ui_->buttonStack->setCurrentIndex(MARKS);
    ui_->answerText->setVisible(true);
}

/** Wybrano uzytkownika z listy - mozna aktywowac przyciski wybierz/usun uzytkownika*/
void MainWindow::on_userListView_clicked()
{
    ui_->userPageButton->setDisabled(false);
    ui_->deleteUserButton->setDisabled(false);
}

/** Zmienia widok na ekran startowy uzytkownika*/
void MainWindow::on_userPageButton_clicked()
{
  QVariant s = userListModel_->data(ui_->userListView->currentIndex(),0);
  QString user_name = s.toString();

  usersManager_->set(user_name);
  ui_->userNameLabel->setText(user_name);

  updateUserStatistics();
  setView(USER_PAGE);
}

/** Wyswietla dialog CreateUser*/
void MainWindow::on_addUserButton_clicked()
{
    QString user_name;
    QStringList courses_list = coursesManager_->getCourses();
    CreateUserDialog a(this, &user_name, &courses_list);
    if(a.exec() == QDialog::Accepted)
    {
       if(usersManager_->add(user_name))
       {
           setUserList(usersManager_->getUsers());
           ui_->statusBar->showMessage("Dodano Użytkownika", 5000);
           ui_->userPageButton->setDisabled(true);
           ui_->deleteUserButton->setDisabled(true);

           int id = usersManager_->getID(user_name);

           for(auto i = courses_list.begin(); i!= courses_list.end(); ++i)
               coursesManager_->subscribeCourseForUser(id, *i);
       }
       else
       {
             QMessageBox::critical(this,"Błąd","Użytkownik już istnieje!");
       }
    }
}

/** Usuwa wybranego uzytkownika*/
void MainWindow::on_deleteUserButton_clicked()
{
    QMessageBox warning(this);
    warning.setWindowTitle("Ostrzeżenie");
    warning.setIcon(QMessageBox::Warning);
    warning.setText("Czy na pewno chcesz usunąć użytkownika?");
    QAbstractButton *accept = warning.addButton("OK", QMessageBox::YesRole);
    warning.addButton("Anuluj", QMessageBox::NoRole);
    warning.exec();

    if (warning.clickedButton() == accept)
    {
        QVariant s = userListModel_->data(ui_->userListView->currentIndex(),0);
        if(usersManager_->erase(s.toString()))
        {
            ui_->statusBar->showMessage("Usunięto Użytkownika", 5000);
            setUserList(usersManager_->getUsers());
            ui_->userPageButton->setDisabled(true);
            ui_->deleteUserButton->setDisabled(true);
        }
    }
}

/** Zamyka program*/
void MainWindow::on_actionQuit_triggered()
{
   close();
}

/** Wyswietla informacje o Qt*/
void MainWindow::on_actionAboutQt_triggered()
{
    QMessageBox::aboutQt(this,"O Qt");
}

/** Wyswietla dialog AddCards*/
void MainWindow::on_addCardsDialogButton_clicked()
{
    AddCardsDialog dialog(this, coursesManager_, questionManager_);
    dialog.exec();
    updateUserStatistics();
}

/** Wylogowuje uzytkownika i wraca do ekranu wyboru*/
void MainWindow::on_logOffButton_clicked()
{
    setView(USER_SELECT);
}

/** Wyswietla dialog wyboru kursu i przy akceptacji uruchamia kurs*/
void MainWindow::on_courseStartButton_clicked()
{
    StartCourseDialog a(this, coursesManager_);
    if(a.exec() == QDialog::Accepted)
    {
        setView(COURSE);
        Question current_quest = questionManager_->getQuestion();
        ui_->questionText->setPlainText(current_quest.question);
        ui_->answerText->setPlainText(current_quest.answer);
        ui_->answerText->setVisible(false);
        ui_->buttonStack->setCurrentIndex(SHOW_ANSWER);
        if(current_quest.id ==0)
            ui_->showAnswerButton->setDisabled(true);
        else
            ui_->showAnswerButton->setDisabled(false);

    }
}

/** Wyswietla dialog do zarzadzania kursami uzytkownika*/
void MainWindow::on_usersCoursesButton_clicked()
{
    UsersCoursesDialog dialog(this, coursesManager_);
    dialog.exec();
    updateUserStatistics();
}

/** Wyswietla edytor kursow*/
void MainWindow::on_courseEditorButton_clicked()
{

    setView(COURSE_EDITOR);
    ui_->coursesComboBox->clear();
    ui_->coursesComboBox->addItems(coursesManager_->getCoursesForCurrentUser());
    if(ui_->coursesComboBox->count() == 0)
    {
        ui_->addCardsButton2->setDisabled(true);
        ui_->answerEdit->setDisabled(true);
        ui_->questionEdit->setDisabled(true);
        ui_->deleteCardButton->setDisabled(true);
        ui_->updateQuestionButton->setDisabled(true);
        ui_->questionEdit->setPlainText("BRAK KURSÓW");
        ui_->answerEdit->setPlainText("");
    }
    else
    {
        ui_->addCardsButton2->setDisabled(false);
    }
}

/** Zmienia aktywny kurs przeznaczony do edycji*/
void MainWindow::on_coursesComboBox_currentIndexChanged(const QString &course_name)
{
    QStringList cards;
    int id = coursesManager_->getID(course_name);
    questionsToEdit_ = questionManager_->getAllQuestionsFromCourse(id);

    int j = 1;
    for(auto i = questionsToEdit_.begin(); i!= questionsToEdit_.end(); ++i, ++j)
        cards << (*i).question;

    ui_->cardsComboBox->clear();
    ui_->cardsComboBox->addItems(cards);

    if(ui_->cardsComboBox->count() == 0)
    {

        ui_->questionEdit->setPlainText("BRAK PYTAŃ");
        ui_->answerEdit->setPlainText("");
        ui_->answerEdit->setDisabled(true);
        ui_->questionEdit->setDisabled(true);
        ui_->deleteCardButton->setDisabled(true);
        ui_->updateQuestionButton->setDisabled(true);
    }
    else
    {

        ui_->answerEdit->setDisabled(false);
        ui_->questionEdit->setDisabled(false);
        ui_->deleteCardButton->setDisabled(false);
        ui_->updateQuestionButton->setDisabled(false);
        ui_->questionEdit->setPlainText(questionsToEdit_[0].question);
        ui_->answerEdit->setPlainText(questionsToEdit_[0].answer);
    }
}

/** Powraca do ekranu uzytkownika*/
void MainWindow::on_returnToUserPageButton_clicked()
{
    setView(USER_PAGE);
    updateUserStatistics();
}

/** Zmienia aktualnie edytowana karte*/
void MainWindow::on_cardsComboBox_currentIndexChanged(int index)
{
    if(index != -1)
    {
        ui_->questionEdit->setPlainText(questionsToEdit_[index].question);
        ui_->answerEdit->setPlainText(questionsToEdit_[index].answer);
    }
    else
    {
        ui_->answerEdit->setDisabled(true);
        ui_->questionEdit->setDisabled(true);
        ui_->deleteCardButton->setDisabled(true);
        ui_->updateQuestionButton->setDisabled(true);
        ui_->questionEdit->setPlainText("BRAK PYTAŃ");
        ui_->answerEdit->setPlainText("");
    }
}

/** Usuwa wybrana karte*/
void MainWindow::on_deleteCardButton_clicked()
{
    QMessageBox warning(this);
    warning.setWindowTitle("Ostrzeżenie");
    warning.setIcon(QMessageBox::Warning);
    warning.setText("Czy na pewno chcesz usunąć tę kartę?");
    QAbstractButton *accept = warning.addButton("OK", QMessageBox::YesRole);
    warning.addButton("Anuluj", QMessageBox::NoRole);
    warning.exec();

    if (warning.clickedButton() == accept)
    {
        int index = ui_->cardsComboBox->currentIndex();
        int id = questionsToEdit_[index].id;
        questionManager_->eraseQuestion(id);
        for(auto it = questionsToEdit_.begin(); it != questionsToEdit_.end(); ++it)
            if((*it).id == id)
            {
                questionsToEdit_.erase(it);
                break;
            }

        ui_->cardsComboBox->removeItem(index);
        if(ui_->cardsComboBox->count() > 0)
            ui_->cardsComboBox->setCurrentIndex(0);
    }
}

/** Wyswietla dialog AddCards i aktualizuje ekran edytora kursow*/
void MainWindow::on_addCardsButton2_clicked()
{
    QString course_name = ui_->coursesComboBox->currentText();
    AddCardsDialog dialog(this, coursesManager_, questionManager_, course_name);
    dialog.exec();
    QStringList cards;
    int id = coursesManager_->getID(course_name);
    questionsToEdit_ = questionManager_->getAllQuestionsFromCourse(id);

    int j = 1;
    for(auto i = questionsToEdit_.begin(); i!= questionsToEdit_.end(); ++i, ++j)
        cards << (*i).question;

    ui_->cardsComboBox->clear();
    ui_->cardsComboBox->addItems(cards);

    if(ui_->cardsComboBox->count() == 0)
    {

        ui_->questionEdit->setPlainText("BRAK PYTAŃ");
        ui_->answerEdit->setPlainText("");
        ui_->answerEdit->setDisabled(true);
        ui_->questionEdit->setDisabled(true);
        ui_->deleteCardButton->setDisabled(true);
        ui_->updateQuestionButton->setDisabled(true);
    }
    else
    {

        ui_->answerEdit->setDisabled(false);
        ui_->questionEdit->setDisabled(false);
        ui_->deleteCardButton->setDisabled(false);
        ui_->updateQuestionButton->setDisabled(false);
        ui_->questionEdit->setPlainText(questionsToEdit_[0].question);
        ui_->answerEdit->setPlainText(questionsToEdit_[0].answer);
    }
}

/** Zapisuje zmodyfikowana karte w bazie danych*/
void MainWindow::on_updateQuestionButton_clicked()
{
    int index = ui_->cardsComboBox->currentIndex();
    QString quest = ui_->questionEdit->toPlainText().trimmed();
    QString ans = ui_->answerEdit->toPlainText().trimmed();
    int id = questionsToEdit_[index].id;

    if(quest== "" || ans == "")
        QMessageBox::critical(this,"Błąd","Proszę wypełnić oba pola!");

    else
    {
        if(questionManager_->editQuestion(id,quest,ans))
        {
            ui_->statusBar->showMessage("Zmodyfikowano pytanie", 5000);
            ui_->cardsComboBox->setItemText(index,quest);
            questionsToEdit_[index].question = quest;
            questionsToEdit_[index].answer = ans;
        }

    }
}

/** Wyswietla informacje o autorach*/
void MainWindow::on_actionAuthors_triggered()
{
    QMessageBox::about(this, "Autorzy", "<p align='center'><b>QuickMem</b> <br><br> "
                                        "Adrian Stachlewski,"
                                        " Michał Kamiński"
                                        "<br><br> Warszawa, 2015</p>");
}

/** Konczy nauke i wraca do ekranu uzytkownika*/
void MainWindow::on_endCourseButton_clicked()
{
    updateUserStatistics();
    setView(USER_PAGE);
}

/** Zmienia widok na ekran uzytkownika*/
void MainWindow::on_actionUserPage_triggered()
{
    updateUserStatistics();
    setView(USER_PAGE);
}

/** Zmienia widok na ekran wyboru uzytkownika*/
void MainWindow::on_actionChangeUser_triggered()
{
    setView(USER_SELECT);
}

/** Zmienia widok na ekran edytora kursow*/
void MainWindow::on_actionCourseEditor_triggered()
{
    on_courseEditorButton_clicked();
}

/** Pozwala na usuniecie wybranego kursu z bazy danych*/
void MainWindow::on_actionDeleteCourse_triggered()
{
    QInputDialog i(this);
    i.setInputMode(QInputDialog::TextInput);
    i.setWindowTitle("Usuń kurs");
    i.setComboBoxItems(coursesManager_->getCourses());
    i.setComboBoxEditable(false);
    i.setLabelText("Wybierz kurs do usuniecia:");
    i.setCancelButtonText("Anuluj");
    if(i.exec() == QDialog::Accepted)
    {
        coursesManager_->erase(i.textValue());
        updateUserStatistics();
    }
}
