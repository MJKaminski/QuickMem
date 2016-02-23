/*
 * QuickMEM - program wspomagajacy nauke
 * Autorzy: Adrian Stachlewski, Michal Jerzy Kaminski
*/

#include "users.h"

Users::Users(Database *database) :
     valid_(false), db_(database)
{

}

QStringList Users::getUsers()
{
    QStringList usersList;
    getUsersListFromDb();
    for (auto &x : usersList_)
        usersList.push_back(x.first);
    return usersList;
}

bool Users::add(QString name)
{
    valid_ = false;
    return db_->addUser(name);
}

bool Users::erase(QString name)
{
    getUsersListFromDb();
    auto it = usersList_.find(name);
    if (it == usersList_.end())
        return false;
    bool ret = db_->deleteUser(it->second);
    usersList_.erase(it);
    return ret;
}

bool Users::set(QString name)
{
    getUsersListFromDb();
    auto it = usersList_.find(name);
    if (it == usersList_.end())
        return false;
    db_->setUser(it->second);
    return true;
}

int Users::getID(QString name)
{
    getUsersListFromDb();
    auto it = usersList_.find(name);
    if (it == usersList_.end())
        return 0;
    return it->second;
}

void Users::getUsersListFromDb()
{
    if(!valid_)
    {
        usersList_ = db_->getUsers();
        valid_ = true;
    }
}
