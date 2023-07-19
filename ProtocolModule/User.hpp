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

    ROOM()
    {
        this->room_name = "";
        this->op_level = 0;
        this->id = 0;
    }
    ROOM(std::string room_name, int op_level, int id)
    {
        this->room_name = room_name;
        this->op_level = op_level;
        this->id = id;
    }
    ROOM(ROOM const & src)
    {
        this->room_name = src.room_name;
        this->op_level = src.op_level;
        this->id = src.id;
    }
    ~ROOM()
    {
        return ;
    }
    ROOM    &operator=(ROOM const & src)
    {
        this->room_name = src.room_name;
        this->op_level = src.op_level;
        this->id = src.id;
        return *this;
    }
    ROOM(std::string room_name)
    {
        this->room_name = room_name;
        this->op_level = 0;
        this->id = 0;
    }
};


struct ROOMS
{
    std::list<ROOM> rooms;

    int             in(std::string room_name)
    {
        for (std::list<ROOM>::iterator it = this->rooms.begin(); it != this->rooms.end(); it++)
        {
            if (it->room_name == room_name)
                return 1;
        }
        return 0;
    }
    int             op_level(std::string room_name)
    {
        for (std::list<ROOM>::iterator it = this->rooms.begin(); it != this->rooms.end(); it++)
        {
            if (it->room_name == room_name)
                return it->op_level;
        }
        return 0;
    }
    int             id(std::string room_name)
    {
        for (std::list<ROOM>::iterator it = this->rooms.begin(); it != this->rooms.end(); it++)
        {
            if (it->room_name == room_name)
                return it->id;
        }
        return 0;
    }
    void            add(std::string room_name)
    {
        ROOM    new_room(room_name);
        this->rooms.push_back(new_room);
    }
    void           out(std::string room_name)
    {
        for (std::list<ROOM>::iterator it = this->rooms.begin(); it != this->rooms.end(); it++)
        {
            if (it->room_name == room_name)
            {
                this->rooms.erase(it);
                return ;
            }
        }
    } 
    void            set_op_level(std::string room_name, int op_level)
    {
        for (std::list<ROOM>::iterator it = this->rooms.begin(); it != this->rooms.end(); it++)
        {
            if (it->room_name == room_name)
            {
                it->op_level = op_level;
                return ;
            }
        }
    }
};


class User
{
    int id;

    std::string user_name;
    std::string real_name;
    std::string nick_name;
    std::string password_in;
    int         status;
    int         op_level;
    
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