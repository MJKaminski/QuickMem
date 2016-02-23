/*
 * QuickMEM - program wspomagajacy nauke
 * Autorzy: Adrian Stachlewski, Michal Jerzy Kaminski
*/

#ifndef COURSES_H
#define COURSES_H

#include "database.h"

/** Klasa zapewniajaca interfejs do obslugi kursow.
 *  \author Adrian Stachlewski
 */
class Courses
{
    Database::CoursesList coursesList_; /**< Lista wszystkich kursow */
    Database::CoursesList coursesListForCurrentUser_; /**< Lista wszystkich kursow, na ktore zapisany jest aktualny uzytkownik */

    bool validCurrentUser_; /**< Flaga oznaczajaca aktualnosc listy kursow, dla aktualnego uzytkwonika */
    bool valid_;/**< Flaga oznaczajaca aktualnosc listy wszystkich kursow */
    Database *db_; /**< Wskaznik do klasy obslugujacej polaczenie z baza danych */

public:
    Courses(Database *database); /**< Konstruktor klasy Courses, wymaga wskaznika do klasy Database */
    QStringList getCourses(); /**< Zwraca liste wszystkich kursow */
    bool add(QString name); /**< Dodaje kurs o okreslonej nazwie */
    bool erase(QString name); /**< Usuwa kurs o okreslonej nazwie */
    bool set(QString name); /**< Ustawia aktualny kurs */

    QStringList getCoursesForCurrentUser(); /**< Zwraca liste kursow, na ktore zapisany jest aktualny uzytkownik */
    QStringList getUnsubcribeCoursesForCurrentUser(); /**< Zwraca liste kursow, na ktore nie jest zapisany aktualny uzytkownik */
    bool subscribeCourseForCurrentUser(QString name); /**< Zapisuje akutualnego uzytkownika na kurs o podanej nazwie */
    bool unsubscribeCurrentUserFromCourse(QString name); /**< Wypisuje aktualnego uzytkownika z kusru */

    int countSubscribedCourses(); /**< Zlicza liczbe kusrow, na ktore zapisany jest uzytkownik */
    int countCoursesWithReadyQuestion(); /**< Zliczba liczbe kursow z gotowymi pytaniami */
    int countReadyQuestions(int id_course); /**< Zlicza liczbe gotowych pytan w danym kursie */

    bool subscribeCourseForUser(int id_user, QString name); /**< Zapisuje uzytkownika o dowolnym ID na kurs */

    int getID(QString name); /**< Pobiera ID kursu na podstawie nazwy */

private:
    void getCoursesListFromDb(); /**< Sprawdza aktulanosc listy kursow i ewentualnie pobiera dane z bazy */
    void getSubCoursesListFromDb(); /**< Sprawdza aktualnosc listy kursow dla uzytkownika i ewentualnie pobiera dane z bazy */
};

#endif // COURSES_H
