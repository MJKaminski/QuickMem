/*
 * QuickMEM - program wspomagajacy nauke
 * Autorzy: Adrian Stachlewski, Michal Jerzy Kaminski
*/

#include "database.h"

Database::Database() :
    validError_(false), currentUserId_(0), currentCourseId_(0), currentUserChanged_(true)
{    
    db_ = QSqlDatabase::addDatabase("QSQLITE");
    db_.setDatabaseName("pytania.db");
    if(!db_.open())
    {
        error_ = "Cannot open database file.";
        validError_ = true;
    }
    turnOnForeignKey();
    createUsersTable();
    createCoursesTable();
    createQuestionsTable();
    createCoursesOfUsersTable();
    createMarksTable();

    createAutoMarkTrigger();
    createUserAddCourseTrigger();
    createQuestionEditedTrigger();
}

Database::~Database()
{
    db_.close();
}

Database::UsersList Database::getUsers()
{
    UsersList list;
    QSqlQuery query;
    query.exec("SELECT id_user, name FROM users");
    while(query.next())
        list.emplace(query.value(1).toString(), query.value(0).toInt());
    return list;
}

bool Database::addUser(QString name)
{
    QSqlQuery query;
    query.prepare("INSERT INTO users VALUES (NULL, ?)");
    query.addBindValue(name);
    if (!query.exec())
    {
        validError_ = true;
        error_ = "Dodanie użytkownika nie powiodło się";
        return false;
    }
    return true;
}

bool Database::deleteUser(int id_user)
{
    QSqlQuery query;
    query.prepare("DELETE FROM users WHERE id_user = ?");
    query.addBindValue(id_user);
    if (!query.exec())
    {
        validError_ = true;
        error_ = "Usunięcie użytkownika nie powiodło się";
        return false;
    }
    return true;
}

void Database::setUser(int id_user)
{
    currentUserId_ = id_user;
    currentUserChanged_ = true;
}

Database::CoursesList Database::getCourses(bool for_current_user)
{
    CoursesList list;
    QSqlQuery query;
    if (for_current_user)
    {
        currentUserChanged_ = false;
        query.prepare("SELECT courses.id_course, courses.name "
                      "FROM coursesOfUsers JOIN courses ON coursesOfUsers.id_course = courses.id_course "
                      "WHERE coursesOfUsers.id_user = ?");
        query.addBindValue(currentUserId_);
        query.exec();
    }
    else
    {
        query.exec("SELECT id_course, name FROM courses");
    }
    while(query.next())
        list.emplace(query.value(1).toString(), query.value(0).toInt());
    return list;
}

bool Database::addCourse(QString name)
{
    QSqlQuery query;
    query.prepare("INSERT INTO courses VALUES (NULL, ?)");
    query.addBindValue(name);
    if (!query.exec())
    {
        validError_ = true;
        error_ = "Dodanie kursu nie powiodło się";
        return false;
    }
    return true;
}

bool Database::deleteCourse(int id_course)
{
    QSqlQuery query;
    query.prepare("DELETE FROM courses WHERE id_course = ?");
    query.addBindValue(id_course);
    if (!query.exec())
    {
        validError_ = true;
        error_ = "Usnięcie kursu nie powiodło się";
        return false;
    }
    return true;
}

void Database::setCourse(int id_course)
{
    currentCourseId_ = id_course;
    currentCourseChanged_ = true;
}

bool Database::addUserToCourse(int id_course, int id_user)
{
    if(!id_user)
        id_user = currentUserId_;
    QSqlQuery query;
    query.prepare("INSERT INTO coursesOfUsers VALUES (?, ?)");
    query.addBindValue(id_user);
    query.addBindValue(id_course);
    if (!query.exec())
    {
        validError_ = true;
        error_ = "Subskrypcja kursu nie powiodła się";
        return false;
    }
    return true;
}

bool Database::deleteUserFromCourse(int id_course, int id_user)
{
    if(!id_user)
        id_user = currentUserId_;
    QSqlQuery query;
    query.prepare("DELETE FROM coursesOfUsers WHERE id_course = ? AND id_user = ?");
    query.addBindValue(id_course);
    query.addBindValue(id_user);
    if (!query.exec())
    {
        validError_ = true;
        error_ = "Usuniecię subskrypcji nie powiodło się";
        return false;
    }
    return true;
}

bool Database::currentUserChanged()
{
    return currentUserChanged_;
}

bool Database::currentCourseChanged()
{
    return currentCourseChanged_;
}

bool Database::addQuestion(int id_course, QString question, QString answer)
{
    QSqlQuery query;
    query.prepare("INSERT INTO questions VALUES (NULL, ?, ?, ?);");
    query.addBindValue(question);
    query.addBindValue(answer);
    query.addBindValue(id_course);
    if (!query.exec())
    {
        validError_ = true;
        error_ = "Dodanie pytania nie powiodło się";
        return false;
    }
    return true;
}

bool Database::deleteQuestion(int id_question)
{
    QSqlQuery query;
    query.prepare("DELETE FROM questions WHERE questions.id_question = ?");
    query.addBindValue(id_question);
    if (!query.exec())
    {
        validError_ = true;
        error_ = "Usunięcie pytania nie powiodło się";
        return false;
    }
    return true;
}

bool Database::editQuestion(int id_question, QString question, QString answer)
{
    QSqlQuery query;
    query.prepare("UPDATE questions SET question = ?, answer = ? WHERE id_question = ?");
    query.addBindValue(question);
    query.addBindValue(answer);
    query.addBindValue(id_question);
    if (!query.exec())
    {
        validError_ = true;
        error_ = "Modyfikacja pytania nie powiodła się";
        return false;
    }
    return true;
}

int Database::countSubscribedCourses()
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(id_user) FROM coursesOfUsers WHERE id_user = ?");
    query.addBindValue(currentUserId_);
    query.exec();
    query.next();
    return query.value(0).toInt();
}

int Database::countCoursesWithReadyQuestions()
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(how_many_questions) "
                  "FROM (SELECT COUNT(id_user) as how_many_questions "
                  "FROM coursesOfUsers NATURAL JOIN questions NATURAL JOIN marks "
                  "WHERE coursesOfUsers.id_user = ?  AND (marks.date <= ? OR marks.counter = 0) "
                  "GROUP BY id_course)");
    query.addBindValue(currentUserId_);
    query.addBindValue(QDate::currentDate().toJulianDay());
    query.exec();
    query.next();
    return query.value(0).toInt();
}

int Database::countReadyQuestions(int id_course)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(id_question) "
                  "FROM questions NATURAL JOIN marks "
                  "WHERE id_user = ? AND id_course = ? AND (date <= ? OR counter = 0 )");
    query.addBindValue(currentUserId_);
    query.addBindValue(id_course);
    query.addBindValue(QDate::currentDate().toJulianDay());
    query.exec();
    query.next();
    return query.value(0).toInt();
}

bool Database::storeValuesInDatabase(int id_question, double ef, int counter, unsigned long long date, int interval)
{
    QSqlQuery query;
    query.prepare("UPDATE marks "
                  "SET ef = ?, counter = ?, date = ?, interval = ? "
                  "WHERE marks.id_question = ? AND marks.id_user = ?");
    query.addBindValue(ef);
    query.addBindValue(counter);
    query.addBindValue(date);
    query.addBindValue(interval);
    query.addBindValue(id_question);
    query.addBindValue(currentUserId_);
    if (!query.exec())
    {
        validError_ = true;
        error_ = "Zapis ocen nie powiódł się";
        return false;
    }
    return true;
}

Database::MarkParameters Database::getValuesFromDatabase(int id_question)
{
    QSqlQuery query;
    query.prepare("SELECT ef, counter, date, interval "
                  "FROM marks "
                  "WHERE id_user = ? AND id_question = ?");
    query.addBindValue(currentUserId_);
    query.addBindValue(id_question);
    query.exec();
    query.next();
    return MarkParameters(query.value(0).toDouble(), query.value(1).toInt(), query.value(2).toLongLong(), query.value(3).toInt());
}

Database::QuestionsList Database::getQuestions(int idCourse, bool onlyReady, int id_user)
{
    if (!idCourse)
        idCourse = currentCourseId_;
    if (!id_user)
        id_user = currentUserId_;
    QSqlQuery query;
    if (onlyReady)
    {
        currentCourseChanged_ = false;
        query.prepare("SELECT questions.id_question, questions.question, questions.answer "
                      "FROM questions JOIN marks ON questions.id_question = marks.id_question "
                      "WHERE marks.id_user = ? AND (marks.date <= ? OR marks.counter = 0) AND questions.id_course = ?");
        query.addBindValue(id_user);
        query.addBindValue(QDate::currentDate().toJulianDay());
    }
    else
    {
        query.prepare("SELECT id_question, question, answer "
                      "FROM questions "
                      "WHERE id_course = ?");
    }
    query.addBindValue(idCourse);
    query.exec();
    QuestionsList list;
    while(query.next())
        list.emplace_back(query.value(0).toInt(), query.value(1).toString(), query.value(2).toString());
    return list;
}

QString Database::getLastError()
{
    validError_ = false;
    return error_;
}

bool Database::validError()
{
    return validError_;
}

void Database::createUsersTable()
{
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS users ("
               "id_user         INTEGER PRIMARY KEY AUTOINCREMENT,"
               "name            TEXT UNIQUE"
               ")");
}

void Database::createQuestionsTable()
{
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS questions ("
               "id_question     INTEGER PRIMARY KEY AUTOINCREMENT,"
               "question        TEXT NOT NULL,"
               "answer      	TEXT NOT NULL,"
               "id_course   	INTEGER,"
               "FOREIGN KEY(id_course)      REFERENCES courses (id_course) ON DELETE CASCADE"
               ")");
}

void Database::createCoursesTable()
{
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS courses ("
               "id_course       INTEGER PRIMARY KEY AUTOINCREMENT,"
               "name            TEXT UNIQUE"
               ")");
}

void Database::createMarksTable()
{
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS marks ("
               "id_user         INTEGER NOT NULL,"
               "id_question     INTEGER NOT NULL,"
               "ef          	REAL NOT NULL DEFAULT 2.5,"
               "counter     	INTEGER NOT NULL DEFAULT 0,"
               "date        	INTEGER NOT NULL DEFAULT 0,"
               "interval    	INTEGER NOT NULL DEFAULT 0,"
               "PRIMARY KEY(id_user, id_question),"
               "FOREIGN KEY(id_user)        REFERENCES users (id_user) ON DELETE CASCADE,"
               "FOREIGN KEY(id_question)    REFERENCES questions (id_question) ON DELETE CASCADE"
               ")");
}

void Database::createCoursesOfUsersTable()
{
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS coursesOfUsers ("
               "id_user         INTEGER NOT NULL,"
               "id_course       INTEGER NOT NULL,"
               "PRIMARY KEY(id_user,id_course),"
               "FOREIGN KEY(id_user)        REFERENCES users (id_user) ON DELETE CASCADE,"
               "FOREIGN KEY(id_course)      REFERENCES courses (id_course) ON DELETE CASCADE"
               ")");
}

void Database::turnOnForeignKey()
{
    QSqlQuery query;
    query.exec("PRAGMA foreign_keys = 1;");
}

void Database::createAutoMarkTrigger()
{
    QSqlQuery query;
    query.exec("CREATE TRIGGER IF NOT EXISTS autoMark AFTER INSERT ON questions FOR EACH ROW "
               "BEGIN "
               "INSERT INTO marks (id_user, id_question) "
               "SELECT id_user, id_question "
               "FROM coursesOfUsers JOIN questions ON coursesOfUsers.id_course = questions.id_course "
               "WHERE id_question = NEW.id_question; "
               "END");
}

void Database::createUserAddCourseTrigger()
{
    QSqlQuery query;
    query.exec("CREATE TRIGGER IF NOT EXISTS userAddCourse AFTER INSERT ON coursesOfUsers "
               "BEGIN "
               "INSERT INTO marks (id_user, id_question) "
               "SELECT id_user, id_question "
               "FROM coursesOfUsers JOIN questions ON coursesOfUsers.id_course = questions.id_course "
               "WHERE id_user = NEW.id_user AND coursesOfUsers.id_course = NEW.id_course; "
               "END");
}

void Database::createQuestionEditedTrigger()
{
    QSqlQuery query;
    query.exec("CREATE TRIGGER IF NOT EXISTS questionEdited AFTER UPDATE ON questions "
               "BEGIN "
               "UPDATE marks SET ef = 2.5, counter = 0 WHERE marks.id_question = OLD.id_question; "
               "END;");
}
