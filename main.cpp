#include "Irc.hpp"



MESSAGE  convert_to_protocol_message(Message msg)
{
    MESSAGE     message;

    message.message = msg.message;
    message.id = msg.client_socket;
    message.status = msg.status_out;
    return (message);
}

Message convert_to_server_message(MESSAGE msg)
{
    Message     message;

    message.message = msg.message;
    message.client_socket = msg.id;
    message.status_out = msg.status;
    return (message);
}


std::list<MESSAGE> convert_to_protocol_meesage_list(std::list<Message> messages)
{
    std::list<MESSAGE>  new_messages;
    std::list<Message>::iterator it;

    for (it = messages.begin(); it != messages.end(); it++)
    {
        new_messages.push_back(convert_to_protocol_message(*it));
    }
    return (new_messages);
}

std::list<Message> convert_to_server_message_list(std::list<MESSAGE> messages)
{
    std::list<Message>  new_messages;
    std::list<MESSAGE>::iterator it;

    for (it = messages.begin(); it != messages.end(); it++)
    {
        new_messages.push_back(convert_to_server_message(*it));
    }
    return (new_messages);
}





int main(int argc, char **argv)
{

    Server server(6667, 10);
    Protocol protocol;
    
    protocol.set_hostname("localhost");
    protocol.set_password("pass");
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
        // for (std::list<Message>::iterator it = messages.begin(); it != messages.end(); it++)
        // {
        //     std::cout << "Message received: " << it->message << std::endl;
        // }
        std::list<MESSAGE> to_be_msg = protocol.update(convert_to_protocol_meesage_list(messages));
        for (std::list<MESSAGE>::iterator it = to_be_msg.begin(); it != to_be_msg.end(); it++)
        {
           std::cout << "Message to send: " << it->message << std::endl;
        }
        server.send_messages(convert_to_server_message_list(to_be_msg));
    }
    
  
    return 0;
}