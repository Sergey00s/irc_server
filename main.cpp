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

    Protocol protocol;
    protocol.set_hostname("localhost");
    int port = 6667;
    if (argc > 1)
        port = atoi(argv[1]);
    if (argc > 2)
        protocol.set_password(argv[2]);
    Server server(port, 5);
    while (1)
    {
        if (!server.update())
            continue;
        if (server.check_read(server.get_socket()))
            server.accept_connection();

        server.update_messages();
        std::list<Message> messages = server.get_messages();
        std::list<MESSAGE> to_be_msg = protocol.update(convert_to_protocol_meesage_list(messages));
        server.send_messages(convert_to_server_message_list(to_be_msg));
    }
    
  
    return 0;
}