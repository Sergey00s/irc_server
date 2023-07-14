#include "Protocol.hpp"

std::string decode_command(int type)
{
    int command_code;

    if (type == RPL_WELCOME)
    {
        return RPL_LOGIN_CODE;
    }
    if (type == ERR_NOSUCHCHANNEL)
    {
        return "403";
    }
    if (type == ERR_NOTREGISTERED)
    {
        return "451";
    }
    if (type == RPL_TOPIC)
    {
        return "332";
    }
    if (type == RPL_NAMELIST)
    {
        return "353";
    }
    if (type == RPL_ENDOFNAMES)
    {
        return "366";
    }
    if (type == RPL_YOURHOST)
    {
        return "002";
    }
    if (type == RPL_NAMREPLY)
    {
        return "353";
    }
    if (type == RPL_ENDOFNAMES)
    {
        return "366";
    }
    if (type == RPL_PONG)
    {
        return "PONG";
    }
    return ("");
}

std::string Protocol::irc_message_to_client(int type, std::string to, std::string params)
{
    std::string     message;
    std::string     prefix;
    std::string     command;
    int             i = 0;


    command = decode_command(type);
    message = ":" + hostname + " " + command + " " + to + " " + ":" + params + "\r\n";
    return (message);
}

static int check_nick(std::string nick, Loby &loby)
{
    std::list<User> users = loby.get_users();
    std::list<User>::iterator it;

    for (it = users.begin(); it != users.end(); it++)
    {
        if (it->get_nick_name() == nick)
            return (1);
    }
    return (0);
}


int Protocol::_nick_command(Irc_message msg, std::list<MESSAGE> &new_messages, int id)
{
        std::string     new_nick = msg.params[0];
        User    user = loby.get_user_by_id(id);
        User    *user_in_loby = loby.get_user(user);

        if (user_in_loby)
        {
            int times = 0;
            while (check_nick(new_nick, loby) == 1)
            {
                times++;
                new_nick += "(" + std::to_string(times) + "xFake)";
            }
            user_in_loby->set_nick_name(new_nick);
        }
        else
        {
            int times = 0;
            while (check_nick(new_nick, loby) == 1)
            {
                times++;
                new_nick += "(" + std::to_string(times) + "xFake)";
            }
            user.set_nick_name(new_nick);
            user.set_status(STATUS_CONNECTED);
            user.set_id(id);
            loby.add_user(user);
        }
        return 1;
}

int Protocol::_user_command(Irc_message msg, std::list<MESSAGE> &new_messages, int id)
{
        std::string     user_name = msg.params[0];
        std::string     real_name = msg.params[3];
        User            user = loby.get_user_by_id(id);
        User            *user_in_loby = loby.get_user(user);

        if (user.get_id() != -1 && user.get_status() == STATUS_REGISTERED)
        {
            user_in_loby->set_user_name(user_name);
            user_in_loby->set_real_name(real_name);
        }
        else if (user.get_id() != -1 && user.get_status() == STATUS_CONNECTED)
        {
            MESSAGE     msgto;
            user.set_user_name(user_name);
            user.set_real_name(real_name);
            user.set_id(id);
            user.set_status(STATUS_REGISTERED);
            *user_in_loby = user;
            msgto.message = irc_message_to_client(RPL_WELCOME, user.get_nick_name(), "Welcome to the Internet Relay Network");
            msgto.id = id;
            new_messages.push_back(msgto);
        }
        return 1;
}

int Protocol::_join_command(Irc_message msg, std::list<MESSAGE> &new_messages, int id)
{
    User user = loby.get_user_by_id(id);
    User *user_in_loby = loby.get_user(user);
    std::string channel_name = msg.params[0];
    MESSAGE     msgto;

    if (user.get_status() != STATUS_REGISTERED)
    {
        msgto.message = irc_message_to_client(ERR_NOTREGISTERED, user.get_nick_name(), "You have not registered");
        msgto.id = id;
        new_messages.push_back(msgto);
        return 0;
    }
    if (channel_name[0] != '#')
    {
        msgto.message = irc_message_to_client(ERR_NOSUCHCHANNEL, user.get_nick_name(), "No such channel");
        msgto.id = id;
        new_messages.push_back(msgto);
        return 0;
    }
    if (loby.move_user(user, channel_name))
    {
        msgto.message = irc_message_to_client(RPL_TOPIC, user.get_nick_name(), channel_name + " :No topic is set");
        msgto.id = id;
        new_messages.push_back(msgto);
        return 1;
    }
    else
    {
        msgto.message = irc_message_to_client(ERR_NOSUCHCHANNEL, user.get_nick_name(), "No such channel");
        msgto.id = id;
        new_messages.push_back(msgto);
        return 0;
    }
    return 1;
}


int Protocol::_privmsg_command(Irc_message msg, std::list<MESSAGE> &new_messages, int id)
{
    User user = loby.get_user_by_id(id);
    User *user_in_loby = loby.get_user(user);
    if (msg.params.size() < 2)
    {
        MESSAGE     msgto;
        //msgto.message = irc_message_to_client(ERR_NEEDMOREPARAMS, std::vector<std::string>());
        msgto.id = id;
        new_messages.push_back(msgto);
        return 0;
    }
    return 1;
}

int Protocol::_ping_command(Irc_message msg, std::list<MESSAGE> &new_messages, int id)
{
    User user = loby.get_user_by_id(id);
    User *user_in_loby = loby.get_user(user);
    MESSAGE     msgto;

    msgto.message = irc_message_to_client(RPL_PONG, user.get_nick_name(), msg.params[0]);
    msgto.id = id;
    new_messages.push_back(msgto);
    return 1;
}