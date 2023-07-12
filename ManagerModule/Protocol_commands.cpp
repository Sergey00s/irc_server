#include "Protocol.hpp"


int Protocol::_nick_command(Irc_message msg, std::list<MESSAGE> &new_messages, int id)
{
        std::string     new_nick = msg.params[0];
        User    user = loby.get_user_by_id(id);
        User    *user_in_loby = loby.get_user(user);

        if (user_in_loby)
        {
            user_in_loby->set_nick_name(new_nick);
        }
        else
        {
            user.set_nick_name(new_nick);
            user.set_status(STATUS_CONNECTED);
            user.set_id(id);
            loby.add_user(user);
        }
        return;
}

int Protocol::_user_command(Irc_message msg, std::list<MESSAGE> &new_messages, int id)
{
        std::string     user_name = msg.params[0];
        std::string     real_name = msg.params[3];
        User            user = loby.get_user_by_id(id);
        User            *user_in_loby = loby.get_user(user);

        if (user_in_loby)
        {
            user_in_loby->set_user_name(user_name);
            user_in_loby->set_real_name(real_name);
        }
        else
        {
            MESSAGE     msgto;
            user.set_user_name(user_name);
            user.set_real_name(real_name);
            user.set_id(id);
            user.set_status(STATUS_REGISTERED);
            loby.add_user(user);
            msgto.message = irc_message_to_client(RPL_WELCOME, std::vector<std::string>());
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
        msgto.message = irc_message_to_client(ERR_NOTREGISTERED, std::vector<std::string>());
        msgto.id = id;
        new_messages.push_back(msgto);
        return 0;
    }
    if (channel_name[0] != '#')
    {
        msgto.message = irc_message_to_client(ERR_NOSUCHCHANNEL, std::vector<std::string>());
        msgto.id = id;
        new_messages.push_back(msgto);
        return 0;
    }
    if (loby.move_user(user, channel_name))
    {
        msgto.message = irc_message_to_client(RPL_TOPIC, std::vector<std::string>());
        msgto.id = id;
        new_messages.push_back(msgto);
        return 1;
    }
    else
    {
        msgto.message = irc_message_to_client(ERR_NOSUCHCHANNEL, std::vector<std::string>());
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
        msgto.message = irc_message_to_client(ERR_NEEDMOREPARAMS, std::vector<std::string>());
        msgto.id = id;
        new_messages.push_back(msgto);
        return 0;
    }
    return 1;
}
