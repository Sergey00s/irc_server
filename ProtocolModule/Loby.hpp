#ifndef LOBBY_HPP
# define LOBBY_HPP

#include <iostream>
#include <list>
#include "User.hpp"



class Loby
{

    public:
        std::list<User>             users;
        std::list<std::string>      rooms;
        int                         last_room_id;

    public:
        Loby();
        Loby(Loby const & src);
        ~Loby();

    public:
        Loby    &operator=(Loby const & src);
        
    public:
        int                 add_user(User user);
        int                 move_user(User &user, std::string room_name);
        int                 remove_user(User user);
        int                 get_last_room_id();
        std::list<User>     &get_users();
        std::list<User>     get_users_by_room_name(std::string room_name);
        std::list<User>     get_users_by_room_id(int room_id);
        std::list<User>     get_users_by_status(int status);
        std::list<User>     get_users_by_nick_name(std::string nick_name);
        std::list<User>     get_users_by_real_name(std::string real_name);
        std::list<User>     get_users_by_user_name(std::string user_name);
        std::list<User>     get_users_by_id(int id);
        User                get_user_by_id(int id);
        User                get_user_by_nick_name(std::string nick_name);
        User                get_user_by_real_name(std::string real_name);
        User                get_user_by_user_name(std::string user_name);
        User                *get_user(User user);
};


# endif