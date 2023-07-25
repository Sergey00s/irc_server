#ifndef PROTOCOL_HPP
# define PROTOCOL_HPP


# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <list>
# include "Loby.hpp"
# include "Scodes.hpp"
# include "Bot.hpp"

#define PRIVMSG 852
#define JOIN 853
#define NICK 854
#define USER 855
#define PASS 856
#define PING 857
#define PONG 858
#define TOPIC 859


BOT bot_parse(std::string raw_msg);
int     bot_auth(std::string bot_secret, std::string secret);
int     _bot_is_bot(std::string raw_msg);

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


struct priv_message
{
    std::string     from;
    std::string     to;
    std::string     message;
};


class Protocol
{
    std::string    hostname;
    std::string    password;
    std::string    bot_secret;

    private:
        std::list<priv_message> last_messages;

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
        std::string    get_bot_secret();
        void           set_hostname(std::string hostname);
        void           set_password(std::string password);
        void           set_bot_secret(std::string bot_secret);

    
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
        int     _kick_command(Irc_message msg, std::list<MESSAGE> &new_messages, int id);


    private:
        int     _bot_command_handler(BOT msg, std::list<MESSAGE> &new_messages, int id);
        int     _bot_get_last_messages(BOT msg, std::list<MESSAGE> &new_messages, int id);
        int     _bot_get_users(BOT msg, std::list<MESSAGE> &new_messages, int id);
        int     _bot_get_channels(BOT msg, std::list<MESSAGE> &new_messages, int id);


};


#endif
