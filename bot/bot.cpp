#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <list>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>

# include <unistd.h>

# include <fcntl.h>

std::string to_server(std::string msg)
{
    std::string first = "BOT 123456789 ANO ";
    return first + msg;
}


int main(int argc, char const *argv[])
{
    
    return 0;
}
