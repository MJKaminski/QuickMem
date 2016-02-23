/*
 * QuickMEM - program wspomagajacy nauke
 * Autorzy: Adrian Stachlewski, Michal Jerzy Kaminski
*/

#include "questions.h"

Questions::Questions(Database *database) :
    valid_(false), loadedCourse_(0), loadedReadyQuestions_(false), db_(database)
{

}

bool Questions::addQuestion(QString question, QString answer, int id_course)
{
    bool var = db_->addQuestion(id_course, question, answer);
    if (var && id_course == loadedCourse_) questions_.emplace_back(id_course, question, answer);
    return var;
}

bool Questions::editQuestion(int id_question, QString question, QString answer)
{
    valid_ = false;
    return db_->editQuestion(id_question, question, answer);
}

bool Questions::eraseQuestion(int id_question)
{
    valid_ = false;
    return db_->deleteQuestion(id_question);
}

bool Questions::rateCurrentQuestion(int mark)
{
    double oldEF, newEF;
    qint64 oldDate, newDate;
    int oldCounter, newCounter;
    int oldInterval, newInterval;
    Question question = readyQuestions_.back();
    readyQuestions_.pop_back();

    std::tie(oldEF, oldCounter, oldDate, oldInterval) = db_->getValuesFromDatabase(question.id);

    newEF = oldEF + (0.1 - (5-mark) * (0.08 +(5-mark) * 0.02));

    if(newEF < 1.3)
        newEF = 1.3;

    if(mark<3)
    {
        readyQuestions_.insert(readyQuestions_.begin(), question);
        newCounter = 0;
        newDate = oldDate;
        newInterval = 0;
    }
    else
    {

        if(oldCounter == 0)
        {
            newDate = QDate::currentDate().addDays(1).toJulianDay();
            newInterval = 1;
        }
        else if(oldCounter == 1)
        {
            newDate = QDate::currentDate().addDays(6).toJulianDay();
            newInterval = 6;
        }

        else
        {

            newInterval = (int) round(oldInterval * newEF);
            newDate = QDate::currentDate().addDays(newInterval).toJulianDay();
        }

        newCounter = ++oldCounter;
    }

    return db_->storeValuesInDatabase(question.id, newEF, newCounter, newDate, newInterval);
}

Question Questions::getQuestion()
{
    if (readyQuestions_.empty())
        loadedReadyQuestions_ = false;
    getReadyQuestionsFromDb();
    if (readyQuestions_.empty())
        return Question(0, "Koniec pytań", "");
    return readyQuestions_[readyQuestions_.size()-1];
}

Database::QuestionsList Questions::getAllQuestionsFromCourse(int id_course)
{
    getQuestionsFromDatabase(id_course);
    return questions_;
}

void Questions::getQuestionsFromDatabase(int id_course)
{
    if (!valid_ || loadedCourse_ != id_course)
    {
        questions_ = db_->getQuestions(id_course, false);
        valid_ = true;
        loadedCourse_ = id_course;
    }
}

void Questions::getReadyQuestionsFromDb()
{
    if (!loadedReadyQuestions_ || db_->currentCourseChanged() || db_->currentUserChanged())
    {
        readyQuestions_ = db_->getQuestions();
        loadedReadyQuestions_ = true;
    }
}


