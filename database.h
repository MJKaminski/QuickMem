/*
 * QuickMEM - program wspomagajacy nauke
 * Autorzy: Adrian Stachlewski, Michal Jerzy Kaminski
*/

#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QString>
#include <QStringList>
#include <QSqlTableModel>
#include <QDate>
#include <vector>
#include <queue>
#include <map>
#include "question.h"

/** Klasa zapewniajaca polaczenie z baza danych SQLite
 *  Zalecane jest uzywanie klas posredniczacych Users, Courses oraz Questions, ktore zmniejszaja liczbe zapytan do bazy danych
 *  oraz przyspieszaja dzialanie programu.
 *  \author Adrian Stachlewski
 */
class Database
{
    typedef std::map<QString, int> UsersList;
    typedef std::map<QString, int> CoursesList;
    typedef std::vector<Question> QuestionsList;
    typedef std::tuple<double, int, unsigned long long, int> MarkParameters;

    friend class Users;
    friend class Courses;
    friend class Questions;

    bool validError_; /**< Flaga informujaca o aktualnosci bledu */
    QString error_; /**< Informacja o bledzie */
    QSqlDatabase db_; /**< Odnosnik do bazy danych */

    int currentUserId_; /**< Aktualny uzytkownik */
    int currentCourseId_; /**< Aktualny kurs */
    bool currentUserChanged_; /**< Informacja, czy po pobraniu danych zmieniono uzytkownika */
    bool currentCourseChanged_; /**< Informacja, czy po pobraniu danych zmieniono kurs */

public:
    Database();
    ~Database();

    QString getLastError(); /**< Pobiera informacje o bledzie */
    bool validError(); /**< Sprawdza czy blad nie zostal jeszcze pobrany */

    UsersList getUsers(); /**< Pobiera liste uzytkownikow */
    bool addUser(QString name); /**< Dodaje uzytkownika do bazy */
    bool deleteUser(int id_user); /**< Usuwa uzytkownika z bazy */
    void setUser(int id_user); /**< Ustawia aktualnego uzytkownika */

    CoursesList getCourses(bool for_current_user = false); /**< Pobiera liste kursow */
    bool addCourse(QString name); /**< Dodaje kurs do bazy danych */
    bool deleteCourse(int id_course); /**< Usuwa kurs z bazy danych */
    void setCourse(int id_course); /**< Ustawia aktualny kurs */
    bool addUserToCourse(int id_course, int id_user = 0); /**< Dodaje uzytkownika do kursu */
    bool deleteUserFromCourse(int id_course, int id_user = 0); /**< Usuwa uzytkownika z kursu */
    bool currentUserChanged(); /**< Sprawdza, czy aktualny uzytkownik zmienil sie od ostaniego pobrania danych */
    bool currentCourseChanged(); /**< Sprawdza, czy aktualny kurs zmienil sie od ostatniego pobrania danych */

    QuestionsList getQuestions(int id_course = 0, bool only_ready = true, int id_user = 0); /**< Zwraca liste pytan */
    bool addQuestion(int id_course, QString question, QString answer); /**< Dodaje pytanie do bazy */
    bool deleteQuestion(int id_question); /**< Usuwa pytanie z bazy danych */
    bool editQuestion(int id_question, QString question, QString answer); /**< Edytuje pytanie w bazie danych */

    int countSubscribedCourses(); /**< Zlicza kursy, na ktore zapisany jest aktualny uzytkownik */
    int countCoursesWithReadyQuestions(); /**< Zlicza kursy z gotowymi kartami */
    int countReadyQuestions(int id_course); /**< Zlicza gotowe pytania dla kursu */

    /** Zapisuje parametry oceny pytania do bazy danych */
    bool storeValuesInDatabase(int id_question, double ef, int counter, unsigned long long date, int interval);
    MarkParameters getValuesFromDatabase(int id_question); /**< Pobiera patametry oceny pytania z bazy danych */

private:
    void createUsersTable(); /**< Tworzy w bazie danych tabele uzytkownikow */
    void createQuestionsTable(); /**< Tworzy w bazie danych tabele pytan */
    void createCoursesTable(); /**< Tworzy w bazie danych tabele kursow */
    void createMarksTable(); /**< Tworzy w bazie danych tabele ocen */
    void createCoursesOfUsersTable(); /**< Tworzy w bazie danych tabele subskrypcji kursow */
    void turnOnForeignKey(); /**< Wlacza obsluge kluczy obcych */

    void createAutoMarkTrigger(); /**< Tworzy trigger do auto oceny nowych pytan */
    void createUserAddCourseTrigger(); /**< Tworzy trigger do zapisow uzykownika na kurs */
    void createQuestionEditedTrigger(); /**< Tworzy trigger do restartowania oceny po modyfikacji pytania */
};

#endif // DATABASE_H
