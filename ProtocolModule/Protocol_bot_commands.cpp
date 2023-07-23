#include "Protocol.hpp"

int announce_channel(std::string to_ann, std::string channel_name, std::list<MESSAGE> &new_messages, Loby &loby);

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
    std:: cout << "BOT AUTH FAILED : " << bot_secret << " -> "<< secret << std::endl;
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

int     Protocol::_bot_command_handler(BOT msg, std::list<MESSAGE> &new_messages, int id)
{
    MESSAGE new_message;
    
    std::cout << "burda mıyım" << std::endl;
    if (msg.command == "ANN")
    {
        for (int i = 0; i < msg.params.size(); i++)
        {
            new_message.message += msg.params[i];
        }
        std::string to_ann;
        to_ann = ":irc_bot PRIVMSG " + msg.params[1] + " :" + new_message.message + "\r\n";
        std::cout  << to_ann << std::endl;
        announce_channel(new_message.message, msg.params[1], new_messages, this->loby);
        new_message.id = id;
        new_message.status = "";
        new_message.message = "OK\n";
        new_messages.push_back(new_message);
        std::cout << "sendeaaaaa" << std::endl;
    }
    std::cout << "BOT COMMAND HANDLER : " << msg.command << std::endl;
    return 1;
}