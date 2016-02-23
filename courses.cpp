/*
 * QuickMEM - program wspomagajacy nauke
 * Autorzy: Adrian Stachlewski, Michal Jerzy Kaminski
*/

#include "courses.h"

Courses::Courses(Database *database) :
    validCurrentUser_(false), valid_(false), db_(database)
{

}

QStringList Courses::getCourses()
{
    QStringList coursesList;
    getCoursesListFromDb();
    for (auto &x : coursesList_)
        coursesList.push_back(x.first);
    return coursesList;
}

bool Courses::add(QString name)
{
    valid_ = false;
    return db_->addCourse(name);
}

bool Courses::erase(QString name)
{
    getCoursesListFromDb();
    auto it = coursesList_.find(name);
    if (it == coursesList_.end())
        return false;
    bool ret = db_->deleteCourse(it->second);
    coursesList_.erase(it);
    return ret;
}

bool Courses::set(QString name)
{
    getCoursesListFromDb();
    int id_course = getID(name);
    if (id_course == 0)
        return false;
    db_->setCourse(id_course);
    return true;
}

QStringList Courses::getCoursesForCurrentUser()
{
    QStringList coursesList;
    getSubCoursesListFromDb();
    for (auto &x : coursesListForCurrentUser_)
        coursesList.push_back(x.first);
    return coursesList;
}

QStringList Courses::getUnsubcribeCoursesForCurrentUser()
{
    QStringList coursesList;
    getCoursesListFromDb();
    getSubCoursesListFromDb();
    std::vector<std::pair<QString, int>> difference(coursesList_.size() - coursesListForCurrentUser_.size());
    std::set_difference(coursesList_.begin(), coursesList_.end(), coursesListForCurrentUser_.begin(), coursesListForCurrentUser_.end(), difference.begin());
    QTextStream cout(stdout);
    for (auto &x : difference)
        coursesList.push_back(x.first);
    return coursesList;
}

bool Courses::subscribeCourseForCurrentUser(QString name)
{
    int id_course = getID(name);
    validCurrentUser_ = false;
    return db_->addUserToCourse(id_course);
}

bool Courses::unsubscribeCurrentUserFromCourse(QString name)
{
    int id_course = getID(name);
    validCurrentUser_ = false;
    return db_->deleteUserFromCourse(id_course);
}

int Courses::countSubscribedCourses()
{
    return db_->countSubscribedCourses();
}

int Courses::countCoursesWithReadyQuestion()
{
    return db_->countCoursesWithReadyQuestions();
}

int Courses::countReadyQuestions(int id_course)
{
    return db_->countReadyQuestions(id_course);
}

bool Courses::subscribeCourseForUser(int id_user, QString name)
{
    int id_course = getID(name);
    return db_->addUserToCourse(id_course, id_user);
}

int Courses::getID(QString name)
{
    getCoursesListFromDb();
    auto it = coursesList_.find(name);
    if (it == coursesList_.end())
        return 0;
    return it->second;
}

void Courses::getCoursesListFromDb()
{
    if(!valid_)
    {
        coursesList_ = db_->getCourses();
        valid_ = true;
    }
}

void Courses::getSubCoursesListFromDb()
{
    if(!validCurrentUser_ || db_->currentUserChanged())
    {
        coursesListForCurrentUser_ = db_->getCourses(true);
        validCurrentUser_ = true;
    }
}
