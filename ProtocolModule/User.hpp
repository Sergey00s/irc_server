#ifndef USER_HPP
# define USER_HPP


#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <list>
# include "Scodes.hpp"


class User
{
    int id;

    std::string user_name;
    std::string real_name;
    std::string nick_name;
    int         status;
    int         op_level;

    public:
        std::string     room_name;
        int             room_id;

    public:
        User();
        User(int id);
        User(User const & src);
        User(int id, std::string user_name, std::string real_name, std::string nick_name);
        ~User();
    
    public:
        User    &operator=(User const & src);
    
    public:
        int         get_id();
        int         get_room_id();
        std::string get_user_name();
        std::string get_real_name();
        std::string get_nick_name();
        std::string get_room_name();
        void        set_id(int id);
        void        set_user_name(std::string user_name);
        void        set_real_name(std::string real_name);
        void        set_nick_name(std::string nick_name);
        int         get_status();
        void        set_status(int status);
        void        set_room_name(std::string room_name, int room_id);
        
};


#endif