#include "Protocol.hpp"

int reply(int status, std::list<MESSAGE> &new_messages, int id);

std::string decode_command(int type)
{

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
        int k;
        if (msg.params.size() < 1)
        {
            return 0;
        }
        k = (int)new_messages.size();
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
        return 1;
}

int Protocol::_user_command(Irc_message msg, std::list<MESSAGE> &new_messages, int id)
{
        int k;
        if (msg.params.size() < 4)
        {
            return 0;
        }

        k = (int)new_messages.size();
        std::string     user_name = msg.params[0];
        std::string     real_name = msg.params[3];
        User            user = loby.get_user_by_id(id);
        User            *user_in_loby = loby.get_user(user);

        user_in_loby->set_user_name(user_name);
        user_in_loby->set_real_name(real_name);
        return 1;
}



static std::vector<std::string> get_nicks(std::list<User> users, std::string room_name)
{
    std::vector<std::string> nicks;
    std::list<User>::iterator it;
    std::string nick;

    for (it = users.begin(); it != users.end(); it++)
    {
        
        nick = it->get_nick_name();
        if (it->rooms.op_level(room_name) > 0)
            nick = "@" + nick;
        nicks.push_back(nick);
    }
    return (nicks);
}

int announce_channel(std::string to_ann, std::string channel_name, std::list<MESSAGE> &new_messages, Loby &loby)
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
    if (msg.params.size() < 1)
    {
        return 0;
    }

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
    if (loby.move_user(user_in_loby, channel_name))
    {
        //:Nick!Username@Hostname JOIN #Kanal
        std::string to_ann =  ":" + user_in_loby->get_nick_name() + "!" + user_in_loby->get_user_name() + "@" + hostname + " JOIN " + channel_name + "\r\n";
        announce_channel(to_ann, channel_name, new_messages, this->loby);
        //:irc.example.com 332 YourNick #hello :Welcome to the channel topic!
        msgto.message = ":localhost 332 " + user_in_loby->get_nick_name() + " " + channel_name + " :Welcome to the channel topic!\r\n";
        msgto.id = id;
        new_messages.push_back(msgto);
        //:irc.example.com 353 YourNick = #hello :YourNick @YourNick2
        std::vector<std::string> nicks = get_nicks(loby.get_users_by_room_name(channel_name), channel_name);
        std::string nicks_str = "";
        for (std::vector<std::string>::iterator it = nicks.begin(); it != nicks.end(); it++)
        {
            nicks_str += *it + " ";
        }
        msgto.message = ":localhost 353 " + user_in_loby->get_nick_name() + " = " + channel_name + " :" + nicks_str + "\r\n";
        msgto.id = id;
        new_messages.push_back(msgto);
        //:irc.example.com 366 YourNick #hello :End of /NAMES list.
        msgto.message = ":localhost 366 " + user_in_loby->get_nick_name() + " " + channel_name + " :End of /NAMES list.\r\n";
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


int Protocol::_ping_command(Irc_message msg, std::list<MESSAGE> &new_messages, int id)
{
    User user = loby.get_user_by_id(id);
    MESSAGE     msgto;

    msgto.message = irc_message_to_client(RPL_PONG, user.get_nick_name(), msg.params[0]);
    msgto.id = id;
    new_messages.push_back(msgto);
    return 1;
}




int Protocol::_pass_command(Irc_message msg, std::list<MESSAGE> &new_messages, int id)
{
    int k;
    if (msg.params.size() < 1)
    {
        return 0;
    }
    
    k = new_messages.size();
    User user = loby.get_user_by_id(id);
    User *user_in_loby = loby.get_user(user);
    MESSAGE     msgto;

    user_in_loby->set_password(msg.params[0]);
    return 1;
}

int Protocol::_quit_command(Irc_message msg, std::list<MESSAGE> &new_messages, int id)
{
    User user = loby.get_user_by_id(id);
    User *user_in_loby = loby.get_user(user);
    MESSAGE     msgto;
    std::string _null = "";

    if (msg.params.size() <= 0)
        msg.params.push_back(_null);
    std::string to = ":" + user.get_nick_name() + "!" + user.get_user_name() + "@" + hostname + " QUIT " + msg.params[0] + "\r\n";
    std::list<ROOM> uroom = user_in_loby->rooms.rooms;
    for (std::list<ROOM>::iterator it = uroom.begin(); it != uroom.end(); it++)
    {
        announce_channel(to, it->room_name, new_messages, this->loby);
    }
    msgto.message = ":localhost 400 " + user.get_nick_name() + " :Quit: " + msg.params[0] + "\r\n";
    msgto.id = id;
    loby.remove_user(*user_in_loby);
    new_messages.push_back(msgto);
    msgto.message = "";
    msgto.status = "0000";
    msgto.id = id;
    new_messages.push_back(msgto);
    return 1;
}

