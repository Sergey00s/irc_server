#ifndef USER_HPP
# define USER_HPP


#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <list>
# include "Scodes.hpp"


struct ROOM
{
    std::string    room_name;
    int            op_level;
    int            id;
};


struct ROOMS
{
    std::list<ROOM> rooms;
    
    ROOMS();
    ROOMS(ROOMS const & src);
    ~ROOMS();
    ROOMS           &operator=(ROOMS const & src);
    int             in(std::string room_name);
    int             op_level(std::string room_name);
    int             id(std::string room_name);
    void            add(std::string room_name);
    void           out(std::string room_name); 
    void            set_op_level(std::string room_name, int op_level);
};


class User
{
    int id;

    std::string user_name;
    std::string real_name;
    std::string nick_name;
    std::string password_in;
    int         status;
    
    public:
        ROOMS       rooms;

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
        std::string get_password();
        void       set_password(std::string password);
        void        set_id(int id);
        void        set_user_name(std::string user_name);
        void        set_real_name(std::string real_name);
        void        set_nick_name(std::string nick_name);
        int         get_status();
        void        set_status(int status);
        void        set_room_name(std::string room_name, int room_id);
        int         check_password(std::string to_check);
        
};


#endif