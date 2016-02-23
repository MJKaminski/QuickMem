/*
 * QuickMEM - program wspomagajacy nauke
 * Autorzy: Adrian Stachlewski, Michal Jerzy Kaminski
*/

#ifndef QUESTION_H
#define QUESTION_H

#include <QString>

/** Struktura przechowujaca pojedyncze pytanie
 *  \author Adrian Stachlewski
 */
struct Question
{
    Question(int id_question, QString q, QString a); /**< Konstruktor pytania, wymaga ID, pytania i odpowiedzi */
    int id; /**< ID pytania */
    QString question, answer; /**< pytanie oraz odpowiedz */
};
#endif // QUESTION_H
