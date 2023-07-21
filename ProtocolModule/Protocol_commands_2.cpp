#include "Protocol.hpp"


static int message_to_channel(std::string from, std::string to, std::string message, std::list<MESSAGE> &new_messages, Loby &loby)
{
    std::list<User> users = loby.get_users_by_room_name(to);
    std::list<User>::iterator it;
    MESSAGE new_message;

    for (it = users.begin(); it != users.end(); it++)
    {
        if (it->get_nick_name() == from)
        {
            continue ;
        }
        new_message.id = it->get_id();
        new_message.message = ":" + from + " PRIVMSG " + to + " :" + message + "\r\n";
        new_messages.push_back(new_message);
    }
    return (0);
}


int Protocol::_privmsg_command(Irc_message msg, std::list<MESSAGE> &new_messages, int id)
{

    User *user_in_loby = loby.get_user(loby.get_user_by_id(id));

    if (user_in_loby)
    {
        std::string to = msg.params[0];
        std::string message = msg.params[1];
        std::string from = user_in_loby->get_nick_name();

        if (to[0] == '#')
        {
            message_to_channel(from, to, message, new_messages, loby);
        }
        else
        {
            MESSAGE new_message;
            new_message.id = loby.get_user_by_nick_name(to).get_id();
            new_message.message = ":" + from + " PRIVMSG " + to + " :" + message + "\r\n";
            new_messages.push_back(new_message);
        }
    }
    return 1;
}