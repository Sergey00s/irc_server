#include "Irc.hpp"



int main(int argc, char **argv)
{

    Server server(5324, 10);

  
    std::cout << "Server stopped" << std::endl;    
    return 0;
}