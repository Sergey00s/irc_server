#include "Protocol.hpp"

int announce_channel(std::string to_ann, std::string channel_name, std::list<MESSAGE> &new_messages, Loby &loby);

static int message_to_channel(std::string from, std::string to, std::string message, std::list<MESSAGE> &new_messages, Loby &loby)
{
    std::list<User> users = loby.get_users_by_room_name(to);
    std::list<User>::iterator it;
    MESSAGE new_message;

    int in_channel = 0;
    for (it = users.begin(); it != users.end(); it++)
    {
        if (it->get_nick_name() == from)
            continue ;
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
        if (msg.params.size() < 2)
        {
            return 0;
        }
        std::string to = msg.params[0];
        std::string message = msg.params[1];
        std::string from = user_in_loby->get_nick_name();

        if (to[0] == '#')
        {
            if (user_in_loby->rooms.in(to) == 0)
            {
                return 0;
            }
            message_to_channel(from, to, message, new_messages, loby);
        }
        else
        {
            MESSAGE new_message;
            
            User *user_to = loby.get_user(loby.get_user_by_nick_name(to));
            if (!user_to)
            {
                return 0;
            }
            new_message.id = user_to->get_id();
            new_message.message = ":" + from + " PRIVMSG " + to + " :" + message + "\r\n";
            new_messages.push_back(new_message);
        }
        priv_message new_priv_message;
        new_priv_message.from = from;
        new_priv_message.to = to;
        new_priv_message.message = message;
        this->last_messages.push_back(new_priv_message);
    }
    return 1;
}


int Protocol::_part_command(Irc_message msg, std::list<MESSAGE> &new_messages, int id)
{
    User *user_in_loby = loby.get_user(loby.get_user_by_id(id));
    MESSAGE new_message;

    if (user_in_loby)
    {
        if (msg.params.size() < 1)
        {   
            return 0;
        }
        std::string room_name = msg.params[0];

        user_in_loby->rooms.out(room_name);
        new_message.id = id;
        std::string to_ann =  ":" + user_in_loby->get_nick_name() + "!" + user_in_loby->get_user_name() + "@" + hostname + " PART " + room_name + "\r\n";
        new_message.status = "";
        new_message.message = to_ann;
        new_messages.push_back(new_message);
        announce_channel(to_ann, room_name, new_messages, loby);
    }

    return 1;
}

int Protocol::_kick_command(Irc_message msg, std::list<MESSAGE> &new_messages, int id)
{
    User *user_in_loby = loby.get_user(loby.get_user_by_id(id));
    MESSAGE new_message;

    if (user_in_loby)
    {
        if (msg.params.size() < 2)
        {
            return 0;
        }
        std::string room_name = msg.params[0];
        std::string nick_name = msg.params[1];
        User user_to_kick = loby.get_user_by_nick_name(nick_name);
        User *user_to_kick_in_loby = loby.get_user(user_to_kick);

        if (user_to_kick_in_loby)
        {
            if (user_in_loby->rooms.op_level(room_name) > 0)
            {
                user_to_kick_in_loby->rooms.out(room_name);
                new_message.id = user_to_kick_in_loby->get_id();
                std::string to_ann =  ":" + user_in_loby->get_nick_name() + "!" + user_in_loby->get_user_name() + "@" + hostname + " KICK " + room_name + " " + nick_name + "\r\n";
                new_message.status = "";
                new_message.message = to_ann;
                new_messages.push_back(new_message);
                announce_channel(to_ann, room_name, new_messages, loby);
            }
        }
    }
    return 1;
}