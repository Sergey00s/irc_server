#include "Loby.hpp"

Loby::Loby()
{
    return ;
}

Loby::Loby(Loby const & src)
{
    *this = src;
}

Loby::~Loby()
{
    return ;
}


Loby    &Loby::operator=(Loby const & src)
{
    this->users = src.users;
    this->last_room_id = src.last_room_id;
    return *this;
}


int     Loby::add_user(User user)
{
    if (user.get_id() == -1)
        return -1;

    user.set_room_name("Loby", 0);
    this->users.push_back(user);
    return 0;
}

int     Loby::move_user(User *user, std::string room_name)
{
    int check = 0;

    std::list<User> users_in_ch = this->get_users_by_room_name(room_name);
    if (users_in_ch.size() > 0)
        check = 1;
    user->set_room_name(room_name, this->get_last_room_id());
    if (check == 0)
        user->rooms.set_op_level(room_name, 1);
    else
        user->rooms.set_op_level(room_name, 0);
    return 1;
}

std::list<User>     Loby::get_users_by_room_name(std::string room_name)
{
    std::list<User>     users_in;
    std::list<User>::iterator it;
    for (it = this->users.begin(); it != this->users.end(); it++)
    {
        if (it->rooms.in(room_name))
            users_in.push_back(*it);
    }
    return users_in;
}

// std::list<User>     Loby::get_users_by_room_id(int room_id)
// {
//     std::list<User>     users;
//     std::list<User>::iterator it;
//     for (it = this->users.begin(); it != this->users.end(); it++)
//     {
//         if (it->get_room_id() == room_id)
//             users.push_back(*it);
//     }
//     return users;
// }


std::list<User>     Loby::get_users_by_status(int status)
{
    std::list<User>     users;
    std::list<User>::iterator it;
    for (it = this->users.begin(); it != this->users.end(); it++)
    {
        if (it->get_status() == status)
            users.push_back(*it);
    }
    return users;
}

std::list<User>     Loby::get_users_by_nick_name(std::string nick_name)
{
    std::list<User>     users;
    std::list<User>::iterator it;
    for (it = this->users.begin(); it != this->users.end(); it++)
    {
        if (it->get_nick_name() == nick_name)
            users.push_back(*it);
    }
    return users;
}

std::list<User>     Loby::get_users_by_real_name(std::string real_name)
{
    std::list<User>     users;
    std::list<User>::iterator it;
    for (it = this->users.begin(); it != this->users.end(); it++)
    {
        if (it->get_real_name() == real_name)
            users.push_back(*it);
    }
    return users;
}


std::list<User>     Loby::get_users_by_user_name(std::string user_name)
{
    std::list<User>     users;
    std::list<User>::iterator it;
    for (it = this->users.begin(); it != this->users.end(); it++)
    {
        if (it->get_user_name() == user_name)
            users.push_back(*it);
    }
    return users;
}

std::list<User>     Loby::get_users_by_id(int id)
{
    std::list<User>     users;
    std::list<User>::iterator it;
    for (it = this->users.begin(); it != this->users.end(); it++)
    {
        if (it->get_id() == id)
            users.push_back(*it);
    }
    return users;
}

int     Loby::get_last_room_id()
{
    return this->last_room_id;
}


User  Loby::get_user_by_id(int id)
{
    std::list<User>::iterator it;
    for (it = this->users.begin(); it != this->users.end(); it++)
    {
        if (it->get_id() == id)
            return *it;
    }
    return User();
}

User  Loby::get_user_by_nick_name(std::string nick_name)
{
    std::list<User>::iterator it;
    for (it = this->users.begin(); it != this->users.end(); it++)
    {
        if (it->get_nick_name() == nick_name)
            return *it;
    }
    return User();
}

User  Loby::get_user_by_real_name(std::string real_name)
{
    std::list<User>::iterator it;
    for (it = this->users.begin(); it != this->users.end(); it++)
    {
        if (it->get_real_name() == real_name)
            return *it;
    }
    return User();
}

User  Loby::get_user_by_user_name(std::string user_name)
{
    std::list<User>::iterator it;
    for (it = this->users.begin(); it != this->users.end(); it++)
    {
        if (it->get_user_name() == user_name)
            return *it;
    }
    return User();
}

int  Loby::remove_user(User user)
{
    std::list<User> updated;
    std::list<User>::iterator it;
    for (it = this->users.begin(); it != this->users.end(); it++)
    {
        if (it->get_id() != user.get_id())
            updated.push_back(*it);
    }
    this->users = updated;
    return 1;
}


User *Loby::get_user(User user)
{
    std::list<User>::iterator it;
    for (it = this->users.begin(); it != this->users.end(); it++)
    {
        if (it->get_id() == user.get_id())
            return &*it;
    }
    return NULL;
}

std::list<User> &Loby::get_users()
{
    return this->users;
}


std::list<std::string> Loby::get_rooms()
{
    std::list<std::string> rooms;

    std::list<User>::iterator it;
    for (it = this->users.begin(); it != this->users.end(); it++)
    {
        std::list<ROOM>::iterator it2;
        for (it2 = it->rooms.rooms.begin(); it2 != it->rooms.rooms.end(); it2++)
        {
            
            if (std::find(rooms.begin(), rooms.end(), it2->room_name) == rooms.end())
                rooms.push_back(it2->room_name);
        }
    }
    return rooms;
}