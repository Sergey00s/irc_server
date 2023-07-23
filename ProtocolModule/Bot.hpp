#ifndef BOT_HPP

# define BOT_HPP

# include <iostream>
# include <vector>

/*
    **  Bot request protocol:
            BOT <BOT SECRET> <BOT COMMAND> <BOT PARAMS.......>
        **  Bot request example:
                BOT <123456789> <ANN> <#test> <Hello world!>
                BOT <123456789> <KICK> <#test> <user>
                BOT <123456789> <KICKFORCE> <user>

    **  Bot structure
        **  command:    command name
        **  params:     command params
        **  bot_secret: bot secret key
*/





struct BOT
{
    std::string                 command;
    std::string                 bot_secret;
    std::vector<std::string>    params;
};


#endif