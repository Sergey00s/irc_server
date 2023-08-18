#include "Protocol.hpp"

int announce_channel(std::string to_ann, std::string channel_name, std::list<MESSAGE> &new_messages, Loby &loby);

int reply(int status, std::list<MESSAGE> &new_messages, int id)
{
    MESSAGE new_message;
    new_message.id = id;
    new_message.status = "";
    if (status == 1)
        new_message.message = "OK -END-\n";
    else
        new_message.message = "KO -END-\n";
    new_messages.push_back(new_message);
    return 1;
}

int reply_parse(std::string raw, int con)
{
    if (con)
        raw = raw;
    else
        raw = raw + "-END-\n";
    return 1;
}

BOT bot_parse(std::string raw_msg)
{
    std::vector <std::string>   params;
    BOT                         bot;

    raw_msg = raw_msg.substr(4, raw_msg.size() - 4);

    if (raw_msg.size() <= 1)
        return bot;
    if (raw_msg[raw_msg.size() - 1] == '\n')
        raw_msg.erase(raw_msg.size() - 1);
    if (raw_msg[raw_msg.size() - 1] == '\r')
        raw_msg.erase(raw_msg.size() - 1);

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
        i++;
    }
    if (raw_msg.size() > 0)
        params.push_back(raw_msg);
        bot.command = params[1];
        bot.bot_secret = params[0];
        params.erase(params.begin());
        params.erase(params.begin());
        bot.params = params;

    return bot;
}


int     bot_auth(std::string bot_secret, std::string secret)
{
    if (bot_secret == secret)
        return 1;
    return 0;
}


int     _bot_is_bot(std::string raw_msg)
{
    std::vector <std::string>   params;

    for (int i = 0; i < raw_msg.size(); i++)
    {
        if (raw_msg[i] == ' ')
        {
            params.push_back(raw_msg.substr(0, i));
            raw_msg = raw_msg.substr(i + 1, raw_msg.size() - i);
            i = 0;
            continue;
        }
    }
    if (params.size() > 0 && params[0] == "BOT")
            return 1;
    return 0;
}  

static int anon_all(std::string msg, std::list<MESSAGE> &new_messages, int id, Loby &loby)
{
    std::list<User> users = loby.get_users();
    std::list<std::string> channels = loby.get_rooms();
    std::list<std::string>::iterator it;
    for (it = channels.begin(); it != channels.end(); it++)
    {
        std::string to_ann;
        to_ann = ":irc_bot PRIVMSG " + *it + " :" + msg + "\r\n";
        announce_channel(to_ann, *it, new_messages, loby);
    }

    return 1;
}


int     Protocol::_bot_command_handler(BOT msg, std::list<MESSAGE> &new_messages, int id)
{
    MESSAGE new_message;
    


    std::string msg_to_send;

    for (int i = 0; i < msg.params.size(); i++)
    {
        msg_to_send += msg.params[i];
        if (i != msg.params.size() - 1)
            msg_to_send += " ";
    }
    if (msg.command == "ANO")
    {
        anon_all(msg_to_send, new_messages, id, this->loby);
    }
    return 1;
}

int Protocol::_bot_get_last_messages(BOT msg, std::list<MESSAGE> &new_messages, int id)
{
    MESSAGE new_message;
    std::string message;
    std::list<priv_message>::iterator it;
    int i = 0;
    if (msg.params.size() < 1)
        return reply(0, new_messages, id);
    std::string channel = msg.params[0];
    new_message.id = id;
    new_message.message = "OK\n";
    new_messages.push_back(new_message);
    new_message.message = "";

    for (it = this->last_messages.begin(); it != this->last_messages.end(); it++)
    {
        if (it->to == channel)
        {
            new_message.id = id;
            new_message.status = "";
            new_message.message = it->from + " " + it->message + "\n";
            new_messages.push_back(new_message);
        }
    }
    new_message.id = id;
    new_message.status = "";
    new_message.message = "-END-\n";
    new_messages.push_back(new_message);
    return 1;
}

int Protocol::_bot_get_channels(BOT msg, std::list<MESSAGE> &new_messages, int id)
{
    MESSAGE new_message;
    new_message.id = id;
    new_message.status = "";
    new_message.message = "OK ";
    new_messages.push_back(new_message);
    new_message.message = "";

    std::list<User>::iterator it;
    std::list<std::string> channels;

    for (it = this->loby.users.begin(); it != this->loby.users.end(); it++)
    {
        std::list <ROOM>::iterator it2;

        for (it2 = it->rooms.rooms.begin(); it2 != it->rooms.rooms.end(); it2++)
        {
            if (std::find(channels.begin(), channels.end(), it2->room_name) == channels.end())
                channels.push_back(it2->room_name);
        }
    }
    std::list<std::string>::iterator it3;
    for (it3 = channels.begin(); it3 != channels.end(); it3++)
    {
        new_message.message = *it3 + "\n";
        new_messages.push_back(new_message);
    }
    new_message.message = "-END-\n";
    new_messages.push_back(new_message);
    return 1;
}