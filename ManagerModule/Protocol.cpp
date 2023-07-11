#include "Protocol.hpp"


Protocol::Protocol()
{
    this->hostname = "localhost";
    this->password = "password";
}

Protocol::Protocol(Protocol const & src)
{
    *this = src;
}

Protocol::~Protocol()
{
    return ;
}


Protocol    &Protocol::operator=(Protocol const & src)
{
    this->hostname = src.hostname;
    this->password = src.password;
    return *this;
}

std::string    Protocol::get_hostname()
{
    return this->hostname;
}

std::string    Protocol::get_password()
{
    return this->password;
}

void           Protocol::set_hostname(std::string hostname)
{
    this->hostname = hostname;
}

void           Protocol::set_password(std::string password)
{
    this->password = password;
}

std::list<MESSAGE>      Protocol::update(std::list<MESSAGE> messages)
{
    std::list<MESSAGE>      new_messages;
    std::list<MESSAGE>::iterator it;

    for (it = messages.begin(); it != messages.end(); it++)
    {
                this->_message_handler(*it, new_messages);
    }
}

void  Protocol::_message_handler(MESSAGE recv, std::list<MESSAGE> &new_messages)
{
    std::string     raw_msg;
    int             id;

    raw_msg = recv.message;
    id = recv.id;

    Irc_message msg = this->_irc_parser(raw_msg);
    if (this->_is_command(msg.command))
    {
        this->_command_handler(msg, new_messages, id);
    }
    else
    {
        this->_message_handler(msg, new_messages, id);
    }
}

Irc_message Protocol::_irc_parser(std::string raw_msg)
{
    Irc_message     irc_msg;
    std::string     prefix;
    std::string     command;
    std::vector<std::string>    params;
    std::string     trailing;
    std::string     tmp;
    int             i;
    int             j;

    i = 0;
    j = 0;
    while (raw_msg[i] == ' ')
        i++;
    if (raw_msg[i] == ':')
    {
        while (raw_msg[i] != ' ')
        {
            prefix += raw_msg[i];
            i++;
        }
        i++;
    }
    while (raw_msg[i] != ' ')
    {
        command += raw_msg[i];
        i++;
    }
    i++;
    while (raw_msg[i] != ':')
    {
        tmp = "";
        while (raw_msg[i] != ' ')
        {
            tmp += raw_msg[i];
            i++;
        }
        params.push_back(tmp);
        i++;
    }
    i++;
    while (raw_msg[i] != '\0')
    {
        trailing += raw_msg[i];
        i++;
    }
    irc_msg.raw = raw_msg;
    irc_msg.prefix = prefix;
    irc_msg.command = command;
    irc_msg.params = params;
    irc_msg.trailing = trailing;
    return irc_msg;
}


int        Protocol::_is_command(std::string command)
{
    if (command == "PRIVMSG")
        return 1;
    if (command == "JOIN")
        return 1;
    if (command == "PART")
        return 1;
    if (command == "QUIT")
        return 1;
    if (command == "NICK")
        return 1;
    if (command == "USER")
        return 1;
    if (command == "PASS")
        return 1;
    if (command == "PING")
        return 1;
    if (command == "PONG")
        return 1;
    if (command == "TOPIC")
        return 1;
    if (command == "LIST")
        return 1;
    return 0;
}



void        Protocol::_command_handler(Irc_message msg, std::list<MESSAGE> &new_messages, int id)
{
    if (msg.command == "NICK")
    {
        std::string     new_nick = msg.params[0];
        this->loby.get_user_by_id(id).set_nick_name(new_nick);
        return;
    }
    if (msg.command == "PRIVMSG")
    {
        MESSAGE     new_msg;
        new_msg.message = msg.trailing;
        std::string     to_who = msg.params[0];
        new_msg.id = this->loby.get_user_by_nick_name(to_who).get_id();
        new_messages.push_back(new_msg);
        return;
    }
    if (msg.command == "JOIN")
    {
        std::string     channel = msg.params[0];
        loby.move_user()
        return;
    }

}