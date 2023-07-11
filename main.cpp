#include "Irc.hpp"



int main(int argc, char **argv)
{

    Server server(5324, 10);

    while (1)
    {
        server.update();
        if (server.check_read(server.get_socket()))
        {
            server.accept_connection();
            std::cout << "New connection" << std::endl;
        }
        server.update_messages();
        std::list<Message> messages = server.get_messages();
        //std::list<Message> processed_messages = process_messages(messages);
        //server.send_messages(processed_messages);
    }
    
  
    return 0;
}