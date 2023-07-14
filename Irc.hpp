#ifndef IRC_HPP
#define IRC_HPP


#include <iostream> 
#include "ServerModule/Server.hpp"
#include "ProtocolModule/Protocol.hpp"

/*

user status 300 - 399:
    301 : connected
    302 : disconnected
    303 : registered



*/


/*

login procces -> 
    NICK <nickname>
    USER <username> <hostname> <servername> <realname>
    PASS <password>

    RPL_WELCOME -> :<servername> 001 <nickname> :Welcome to the Internet Relay Network


*/





#endif // IRC_HPP
