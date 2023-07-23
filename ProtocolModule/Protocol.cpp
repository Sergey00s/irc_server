#include "Protocol.hpp"

int precheck(Irc_message msg, int type);

Protocol::Protocol(std::string pass)
{
    this->hostname = "localhost";
    this->password = pass;
    this->bot_secret = "123456789";
}

Protocol::Protocol()
{
    this->hostname = "localhost";
    this->password = "";
    this->bot_secret = "123456789";

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
    this->bot_secret = "123456789";
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

std::string     Protocol::get_bot_secret()
{
    return this->bot_secret;
}

void           Protocol::set_bot_secret(std::string bot_secret)
{
    this->bot_secret = bot_secret;
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

int Protocol::_protocol_command_handler(MESSAGE recv)
{
    std::string status = recv.status;
    if (status == "0000")
    {
        this->loby.remove_user(recv.id);
        std::cout << "User " << recv.id << " disconnected" << std::endl;
        return (1);
    }
    return (0);
}



void  Protocol::_message_handler(MESSAGE recv, std::list<MESSAGE> &new_messages)
{
    std::string     raw_msg;
    int             id;

    raw_msg = recv.message;
    id = recv.id;

    if (_protocol_command_handler(recv))
        return ;

    if (raw_msg.size() <= 1)
        return ;
    
    if (_bot_is_bot(raw_msg))
    {
        std::cout << "BOT REQUEST" << std::endl;
        BOT bot = bot_parse(raw_msg);
        if (bot_auth(this->bot_secret, bot.bot_secret))
        {
            std::cout << "BOT PARSED" << std::endl;
            this->_bot_command_handler(bot, new_messages, id);
        }
        return ;
    }

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
    
    if (raw_msg.size() <= 1)
        return (raw_msg);
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
    User current = this->loby.get_user_by_id(id);
    User *user = this->loby.get_user(current);
    std::cout << "command handler :" << msg.command << std::endl;
    if (user == NULL)
    {
        User new_user(id);
        loby.add_user(new_user);
        user = this->loby.get_user(new_user);
    }
    if (msg.command == "NICK")
    {
        this->_nick_command(msg, new_messages, id);
        std::cout << "{nick command}" << std::endl;
    }
    else if (msg.command == "USER")
    {
        this->_user_command(msg, new_messages, id);
        std::cout << "{user command}" << std::endl;
    }
    else if (msg.command == "PASS")
    {
        this->_pass_command(msg, new_messages, id);
        std::cout << "{pass command}" << std::endl;
    }
    if (user && user->get_nick_name() != "" && user->get_nick_name() != ""  && user->get_status() != STATUS_REGISTERED)
    {
        if (this->get_password() != "")
        {
            if (user->get_password() != this->get_password())
            {
                MESSAGE new_message;
                new_message.id = id;
                if (user->get_password() == "")
                    new_message.message = ":localhost 464 " + user->get_nick_name() + " :Password required";
                else
                    new_message.message = ":localhost 464 " + user->get_nick_name() + " :Password incorrect";
                new_messages.push_back(new_message);
                return;
            }
        }
        user->set_status(STATUS_REGISTERED);
        MESSAGE new_message;
        new_message.id = id;
        new_message.message = ":localhost 001 " + user->get_nick_name() + " :Welcome to the Internet Relay Network\r\n";
        new_messages.push_back(new_message);
        return;
    }
    if (msg.command != "NICK" && msg.command != "USER" && msg.command != "PASS" && user->get_status() != STATUS_REGISTERED)
    {
        MESSAGE new_message;
        new_message.id = id;
        new_message.message = ":localhost 451 :You have not registered\r\n";
        new_messages.push_back(new_message);
        return;
    }
    if (msg.command == "PING")
    {
         this->_ping_command(msg, new_messages, id);
    }
    else if (msg.command == "PRIVMSG")
    {
        if (precheck(msg, PRIVMSG))
            this->_privmsg_command(msg, new_messages, id);
    }
    else if (msg.command == "JOIN")
    {
        this->_join_command(msg, new_messages, id);
    }
    else if (msg.command == "PART")
    {
        this->_part_command(msg, new_messages, id);
    }
    else if (msg.command == "QUIT")
    {
        this->_quit_command(msg, new_messages, id);
    }
    else if (msg.command == "KICK")
    {
        this->_kick_command(msg, new_messages, id);
    }
}