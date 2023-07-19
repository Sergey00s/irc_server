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
    if (type == ERR_NICKNAMEINUSE)
    {
        return "433";
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
            if (user.check_password(this->password))
            {
                msgto.message = irc_message_to_client(RPL_WELCOME, user.get_nick_name(), "Welcome to the Internet Relay Network");
                msgto.id = id;
                new_messages.push_back(msgto);
            }
            else
            {
                msgto.message = ":localhost 464 :Password required";
                msgto.id = id;
                new_messages.push_back(msgto);
            }
        }
        return 1;
}



static std::vector<std::string> get_nicks(std::list<User> users)
{
    std::vector<std::string> nicks;
    std::list<User>::iterator it;

    for (it = users.begin(); it != users.end(); it++)
    {
        nicks.push_back(it->get_nick_name());
    }
    return (nicks);
}

static int announce_channel(std::string to_ann, std::string channel_name, std::list<MESSAGE> &new_messages, Loby &loby)
{
    std::list<User> users = loby.get_users_by_room_name(channel_name);

    for (std::list<User>::iterator it = users.begin(); it != users.end(); it++)
    {
        MESSAGE msgto;
        msgto.message = to_ann;
        msgto.id = it->get_id();
        new_messages.push_back(msgto);
    }
    return (1);
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
    if (loby.move_user(*user_in_loby, channel_name))
    {
        user = *user_in_loby;
        //:Nick!Username@Hostname JOIN #Kanal
        std::string to_ann =  ":" + user.get_nick_name() + "!" + user.get_user_name() + "@" + hostname + " JOIN " + channel_name + "\r\n";
        announce_channel(to_ann, channel_name, new_messages, this->loby);
        //:irc.example.com 332 YourNick #hello :Welcome to the channel topic!
        msgto.message = ":localhost 332 " + user.get_nick_name() + " " + channel_name + " :Welcome to the channel topic!\r\n";
        msgto.id = id;
        new_messages.push_back(msgto);
        //:irc.example.com 353 YourNick = #hello :YourNick @YourNick2
        std::vector<std::string> nicks = get_nicks(loby.get_users_by_room_name(channel_name));
        std::string nicks_str = "";
        for (std::vector<std::string>::iterator it = nicks.begin(); it != nicks.end(); it++)
        {
            nicks_str += *it + " ";
        }
        msgto.message = ":localhost 353 " + user.get_nick_name() + " = " + channel_name + " :" + nicks_str + "\r\n";
        msgto.id = id;
        new_messages.push_back(msgto);
        //:irc.example.com 366 YourNick #hello :End of /NAMES list.
        msgto.message = ":localhost 366 " + user.get_nick_name() + " " + channel_name + " :End of /NAMES list.\r\n";
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
        return 0;
    if (user.get_status() != STATUS_REGISTERED)
    {
        MESSAGE msgto;
        msgto.message = irc_message_to_client(ERR_NOTREGISTERED, user.get_nick_name(), "You have not registered");
        msgto.id = id;
        new_messages.push_back(msgto);
        return 0;
    }
    if (msg.params[0][0] != '#')
    {
        //essage_to_channel(msg, new_messages, id);
        return 1;
    }
    else
    {
        MESSAGE msgto;
        //msgto.message = irc_message_to_client(ERR_NOSUCHNICK_CODE, user.get_nick_name(), "No such nick");
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

int Protocol::_part_command(Irc_message msg, std::list<MESSAGE> &new_messages, int id)
{
    return 1;
}



int Protocol::_pass_command(Irc_message msg, std::list<MESSAGE> &new_messages, int id)
{
    User user = loby.get_user_by_id(id);
    User *user_in_loby = loby.get_user(user);
    MESSAGE     msgto;

    user_in_loby->set_password(msg.params[0]);
    return 1;
}