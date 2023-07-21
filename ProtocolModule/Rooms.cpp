#include "User.hpp"



ROOMS::ROOMS()
{
    return ;
}

ROOMS::ROOMS(ROOMS const & src)
{
    this->rooms = src.rooms;
    *this = src;
}

ROOMS::~ROOMS()
{
    return ;
}

ROOMS   &ROOMS::operator=(ROOMS const & src)
{
    this->rooms = src.rooms;
    return *this;
}

int             ROOMS::in(std::string room_name)
{
    std::list<ROOM>::iterator it;

    it = this->rooms.begin();
    while (it != this->rooms.end())
    {
        if (it->room_name == room_name)
            return 1;
        it++;
    }
    return 0;
}

int             ROOMS::op_level(std::string room_name)
{
    std::list<ROOM>::iterator it;

    if (rooms.size() == 0)
        std::cout << "rooms is empty" << std::endl;

    it = this->rooms.begin();
    while (it != this->rooms.end())
    {
        if (it->room_name == room_name)
            return it->op_level;
        it++;
    }
    return -1;
}

int ROOMS::id(std::string room_name)
{
    std::list<ROOM>::iterator it;

    it = this->rooms.begin();
    while (it != this->rooms.end())
    {
        if (it->room_name == room_name)
            return it->id;
        it++;
    }
    return -1;
}

void            ROOMS::add(std::string room_name)
{
    ROOM room;
    room.room_name = room_name;
    room.id = this->rooms.size();
    room.op_level = -1;
    this->rooms.push_back(room);
}

void           ROOMS::out(std::string room_name)
{
    std::list<ROOM>::iterator it;

    it = this->rooms.begin();
    while (it != this->rooms.end())
    {
        if (it->room_name == room_name)
        {
            this->rooms.erase(it);
            return ;
        }
        it++;
    }
}


void            ROOMS::set_op_level(std::string room_name, int op_level)
{
    std::list<ROOM>::iterator it;

    it = this->rooms.begin();
    while (it != this->rooms.end())
    {
        if (it->room_name == room_name)
        {
            it->op_level = op_level;
            return ;
        }
        it++;
    }
}

