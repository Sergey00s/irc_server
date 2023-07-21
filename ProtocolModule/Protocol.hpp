#ifndef PROTOCOL_HPP
# define PROTOCOL_HPP


# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <list>
# include "Loby.hpp"
# include "Scodes.hpp"


struct MESSAGE
{
    std::string     message;
    int             id;
    std::string     status;
};

struct Irc_message
{
    std::string     raw;
    std::string     command;
    std::vector<std::string>    params;
};

class Protocol
{
    std::string    hostname;
    std::string    password;

    private:
        void        _message_handler(MESSAGE recv, std::list<MESSAGE> &new_messages);
        int        _is_command(std::string command);
        Irc_message _irc_parser(std::string raw_msg);
        void        _command_handler(Irc_message msg, std::list<MESSAGE> &new_messages, int id);
        int         _protocol_command_handler(MESSAGE recv);
        
    public:
        Loby          loby;
        
    public:
        Protocol();
        Protocol(std::string pass);
        Protocol(Protocol const & src);
        ~Protocol();

    public:
        Protocol    &operator=(Protocol const & src);
    
    public:
        std::string    get_hostname();
        std::string    get_password();
        void           set_hostname(std::string hostname);
        void           set_password(std::string password);
    
    public:
        std::list<MESSAGE>      update(std::list<MESSAGE> messages);
        std::string             irc_message_to_client(int type, std::string to, std::string params);

    private:
        int    _nick_command(Irc_message msg, std::list<MESSAGE> &new_messages, int id);
        int    _user_command(Irc_message msg, std::list<MESSAGE> &new_messages, int id);
        int    _join_command(Irc_message msg, std::list<MESSAGE> &new_messages, int id);
        int    _privmsg_command(Irc_message msg, std::list<MESSAGE> &new_messages, int id);
        int    _ping_command(Irc_message msg, std::list<MESSAGE> &new_messages, int id);
        int    _quit_command(Irc_message msg, std::list<MESSAGE> &new_messages, int id);
        int    _part_command(Irc_message msg, std::list<MESSAGE> &new_messages, int id);
        int     _pass_command(Irc_message msg, std::list<MESSAGE> &new_messages, int id);



};


#endif
