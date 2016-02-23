/*
 * QuickMEM - program wspomagajacy nauke
 * Autorzy: Adrian Stachlewski, Michal Jerzy Kaminski
*/

#ifndef USERS_H
#define USERS_H

#include "database.h"

/** Klasa zapewniajaca interfejs zwiazany z obsluga uzytkownikow.
 *  \author Adrian Stachlewski
 */
class Users
{
    Database::UsersList usersList_; /**< Kontener przechowujacy liste uzytkownikow */
    bool valid_; /**< Flaga informujaca o aktualnosci pobranej listy uzytkownikow */
    Database *db_; /**< Wskaznik na klase obslugujaca zapytania do bazy danych */
public:
    Users(Database *database); /**< Konstruktor klasy Users, nastawia wskaznik do klasy obsluhujacej baze danych */
    QStringList getUsers(); /**< Zwraca liste uzytkownikow */
    bool add(QString name); /**< Dodaje uzytkownika do bazy danych */
    bool erase(QString name); /**< Usuwa uzytkownika z bazy danych */
    bool set(QString name); /**< Ustawia aktualnego uzytkownika bazy danych */

    int getID(QString name); /**< Pobiera ID uzytkownika na podstawie nazwy */

private:
    void getUsersListFromDb(); /**< Sprawdza aktualnosc pobranej bazy danych i ewentualnie pobiera ja z bazy danych */
};

#endif // USER_H
