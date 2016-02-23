/*
 * QuickMEM - program wspomagajacy nauke
 * Autorzy: Adrian Stachlewski, Michal Jerzy Kaminski
*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDateTime>
#include <QSignalMapper>
#include <QStringListModel>
#include <vector>
#include "question.h"
#include "courses.h"
#include "users.h"
#include "questions.h"


namespace Ui
{
    class MainWindow;
}


/** Glowne okno GUI
 *  \author Michal Kaminski
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    /** Konstruktor klasy przymujacy wskazniki
    * na kontrolerow kursow, pytan i uzytkownikow
    */
    explicit MainWindow( Courses *courses, Users *users, Questions *questions);
    ~MainWindow();

private slots:

    /** Ocen pytanie w skali od 0 do 5 i powiadom kontroler pytan*/
    void gradeButtonClicked(int number);

    /** Pokaz odpowiedz na pytanie*/
    void on_showAnswerButton_clicked();

    /** Wybrano uzytkownika z listy - mozna aktywowac przyciski wybierz/usun uzytkownika*/
    void on_userListView_clicked();

    /** Zmienia widok na ekran startowy uzytkownika*/
    void on_userPageButton_clicked();

    /** Wyswietla dialog CreateUser*/
    void on_addUserButton_clicked();

    /** Usuwa wybranego uzytkownika*/
    void on_deleteUserButton_clicked();

    /** Zamyka program*/
    void on_actionQuit_triggered();

    /** Wyswietla informacje o Qt*/
    void on_actionAboutQt_triggered();

    /** Wyswietla dialog AddCards*/
    void on_addCardsDialogButton_clicked();

    /** Wylogowuje uzytkownika i wraca do ekranu wyboru*/
    void on_logOffButton_clicked();

    /** Wyswietla dialog wyboru kursu i przy akceptacji uruchamia kurs*/
    void on_courseStartButton_clicked();

    /** Wyswietla dialog do zarzadzania kursami uzytkownika*/
    void on_usersCoursesButton_clicked();

    /** Wyswietla edytor kursow*/
    void on_courseEditorButton_clicked();

    /** Zmienia aktywny kurs przeznaczony do edycji*/
    void on_coursesComboBox_currentIndexChanged(const QString &course_name);

    /** Powraca do ekranu uzytkownika*/
    void on_returnToUserPageButton_clicked();

    /** Zmienia aktualnie edytowana karte*/
    void on_cardsComboBox_currentIndexChanged(int index);

    /** Usuwa wybrana karte*/
    void on_deleteCardButton_clicked();

    /** Wyswietla dialog AddCards i aktualizuje ekran edytora kursow*/
    void on_addCardsButton2_clicked();

    /** Zapisuje zmodyfikowana karte w bazie danych*/
    void on_updateQuestionButton_clicked();

    /** Wyswietla informacje o autorach*/
    void on_actionAuthors_triggered();

    /** Konczy nauke i wraca do ekranu uzytkownika*/
    void on_endCourseButton_clicked();

    /** Zmienia widok na ekran uzytkownika*/
    void on_actionUserPage_triggered();

    /** Zmienia widok na ekran wyboru uzytkownika*/
    void on_actionChangeUser_triggered();

    /** Zmienia widok na ekran edytora kursow*/
    void on_actionCourseEditor_triggered();

    /** Pozwala na usuniecie wybranego kursu z bazy danych*/
    void on_actionDeleteCourse_triggered();

private:
    /** Typ wyliczeniowy reprezentujacy widok okna*/
    enum View
    {
        USER_SELECT = 0, /**< ekran wyboru uzytkownika*/
        USER_PAGE = 1,/**< ekran glowny uzytkownika*/
        COURSE_EDITOR = 2, /**< ekran edytora kursu*/
        COURSE = 3 /**< ekran edytora kursu*/
    };

    /** Typ wyliczeniowy reprezentujacy widok przyciskow w trakcie odbywania kursu*/
    enum AnswerButtons
    {
        SHOW_ANSWER = 0 ,/**< Okno czeka na prosbe o pokazanie odpowiedzi*/
        MARKS = 1 /**< Okno czeka na ocene karty*/
    };

    Ui::MainWindow *ui_; /**< Wskaznik na obiekt do zarzadzania elementami GUI*/
    QSignalMapper* signalMapper_; /**< Mapa sygnalow uzywana przez przyciski oceny karty*/
    QStringListModel* userListModel_; /**< Wskaznik na model listy uzytkownikow*/
    View currView_; /**< Przechowuje informacje o aktualnym widoku*/

    Courses* const coursesManager_; /**< Wskaznik na zarzadce kursow*/
    Users* const usersManager_; /**< Wskaznik na zarzadce uzytkownikow*/
    Questions* const questionManager_; /**< Wskaznik na zarzadce pytan*/

    std::vector<Question> questionsToEdit_; /**< wektor pytan edytowanego kursu*/


    void setView(View v); /**< Funkcja zmieniajaca aktywny widok okna*/
    void setUserList(QStringList l); /**< Funkcja ustawiajaca liste uzytkownikow*/
    void updateUserStatistics(); /**< Funkcja aktualizujaca statystyki aktywnego uzytkownika*/


};

#endif // MAINWINDOW_H
