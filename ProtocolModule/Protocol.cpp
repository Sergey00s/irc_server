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
    return (new_messages);
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
        //this->_message_handler(msg, new_messages, id);
    }
}

static std::string raw_msg_erase_crlf(std::string raw_msg)
{
    std::string::iterator it;
    std::string::iterator it2;

    if (raw_msg[raw_msg.size() - 1] == '\n')
        raw_msg.erase(raw_msg.size() - 1);
    if (raw_msg[raw_msg.size() - 1] == '\r')
        raw_msg.erase(raw_msg.size() - 1);
    return raw_msg;
}


Irc_message Protocol::_irc_parser(std::string raw_msg)
{
    Irc_message     msg;
    std::string     command;
    std::vector<std::string>    params;

    raw_msg = raw_msg_erase_crlf(raw_msg);
    
    int i = 0;
    while (i < raw_msg.size())
    {
        if (raw_msg[i] == ' ')
        {
            params.push_back(raw_msg.substr(0, i));
            raw_msg = raw_msg.substr(i + 1, raw_msg.size() - i);
            i = 0;
            continue;
        }
        else if (raw_msg[i] == ':')
        {
            params.push_back(raw_msg.substr(i + 1, raw_msg.size() - i));
            raw_msg = "";
            break;
        }
        i++;
    }
    if (raw_msg.size() > 0)
        params.push_back(raw_msg);
    command = params[0];
    params.erase(params.begin());
    msg.command = command;
    msg.params = params;
    return msg;
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
        this->_nick_command(msg, new_messages, id);
        std::cout << "{nick command}" << std::endl;
        return;
    }
    if (msg.command == "USER")
    {
        this->_user_command(msg, new_messages, id);
        std::cout << "{user command}" << std::endl;
        return;
    }
    if (msg.command == "PING")
    {
         this->_ping_command(msg, new_messages, id);
        std::cout << "{ping command}" << std::endl; 
         return;
    }
    if (msg.command == "PRIVMSG")
    {
        this->_privmsg_command(msg, new_messages, id);

        return;
    }
    if (msg.command == "JOIN")
    {
        this->_join_command(msg, new_messages, id);
        return;
    }
    
}