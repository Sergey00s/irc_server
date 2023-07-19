#include "User.hpp"

User::User()
{
    this->id = -1;
    this->user_name = "";
    this->real_name = "";
    this->nick_name = "";
    this->status = STATUS_CONNECTED;
    return ;
    
}

User::User(User const & src)
{
    *this = src;
}


User::User(int id)
{
    this->id = id;
    this->user_name = "";
    this->real_name = "";
    this->nick_name = "";
    this->status = STATUS_CONNECTED;
}

User::User(int id, std::string user_name, std::string real_name, std::string nick_name)
{
    this->id = id;
    this->user_name = user_name;
    this->real_name = real_name;
    this->nick_name = nick_name;
    this->status = STATUS_REGISTERED;
}

User::~User()
{
    return ;
}

User    &User::operator=(User const & src)
{
    this->id = src.id;
    this->user_name = src.user_name;
    this->real_name = src.real_name;
    this->nick_name = src.nick_name;
    this->status = src.status;
    return *this;
}

int         User::get_id()
{
    return this->id;
}

int        User::get_status()
{
    return this->status;
}

std::string User::get_user_name()
{
    return this->user_name;
}

std::string User::get_real_name()
{
    return this->real_name;
}

std::string User::get_nick_name()
{
    return this->nick_name;
}

void       User::set_id(int id)
{
    this->id = id;
    return ;
}

void       User::set_user_name(std::string user_name)
{
    this->user_name = user_name;
    return ;
}



void       User::set_real_name(std::string real_name)
{
    this->real_name = real_name;
    return ;
}

void       User::set_nick_name(std::string nick_name)
{
    this->nick_name = nick_name;
    return ;
}

void       User::set_status(int status)
{
    this->status = status;
    return ;
}

void      User::set_room_name(std::string room_name, int id)
{
    id = 0;
    this->rooms.add(room_name);
    return ;
}

std::string User::get_room_name()
{
    return "";
}

int User::get_room_id()
{
    return -1;
}


int User::check_password(std::string to_check)
{
    if (this->password_in == to_check || to_check == "")
        return 1;
    return 0;
}



void User::set_password(std::string password)
{
    this->password_in = password;
    return ;
}

std::string User::get_password()
{
    return this->password_in;
}