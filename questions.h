/*
 * QuickMEM - program wspomagajacy nauke
 * Autorzy: Adrian Stachlewski, Michal Jerzy Kaminski
*/

#ifndef QUESTIONS_H
#define QUESTIONS_H

#include "database.h"

/** Klasa zapewniajaca interfejs do obslugi pytan
 *  \author Adrian Stachlewski
 */
class Questions
{
    bool valid_; /**< Flaga oznaczajaca aktualnosc pobranych danych */
    Database::QuestionsList questions_; /**< Kontener przechowuje liste wszystkich pytan dla danego kursu */
    Database::QuestionsList readyQuestions_; /**< Kontener przechowuje liste gotowych pytan */
    int loadedCourse_; /**< Numer zaladowanego kursu */
    bool loadedReadyQuestions_; /**< Flaga oznaczajaca, czy zostaly pobrane gotowe pytania */
    Database *db_; /**< Wskaznik na klase obslugujaca baze danych */
public:
    Questions(Database *database); /**< Konstruktor klasy Questions, nastawia wskazanie na Database oraz wartosci poczatkowe zmiennych */

    bool addQuestion(QString question, QString answer, int id_course = 0); /**< Dodaje pytanie do bazy danych */
    bool editQuestion(int id_question, QString question, QString answer); /**< Zmienia pytanie w bazie danych */
    bool eraseQuestion(int id_question); /**< Usuwanie pytanie z bazy danych */

    bool rateCurrentQuestion(int mark); /**< Oblicza parametry algorytmu SuperMemo i zapisuje dane do bazy */

    Question getQuestion(); /**< Zwraca pytanie do zadania uzytkownikowi */
    Database::QuestionsList getAllQuestionsFromCourse(int id_course); /**< Pobiera wszystkie pytania z kursu */

private:
    void getQuestionsFromDatabase(int id_course = 0); /**< Sprawdza, czy lista pytan jest aktualna i ewentualnie pobiera je z bazy danych */
    void getReadyQuestionsFromDb(); /**< Sprawdza, czy lista pytan do zadania jest aktualna i ewentualnie pobiera je z bazy danych */
};

#endif // QUESTIONS_H
